#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <pigpiod_if2.h>
#include <sys/stat.h>

#include "color.h"
#include "client.h"

#define RED_PIN 17
#define GREEN_PIN 22
#define BLUE_PIN 24


void listen_loop();
void listen_loop2(int pi);
void fork_child_task(int pi);
int process_request(int pi, int sock);

int main(int argc, const char* argv[])
{
	if(argc >= 2)
	{
		//create a binary code for each different argumant so we can use a switch/case statement here.
		if(strcmp(argv[1], "-c")==0)
		{
			int pi = pigpio_start(NULL, NULL);
			listen_loop2(pi);
		}else if (strcmp(argv[1], "-t")==0)
		{
			test_();
		}else if(strcmp(argv[1], "-d")==0)
		{
			int pi = pigpio_start(NULL, NULL);
			int pid = fork();
			if(pid < 0)
			{
				exit(1);
			}else if(pid == 0)
			{
				umask(0);
				int sid = setsid();
				
				if(sid<0)
				{
					printf("error: setsid() failed.\n");
					fflush(stdout);
					exit(1);
				}
				chdir("/");
				
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				close(STDERR_FILENO);
				listen_loop2(pi);
				exit(0);
			}else
			{
				printf("daemon main process ID: %d\n", pid);
				exit(0);
			}
		}else if(strcmp(argv[1], "-gc")==0)
		{
			int pi = pigpio_start(NULL, NULL);
			int pid = fork();
			if(pid < 0)
			{
				exit(1);
			}else if(pid == 0)
			{
				umask(0);
				int sid = setsid();
				
				if(sid<0)
				{
					printf("error: setsid() failed.\n");
					fflush(stdout);
					exit(1);
				}
				chdir("/");
				
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				close(STDERR_FILENO);
				fork_child_task(pi);
				exit(0);
			}else
			{
				printf("daemon main process ID: %d\n", pid);
				exit(0);
			}
		}
		else
		{
			client_handle_input(argc, argv);
		}
	}else
	{
		listen_loop();
	}
	return 0;
}

int process_request(int pi, int sock)
{
	/*
		command state
		command#	client_command		description
			0			n/a					unused
			1			halt				client requests that server be stopped.
			2			stclr				set pins to given rgb values (0-255).
			3			n/a					signal from forked task requesting the current rgb values.
			4			n/a					signal from forked task requesting to update current rgb values.
			4			getcolor			output current color to console.
			5			n/a					used by color cycle process to communicate with the main server
			6			
		
	*/
	int command = 0;
	read(sock, &command, sizeof(int));
	return command;
}

void fork_child_task(int pi)
{
	struct sockaddr servaddr = {AF_UNIX, "colorserver"};
	socklen_t servaddrlen = sizeof(struct sockaddr)+12;
	//init values of color cycle
	int r = 0;
	int g = 0;
	int b = 0;
	int command = 0;
	float wavelength = 10.0;
	float time = 0.0;
	set_PWM_dutycycle(pi, RED_PIN, r);
	set_PWM_dutycycle(pi, GREEN_PIN, g);
	set_PWM_dutycycle(pi, BLUE_PIN, b);
	const struct timespec delay = {0, 50000000};
	struct timespec catch;
	
	float time_step = delay.tv_nsec / 1000000000.0;
	
	while(nanosleep(&delay, &catch) != -1)
	{
		
		//printf("delta color loop is running...\n");
		//attempt to connect to server to get current values
		command = 5;
		int sock = socket(AF_UNIX, SOCK_STREAM, 0);
		if(sock != -1)
		{		
			if(connect(sock, &servaddr, servaddrlen)!=-1)
			{
				write(sock, &command, sizeof(int));
				
				read(sock, &r, sizeof(int));
				read(sock, &g, sizeof(int));
				read(sock, &b, sizeof(int));
				//changes to color values below
				
				r = (int)sin_color_cycle(time, wavelength, 0, 0);
				g = (int)sin_color_cycle(time, wavelength, 0, 255);
				b = (int)sin_color_cycle(time, wavelength, 0, 10);
				printf("%d\n",g);
				//attempts to update server values
				write(sock, &r, sizeof(int));
				write(sock, &g, sizeof(int));
				write(sock, &b, sizeof(int));
				close(sock);
			}
		}
		time += time_step;
	}
}

void listen_loop()
{
	int pi = pigpio_start(NULL, NULL);
	struct sockaddr servaddr = {AF_UNIX, "colorserver"};
	
	socklen_t servaddrlen = sizeof(struct sockaddr)+12;
	
	int sock1 = socket(AF_UNIX, SOCK_STREAM, 0);
	
	if(sock1 == -1)
	{
		printf("\nFailed to create socket, aborting...\n");
		pigpio_stop(pi);
		exit(1);
	}
	printf("\nSocket Created successfully!\n");
	bind(sock1, &servaddr, servaddrlen);
	
	if( listen(sock1, 0) != -1 )
	{
		printf("Server now listening for connections...\n");
		int var = 0;
		struct sigaction news, olds;
		void ctrlc()
		{
			printf("Halting Color Server...\n");
			sigaction(SIGINT, &olds, NULL);
			close(sock1);
			var+=1;
			unlink("colorserver");
		}
		news.sa_handler = ctrlc;
		news.sa_flags = 0;
		sigaction(SIGINT, &news, &olds);
		while(var == 0)
		{
			int sock2 = accept(sock1, NULL, NULL);
			if(sock2!=-1)
			{
				printf("Connection accepted...\n");
				int resp_code = process_request(pi, sock2);
				close(sock2);
				if(resp_code == 1)
				{
					ctrlc();
				}
				
			}else
			{
				printf("Connect failed...\n");
			}
		}
		
	}else
	{
		printf("Listen failed, aborting...\n");
		pigpio_stop(pi);
		exit(1);
		
	}
	pigpio_stop(pi);
	
}

void listen_loop2(int pi)
{
	FILE* log = NULL;
	log = fopen("pcs_log.txt", "w+");
	//int status;
	
	struct sockaddr servaddr = {AF_UNIX, "colorserver"};
	
	socklen_t servaddrlen = sizeof(struct sockaddr)+12;
	
	int sock1 = socket(AF_UNIX, SOCK_STREAM, 0);
	
	if(sock1 == -1)
	{
		printf("\nFailed to create socket, aborting...\n");
		pigpio_stop(pi);
		fclose(log);
		exit(1);
	}
	printf("\nSocket Created successfully with name/id \"colorserver\"\n");
	bind(sock1, &servaddr, servaddrlen);
	
	if( listen(sock1, 0) != -1 )
	{
		printf("Server now listening for connections...\n");
		int var = 0;
		struct sigaction news, olds;
		void ctrlc()
		{
			printf("\nHalting Color Server...\n");
			sigaction(SIGINT, &olds, NULL);
			close(sock1);
			var+=1;
			unlink("colorserver");
			set_PWM_dutycycle(pi, RED_PIN, 0);
			set_PWM_dutycycle(pi, GREEN_PIN, 0);
			set_PWM_dutycycle(pi, BLUE_PIN, 0);
			pigpio_stop(pi);
		}
		
		news.sa_handler = ctrlc;
		news.sa_flags = 0;
		sigaction(SIGINT, &news, &olds);
		int r = 0;
		int g = 0;
		int b = 10;
		int colorchanged = 0;
		int num = 0;
		int value = 0;
		while(var == 0)
		{
			int sock2 = accept(sock1, NULL, NULL);
			if(sock2!=-1)
			{
				int resp_code = process_request(pi, sock2);
				
				switch(resp_code)
				{
					case 1:
						close(sock2);
						ctrlc();
						break;
					
					case 2:
						if(colorchanged == 0)
						{
							read(sock2, &r, sizeof(int));
							read(sock2, &g, sizeof(int));
							read(sock2, &b, sizeof(int));
							colorchanged = 1;
						}
						close(sock2);
						break;
					case 3:
					case 4:
						write(sock2, &r, sizeof(int));
						write(sock2, &g, sizeof(int));
						write(sock2, &b, sizeof(int));
						close(sock2);
						break;
					case 5:
						write(sock2, &r, sizeof(int));
						write(sock2, &g, sizeof(int));
						write(sock2, &b, sizeof(int));
						if(colorchanged == 0)
						{
							read(sock2, &r, sizeof(int));
							read(sock2, &g, sizeof(int));
							read(sock2, &b, sizeof(int));
							colorchanged = 1;
						}
						close(sock2);
						break;
					case 6:
						if(colorchanged == 0)
						{
							read(sock2, &num, sizeof(int));
							read(sock2, &value, sizeof(int));
							switch(num)
							{
								case 82:
								case 114:
									//red
									r = value;
									colorchanged = 1;
									break;
								case 71:
								case 103:
									//green
									g = value;
									colorchanged = 1;
									break;
								case 66:
								case 98:
									//blue
									b = value;
									colorchanged = 1;
									break;
							}
						}
						close(sock2);
						break;
					default:
						close(sock2);
						break;
				}
				//printf("red = %d\tgreen = %d\tblue = %d\n", r, g, b);
				if(colorchanged==1)
				{
					set_PWM_dutycycle(pi, RED_PIN, r);
					set_PWM_dutycycle(pi, GREEN_PIN, g);
					set_PWM_dutycycle(pi, BLUE_PIN, b);
					colorchanged = 0;
				}
			}
		}
		
	}else
	{
		printf("Listen failed, aborting...\n");
		pigpio_stop(pi);
		fclose(log);
		exit(1);
		
	}
	fclose(log);
}
