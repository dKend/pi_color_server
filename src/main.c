#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#define VERSION "0.0.1"


/*
		pcs-client option [args]
		client_command		command#		description
								0			unused
		-halt,					1			client requests that server be stopped.
		-h
		-setcolor,				2			set pins to given decimal values (0-255) syntax: <red> <green> <blue>
		-sc
								3			signal from forked task requesting the current rgb values.
		-getcolor,				4			output current color to console.
		-gc			
		-getred					5			prints the red value to the console and returns it as the exit code
		-getgreen				6			prints the green value to the console and returns it as the exit code
		-getblue				7			prints the blue value to the console and returns it as the exit code
								
		
*/
static int no_log = 0; //remove
static int multi_thread = 0; //remove
int main(int argc, const char* argv[])
{
	if(argc >= 2)
	{
		struct s_flags flags = {0, 0, 0, 0, 0};
		int c;
		while(1){
			int thisOptionOptind = optind ? optind : 1;
			int index = 0;
			static struct option longopts[] = {
				{"daemon", no_argument, 0, 'd'},
				{"no-log", no_argument, 0, 'l'},
				{"version", no_argument, 0, 'v'},
				{"multi-thread", no_argument, 0, 'm'},
				{"help", no_argument, 0, 'h'},
				{"do-tests", no_argument, 0, 't'},
				{0, 0, 0, 0}
			};
			c = getopt_long(argc, argv, NULL, longopts, &index);
			if(c == -1){
				break;
			}
			switch(c){
				case 'd':
					flags.daemon = 1;
					break;
				case 'l':
					flags.no_log = 1;
					break;
				case 'v':
					flags.version = 1;
					break;
				case 'm':
					flags.multi_thread = 1;
					break;
				case 't':
					break;
				case 'h':
				default:
					flags.help = 1;
					break;
			}
		}
		if(flags.help == 1){
			printf("print help text here.\n");
			exit(0);
		}
		if(flags.version == 1){
			printf("pcsd v%s\n", VERSION);
			exit(0);
		}
		if(flags.no_log == 1){
			no_log = 1;
		}
		if(flags.multi_thread == 1){
			multi_thread = 1;
		}
		if( flags.daemon == 1){
			int pi = pigpio_start(NULL, NULL);
			int pid = fork();
			if(pid < 0){
				exit(1);
			}
			else if(pid == 0){
				umask(0);
				int sid = setsid();
				
				if(sid<0){
					printf("error: setsid() failed.\n");
					fflush(stdout);
					exit(1);
				}
				
				//check if pcs directory exists and create it if it doesn't
				DIR* dir = opendir(DIR_PATH);
				if(dir)
					closedir(dir);
				else if(errno == ENOENT)
					mkdir(DIR_PATH, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH|S_IWOTH);
				else
					perror("unable to determine file existence.");
				
				chdir(DIR_PATH);
				
				if(access("colorserver", F_OK)!=-1){
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
		}
	}
	
	return 0;
}