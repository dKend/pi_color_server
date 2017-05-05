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

#define RED_PIN 17
#define GREEN_PIN 22
#define BLUE_PIN 24

void listen_loop();
void listen_loop2();
void fork_child_task(int pi);
int process_request(int pi, int sock);

int main(int argc, const char* argv[])
{
	if(argc >= 2)
	{
		if(strcmp(argv[1], "-c")==0)
		{
			listen_loop2();
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
		
	*/
	int command = 0;
	unsigned int r;
	unsigned int g;
	unsigned int b;
	if(read(sock, &command, sizeof(int))==0)
	{
		printf("error: unable to read command!\n");
	}
	if(command == 2)
	{
		read(sock, &r, sizeof(int));
		read(sock, &g, sizeof(int));
		read(sock, &b, sizeof(int));
		if(r>255)
		{
			r=255;
		}
		
		if(g>255)
		{
			g=255;
		}
		
		if(b>255)
		{
			b=255;
		}
		
		set_PWM_dutycycle(pi, RED_PIN, r);
		set_PWM_dutycycle(pi, GREEN_PIN, g);
		set_PWM_dutycycle(pi, BLUE_PIN, b);
	}
	return command;
}

void fork_child_task(int pi)
{
	struct sockaddr servaddr = {AF_UNIX, "colorserver"};
	socklen_t servaddrlen = sizeof(struct sockaddr)+12;
	
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	
	if(sock != -1)
	{
		//init values of color cycle
		int r = 50;
		int g = 0;
		int b = 10;
		int g_desc = 1;
		int step = 1;
		int command = 0;
		set_PWM_dutycycle(pi, RED_PIN, r);
		set_PWM_dutycycle(pi, GREEN_PIN, g);
		set_PWM_dutycycle(pi, BLUE_PIN, b);
		const struct timespec delay = {0, 50000000};
		struct timespec catch;
		while(nanosleep(&delay, &catch) != -1)
		{
			//attempt to connect to server to get current values
			command = 3
			if(connect(sock, &servaddr, servaddrlen)!=-1)
			{
				write(sock, &command, sizeof(int));
				read(sock, &r, sizeof(int));
				read(sock, &g, sizeof(int));
				read(sock, &b, sizeof(int));
				close(sock);
			}
			//changes to color values below
			if(g_desc==1)
			{
				g+=step;
			}else if(g_desc==0)
			{
				g-=step;
			}
			
			
			if(g >= 255)
			{
				g = 255;
				g_desc = 0;
			}else if(g <= 0)
			{
				g = 0;
				g_desc = 1;
			}
			
			set_PWM_dutycycle(pi, RED_PIN, r);
			set_PWM_dutycycle(pi, GREEN_PIN, g);
			set_PWM_dutycycle(pi, BLUE_PIN, b);
			
			//attempts to update server values
			command = 4;
			if(connect(sock, &servaddr, servaddrlen)!=-1)
			{
				write(sock, &command, sizeof(int));
				write(sock, &r, sizeof(int));
				write(sock, &g, sizeof(int));
				write(sock, &b, sizeof(int));
				close(sock);
			}
		}
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

void listen_loop2()
{
	//int status;
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
	printf("\nSocket Created successfully with name/id \"colorserver\"\n");
	bind(sock1, &servaddr, servaddrlen);
	
	if( listen(sock1, 0) != -1 )
	{
		pid_t fd = fork();
		
		if(fd == 0) //child task
		{
			fork_child_task(pi);
			exit(1);
		}else if(fd>0) //parent task
		{
			printf("Server now listening for connections...\n");
			int var = 0;
			struct sigaction news, olds;
			int status;
			void ctrlc()
			{
				printf("\nHalting Color Server...\n");
				sigaction(SIGINT, &olds, NULL);
				close(sock1);
				var+=1;
				unlink("colorserver");
				if(waitpid(fd, &status, WNOHANG) == 0)
				{
					kill(fd, SIGKILL);
				}
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
			int b = 0;
			while(var == 0)
			{
				int sock2 = accept(sock1, NULL, NULL);
				if(sock2!=-1)
				{
					printf("Connection accepted...\n");
					int resp_code = process_request(pi, sock2);
					if(resp_code == 1)
					{
						close(sock2);
						ctrlc();
					}else if(resp_code == 3)
					{
						write(sock, &r, sizeof(int));
						write(sock, &g, sizeof(int));
						write(sock, &b, sizeof(int));
						close(sock2);
					}else if(resp_code == 4)
					{
						read(sock, &r, sizeof(int));
						read(sock, &g, sizeof(int));
						read(sock, &b, sizeof(int));
						close(sock2);
					}else
					{
						close(sock2);
					}
				}
			}
		}else
		{
			printf("Fork failed, aborting...\n");
			pigpio_stop(pi);
			exit(1);
		}
		
	}else
	{
		printf("Listen failed, aborting...\n");
		pigpio_stop(pi);
		exit(1);
		
	}
}
