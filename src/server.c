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
	int ret = 0;
	int command = 0;
	unsigned int r;
	unsigned int g;
	unsigned int b;
	read(sock, &command, sizeof(int));
	read(sock, &r, sizeof(int));
	read(sock, &g, sizeof(int));
	read(sock, &b, sizeof(int));
	if(command == 1)
	{
		ret = 1;
	}
	else if(command == 2)
	{
		
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
	return ret;
}

void fork_child_task(int pi)
{
	//init values of color cycle
	int r = 50;
	int g = 0;
	int b = 10;
	int desc = 1;
	int step = 1;
	const struct timespec delay = {0, 50000000};
	struct timespec catch;
	while(nanosleep(&delay, &catch) != -1)
	{
		//changes to color values below
		if(desc==1)
		{
			g+=step;
		}else if(desc==0)
		{
			g-=step;
		}
		
		
		if(g >= 255)
		{
			g = 255;
			desc = 0;
		}else if(g <= 0)
		{
			g = 0;
			desc = 1;
		}
		
		set_PWM_dutycycle(pi, RED_PIN, r);
		set_PWM_dutycycle(pi, GREEN_PIN, g);
		set_PWM_dutycycle(pi, BLUE_PIN, b);
	}
	exit(1);
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
	printf("\nSocket Creadted successfully with name/id \"colorserver\"\n");
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
	printf("\nSocket Creadted successfully with name/id \"colorserver\"\n");
	bind(sock1, &servaddr, servaddrlen);
	
	if( listen(sock1, 0) != -1 )
	{
		pid_t fd = fork();
		
		if(fd == 0)
		{
			fork_child_task(pi);
			exit(1);
		}else if(fd>0)
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
				kill(fd, SIGKILL);
				set_PWM_dutycycle(pi, RED_PIN, 0);
				set_PWM_dutycycle(pi, GREEN_PIN, 0);
				set_PWM_dutycycle(pi, BLUE_PIN, 0);
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
					
				}
			}
		}else
		{
			printf("Fork failed, aborting...\n");
			exit(1);
		}
		
	}else
	{
		printf("Listen failed, aborting...\n");
		pigpio_stop(pi);
		exit(1);
		
	}
	pigpio_stop(pi);
	
}
