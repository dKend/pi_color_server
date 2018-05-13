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
#include "PCSDefine.h"
/**
*	pcs [OPTIONS]
*	Options:
*	-l,		Enables logging 
*	-d,		Runs the process as a daemon.
*	-p,		Print the daemon pid at start. Only works if -d is also specified.
*	-s,		Saves the pid to a file at startup. Only works if -d is also
*			specified.
*/
typedef struct ServerFlags{
	int outputToLog;		//handled by passing flags to log_output function.
	int runAsDaemon;		//handled in main.
	int printPIDAtStartup;	//handled in runServerAsDaemon.
	int savePIDToFile;		//handled in runServerAsDaemon.
	int displayHelp;		//handled in main.
} ServerFlags;
void listen_loop(int pi, ServerFlags flags);
int process_request(int pi, int sock);
void server_tests();
void apply_brightness(int* n, int bright);
int log_output(const char * str, FILE* log, ServerFlags flag);
void printHelp();
void runServerAsDaemon(ServerFlags flags);
void runServerInForground(ServerFlags flags);
/**	
*	IGNORE THIS SECTION, Old reference but some info is still relevant.
*	pcs-client option [args]
*	client_command		command#		description
*							0			unused
*	-halt,					1			client requests that server be stopped.
*	-h
*	-setcolor,				2			set pins to given decimal values (0-255) syntax: <red> <green> <blue>
*	-sc
*							3			signal from forked task requesting the current rgb values.
*	-getcolor,				4			output current color to console.
*	-gc			
*	-getred					5			prints the red value to the console and returns it as the exit code
*	-getgreen				6			prints the green value to the console and returns it as the exit code
*	-getblue				7			prints the blue value to the console and returns it as the exit code
*							
*	
*/

int main(int argc, char **argv){
	ServerFlags flags = {0, 0, 0, 0};
	int c;
	
	while((c = getopt(argc, argv, "ldps")) != -1){
		switch(c){
			case 'l':
				flags.outputToLog = 1;
				break;
			case 'd':
				flags.runAsDaemon = 1;
				break;
			case 'p':
				flags.printPIDAtStartup = 1;
				break;
			case 's':
				flags.savePIDToFile = 1;
				break;
			case '?':
			case 'h':
			default:
				flags.displayHelp = 1;
				break;
		}
	}
	if(flags.displayHelp == 1){
		printHelp();
	}else if(flags.runAsDaemon == 1){
		runServerAsDaemon(flags);
	}else{
		int pi = pigpio_start(NULL, NULL);
		listen_loop(pi, flags);
	}
	
	return 0;
}
void runServerAsDaemon(ServerFlags flags){
	int pi = pigpio_start(NULL, NULL);
	int pid = fork();
	if(pid < 0){
		exit(1);
	}else if(pid == 0){
		umask(0);
		int sid = setsid();
		
		if(sid<0){
			printf("error: setsid() failed.\n");
			fflush(stdout);
			exit(1);
		}
		
		//check if pcs directory exists and create it if it doesnt
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
		listen_loop(pi, flags);
		exit(0);
	}else{
		printf("daemon main process ID: %d\n", pid);
		exit(0);
	}
}

int process_request(int pi, int sock){
	int command = 0;
	read(sock, &command, sizeof(int));
	return command;
}

void listen_loop(int pi, ServerFlags flags){
	DIR* dir = opendir(LOG_PATH);
	if(dir)
		closedir(dir);
	else if(errno == ENOENT)
		mkdir(LOG_PATH, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
	else
		perror("unable to determine file existence.");
	
	char buffer[BUFF_LEN];
	sprintf(buffer, "%s/%s", LOG_PATH, LOG_NAME);
	
	FILE* log = NULL;
	log = fopen(buffer, "w+");
	
	struct sockaddr servaddr = {AF_UNIX, "colorserver"};
	
	socklen_t servaddrlen = sizeof(struct sockaddr)+12;
	
	int sock1 = socket(AF_UNIX, SOCK_STREAM, 0);
	
	if(sock1 == -1){
		log_output("\nFailed to create socket, aborting...\n", log, flags);
		pigpio_stop(pi);
		fclose(log);
		exit(1);
	}
	log_output("\nSocket Created successfully with name/id \"colorserver\"\n", log, flags);
	bind(sock1, &servaddr, servaddrlen);
	
	if(listen(sock1, 0) != -1 ){
		chmod("colorserver", 511);
		log_output("Server now listening for connections...\n", log, flags);
		int var = 0;
		
		struct sigaction news, olds;
		void ctrlc(){
			log_output("\nHalting Color Server...\n", log, flags);
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
		if(access(SAV_NAME, R_OK|F_OK) !=-1 ){
			lastcolor = fopen(SAV_NAME, "r");
			lc_exists = 1;
		}
		int r = 0;
		int g = 0;
		int b = 0;
		int br = MAX;
		int colorchanged = 0;
		
		if(lc_exists == 1){
			fread(&r, sizeof(int), 1, lastcolor);
			fread(&g, sizeof(int), 1, lastcolor);
			fread(&b, sizeof(int), 1, lastcolor);
			fclose(lastcolor);
			lastcolor = NULL;
			colorchanged = 1;
		}
		
		
		
		while(var == 0){
			int sock2 = accept(sock1, NULL, NULL);
			if(sock2!=-1){
				log_output("Socket Connection Accepted!\n", log, flags);
				int resp_code = process_request(pi, sock2);
				char tmp[BUFF_LEN];
				sprintf(tmp,"%d response code.\n",resp_code);
				log_output(tmp , log, flags);
				
				switch(resp_code){
					case 0:
						break;
					case 1:
						close(sock2);
						ctrlc();
						break;
					
					case 2:
						if(colorchanged == 0){
							read(sock2, &r, sizeof(int));
							read(sock2, &g, sizeof(int));
							read(sock2, &b, sizeof(int));
							colorchanged = 1;
						}
						close(sock2);
						break;
					case 3:
						read(sock2, &br, sizeof(int));
						close(sock2);
						colorchanged = 1;
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
					case 8:
						write(sock2, &br, sizeof(int));
						close(sock2);
						break;
					default:
						close(sock2);
						break;
				}
				//printf("red = %d\tgreen = %d\tblue = %d\n", r, g, b);
				
			}
			
			if(colorchanged==1){
				int tmp_r = r;
				int tmp_g = g;
				int tmp_b = b;
				apply_brightness(&tmp_r, br);
				apply_brightness(&tmp_g, br);
				apply_brightness(&tmp_b, br);
				set_PWM_dutycycle(pi, RED_PIN, tmp_r);
				set_PWM_dutycycle(pi, GREEN_PIN, tmp_g);
				set_PWM_dutycycle(pi, BLUE_PIN, tmp_b);
				colorchanged = 0;
			}
		}
		lastcolor = fopen(SAV_NAME, "w+");
		fwrite(&r, sizeof(int), 1, lastcolor);
		fwrite(&g, sizeof(int), 1, lastcolor);
		fwrite(&b, sizeof(int), 1, lastcolor);
		fclose(lastcolor);
		lastcolor = NULL;
		
	}else{
		log_output("Listen failed, aborting...\n", log, flags);
		pigpio_stop(pi);
		fclose(log);
		exit(1);
		
	}
	if(log != NULL){
		fclose(log);
	}
	
}

int log_output(const char * str, FILE* log, ServerFlags flag){
	int ret = 0;
	if (log != NULL && flag.outputToLog == 1){
		ret = strlen(str);
		fwrite(str, sizeof(char), ret, log);
	}
	return ret;
}

void apply_brightness(int* n, int bright){
	if ( n != NULL  ){
		*n = ((*n) * bright)/MAX;
	}
}

void printHelp(){
	const char* helptext[] = {
		"Usage: pcs-server [OPTIONS]",
		"Options:",
		"  -l\t\t\tEnables logging",
		"  -d\t\t\tRuns the process as a daemon.",
		"  -p\t\t\tPrint the daemon pid at start. Only works if -d is also specified.",
		"  -s\t\t\tSaves the pid to a file at startup. Only works if -d is also",
		"  \t\t\tspecified."
	};
	int i;
	for(i=0;i<HELP_LEN;i++){
		printf("%s\n", helptext[i]);
	}
}
