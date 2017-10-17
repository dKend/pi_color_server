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

#include <dirent.h>
#include <errno.h>

#define RED_PIN 17
#define GREEN_PIN 22
#define BLUE_PIN 24

#define LOG_PATH "/var/log/pcs-server"
#define LOG_NAME "pcs-server.log"
#define BUFF_LEN 1024
#define SAV_NAME "pcs-server.color"


void listen_loop(int pi);
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
				listen_loop(pi);
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
		printf("Unable to start the server, run with -d command to start the daemon.\n");
		fflush(stdout);
	}
	
	return 0;
}

int process_request(int pi, int sock)
{
	
	int command = 0;
	read(sock, &command, sizeof(int));
	return command;
}

void listen_loop(int pi)
{
	
	DIR* dir = opendir(LOG_PATH);
	if(dir)
		closedir(dir);
	else if(errno == ENOENT)
	{
		mkdir(LOG_PATH, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
	}else
		perror("unable to determine file existence.");
	
	
	char buffer[BUFF_LEN];
	sprintf(buffer, "%s/%s", LOG_PATH, LOG_NAME);
	
	FILE* log = NULL;
	log = fopen(buffer, "w+");
	//int status;
	
	struct sockaddr servaddr = {AF_UNIX, "colorserver"};
	
	socklen_t servaddrlen = sizeof(struct sockaddr)+12;
	
	int sock1 = socket(AF_UNIX, SOCK_STREAM, 0);
	
	if(sock1 == -1)
	{
		log_output("\nFailed to create socket, aborting...\n", log);
		pigpio_stop(pi);
		fclose(log);
		exit(1);
	}
	log_output("\nSocket Created successfully with name/id \"colorserver\"\n", log);
	bind(sock1, &servaddr, servaddrlen);
	
	if( listen(sock1, 0) != -1 )
	{
		chmod("colorserver", 511);
		log_output("Server now listening for connections...\n", log);
		int var = 0;
		struct sigaction news, olds;
		void ctrlc()
		{
			log_output("\nHalting Color Server...\n", log);
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
		FILE* lastcolor = NULL;
		int lc_exists = 0;
		if(access(SAV_NAME, R_OK|F_OK) !=-1 )
		{
			lastcolor = fopen(SAV_NAME, "r");
			lc_exists = 1;
		}
		int r = 0;
		int g = 0;
		int b = 0;
		
		if(lc_exists == 1)
		{
			fread(&r, sizeof(int), 1, lastcolor);
			fread(&g, sizeof(int), 1, lastcolor);
			fread(&b, sizeof(int), 1, lastcolor);
			fclose(lastcolor);
			lastcolor = NULL;
		}
		
		int colorchanged = 0;
		
		while(var == 0)
		{
			int sock2 = accept(sock1, NULL, NULL);
			if(sock2!=-1)
			{
				log_output("Socket Connection Accepted!\n", log);
				int resp_code = process_request(pi, sock2);
				char tmp[BUFF_LEN];
				sprintf(tmp,"%d response code.\n",resp_code);
				log_output(tmp , log);
				
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
					case 5:
						write(sock2, &r, sizeof(int));
						close(sock2);
						break;
					case 6:
						write(sock2, &g, sizeof(int));
						close(sock2);
						break;
					case 7:
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
		lastcolor = fopen(SAV_NAME, "w+");
		fwrite(&r, sizeof(int), 1, lastcolor);
		fwrite(&g, sizeof(int), 1, lastcolor);
		fwrite(&b, sizeof(int), 1, lastcolor);
		fclose(lastcolor);
		lastcolor = NULL;
		
	}else
	{
		log_output("Listen failed, aborting...\n", log);
		pigpio_stop(pi);
		fclose(log);
		exit(1);
		
	}
	if(log != NULL)
	{
		fclose(log);
	}
	
}

int log_output(const char * str, FILE* log)
{
	int ret = 0;
	if (log != NULL)
	{
		ret = strlen(str);
		fwrite(str, sizeof(char), ret, log);
	}
	return ret;
}
