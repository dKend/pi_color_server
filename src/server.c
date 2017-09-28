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
#include <signal.h>

#define RED_PIN 17
#define GREEN_PIN 22
#define BLUE_PIN 24


void listen_loop2(int pi);
int process_request(int pi, int sock);
void server_tests();
int log_output(const char * str, FILE* log);

/*
		command state
		syntax		command#	client_command		description
					0			n/a					unused
		-halt		1			halt				client requests that server be stopped.
		-stclr		2			stclr				set pins to given rgb values (0-255).
					3			n/a					signal from forked task requesting the current rgb values.
		-getcolor	4			getcolor			output current color to console.
					5			n/a					used by color cycle process to communicate with the main server
					6			
		
	*/

int main(int argc, const char* argv[])
{
	if(argc >= 2)
	{
		//create a binary code for each different argument so we can use a switch/case statement here.
		if (strcmp(argv[1], "-t")==0) //run tests
		{
			
		}
		else if(strcmp(argv[1], "-d")==0) //daemon process
		{
			int pi = pigpio_start(NULL, NULL);
			int pid = fork();
			if(pid < 0)
			{
				exit(1);
			}
			else if(pid == 0)
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
				if(access("colorserver", F_OK)!=-1)
                                {
                                        printf("error: server already running or socket file already exists.\n");
                                        fflush(stdout);
                                        exit(1);
                                }
				
				
				
				
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				close(STDERR_FILENO);
				listen_loop2(pi);
				exit(0);
			}
			else
			{
				printf("daemon main process ID: %d\n", pid);
				exit(0);
			}
		}
		else //handle with client otherwise
		{
			printf("invald arguments.\n");
			fflush(stdout);
		}
	}
	else
	{
		int pi = pigpio_start(NULL, NULL);
		listen_loop2(pi);
	}
	
	return 0;
}

int process_request(int pi, int sock)
{
	
	int command = 0;
	read(sock, &command, sizeof(int));
	return command;
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
		int tr = 0;
		int tg = 255;
		int tb = 10;
		int colorchanged = 0;
		int targetchanged = 0;
		int num = 0;
		int value = 0;
		int tmp;
		int stopcycle = 0;
		int id = 0;
		
		int tmp_tr = 0;
		int tmp_tg = 0;
		int tmp_tb = 0;
		float tmp_wl = 0;
		int tmp_delay = 0;
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
					case 4:
						write(sock2, &r, sizeof(int));
						write(sock2, &g, sizeof(int));
						write(sock2, &b, sizeof(int));
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

int log_output(const char * str, FILE* log)
{
	int ret = strlen(str);
	fwrite(str, sizeof(char), ret, log);
	return ret;
}
