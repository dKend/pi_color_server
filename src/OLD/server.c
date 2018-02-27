#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <pigpiod_if2.h>
#include <sys/stat.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>
#include <semaphore.h>
#include "color.h"
#include "threadArgs.h"
#include "threadList.h"

#define RED_PIN 17
#define GREEN_PIN 22
#define BLUE_PIN 24
#define MAX 255

#define LOG_PATH "/var/log/pcs-server"
#define LOG_NAME "pcs-server.log"
#define DIR_PATH "/tmp/pcs"
#define BUFF_LEN 1024
#define SAV_NAME "pcs-server.color"
#define VERSION "0.0.1"
#define MAX_THREADS 1024

typedef struct s_flags {
	int daemon;
	int no_log;
	int version;
	int multi_thread;
	int help;
};

static unsigned int red = 0;
static unsigned int green = 0;
static unsigned int blue = 0;
static unsigned int brightness = MAX;
static int colorchanged = 0;
static int threadRunning = 0;
static int suspendThreadForNotif = 0;
static sem_t redlock;
static sem_t greenlock;
static sem_t bluelock;
static sem_t brightlock;
static sem_t colorchangedlock;
static sem_t threadrunninglock;
static sem_t colorlock;
static sem_t suspendthreadfornotiflock;
static FILE* log = NULL;
static int no_log = 0;
static int multi_thread = 0;

void* cycle(void* args);
unsigned int getRed();
void setRed(unsigned int value);
unsigned int getGreen();
void setGreen(unsigned int value);
unsigned int getBlue();
void setBlue(unsigned int value);
unsigned int getBrightness();
void setBrightness(unsigned int value);
int getColorChanged();
void setColorChanged();
void unsetColorChanged();
int getThreadRunning();
void setThreadRunning();
void unsetThreadRunning();
void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int br);
void colorChangedActions(int pi);
void listen_loop(int pi);
int process_request(int pi, int sock);
void server_tests();
void apply_brightness(int* n, int bright);
int getSuspendThreadForNotif();	//returns state of suspend flag
int setSuspendThreadForNotif();	//set state of suspend to 1
int unsetSuspendThreadForNotif();	//set state of suspend to 0 (default value)
int log_output(const char * str, FILE* f);
void* threadHandleConnection(void* args);
void initLog();
int initMutex();
int initAll();
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

int process_request(int pi, int sock){
	int command = 0;
	read(sock, &command, sizeof(int));
	char tmp[BUFF_LEN];
	sprintf(tmp,"%d response code.\n",command);
	log_output(tmp, log);
	unsigned int r, g, b, br;
	struct color  c = {getRed(), getBlue(), getGreen(), getBrightness()};
	switch(command){
		case 1:
			//halt
			close(sock);
			ctrlc();
			break;
		
		case 2:
			//setcolor
			read(sock, &c, sizeof(struct color));
			close(sock);
			setColor(c.r, c.g, c.b, c.br);
			setColorChanged();
			break;
		case 3:
			//setbrightness
			read(sock, &br, sizeof(int));
			close(sock);
			setBrightness(br);
			setColorChanged();
			break;
		case 4:
			//getcolor
			write(sock, &c, sizeof(struct color));
			close(sock);
			break;
		case 5:
			//getred
			write(sock, &(c.red), sizeof(int));
			close(sock);
			break;
		case 6:
			//getgreen
			write(sock, &(c.green), sizeof(int));
			close(sock);
			break;
		case 7:
			//getblue
			write(sock, &(c.blue), sizeof(int));
			close(sock);
			break;
		case 8:
			//getbrightness
			write(sock, &(c.brightness), sizeof(int));
			close(sock);
			break;
		case 9:
			//getdelay
			write(sock, &(c.delay), sizeof(int));
			close(sock);
			break;
		default:
			//error
			close(sock);
			break;
	}
	
	return command;
}

void listen_loop(int pi){
	initAll();
	
	struct sockaddr servaddr = {AF_UNIX, "colorserver"};
	socklen_t servaddrlen = sizeof(struct sockaddr)+12;
	int sock1 = socket(AF_UNIX, SOCK_STREAM, 0);
	
	if(sock1 == -1){
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
		void ctrlc(){
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
		if(access(SAV_NAME, R_OK|F_OK) !=-1 ){
			lastcolor = fopen(SAV_NAME, "r");
			lc_exists = 1;
		}
		if(lc_exists == 1){
			int r, g, b;
			fread(&r, sizeof(int), 1, lastcolor);
			fread(&g, sizeof(int), 1, lastcolor);
			fread(&b, sizeof(int), 1, lastcolor);
			setColor(r, g, b, MAX);
			colorChangedActions(pi);
			fclose(lastcolor);
			lastcolor = NULL;
		}
		threadList* tlist;
		initThreadList(&tlist);
		while(var == 0){
			
			int sock2 = accept(sock1, NULL, NULL);
			threadArgs* sockptr;
			
			if(sock2!=-1){
				log_output("Socket Connection Accepted!\n", log);
				if(multi_thread == 0){
					process_request(pi, sock2);
				}else if(multi_thread == 1 && threadCount < MAX_THREADS){
					*sockptr = NULL;
					initThreadArgs(&sockptr, pi, sock2);
					pthread_create(&tids[threadCount], NULL, threadHandleConnection, sockptr);
					threadCount++;
				}
				
			}
			if(threadCount > 0){
				//attempt to join threads
			}
			if(getColorChanged()==1){
				colorChangedActions(pi);
			}
		}
		
		lastcolor = fopen(SAV_NAME, "w+");
		unsigned int r, g, b;
		r = getRed();
		g = getGreen();
		b = getBlue();
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

int log_output(const char * str, FILE* f){
	int ret = 0;
	if (f != NULL){
		ret = strlen(str);
		fwrite(str, sizeof(char), ret, f);
	}
	return ret;
}

void apply_brightness(int* n, int bright)
{
	if ( n != NULL  ){
		*n = ((*n) * bright)/MAX;
	}
}
int getRed(){
	int ret = -1;
	sem_wait(&redlock);
		ret = red;
	sem_post(&redlock);
	return ret;
}
void setRed(unsigned int value){
	sem_wait(&redlock);
		if(value >= 0 && value <= 255 ){
			red = value;
		}
	sem_post(&redlock);
}
int getGreen(){
	int ret = -1;
	sem_wait(&greenlock);
		ret = green;
	sem_post(&greenlock);
	return ret;
}
void setGreen(unsigned int value){
	sem_wait(&greenlock);
		if(value >= 0 && value <= 255 ){
			green = value;
		}
	sem_post(&greenlock);
}
int getBlue(){
	int ret = -1;
	sem_wait(&bluelock);
		ret = blue;
	sem_post(&bluelock);
	return ret;
}
void setBlue(unsigned int value){
	sem_wait(&bluelock);
		if(value >= 0 && value <= 255 ){
			blue = value;
		}
	sem_post(&bluelock);
}
int getBrightness(){
	int ret = -1;
	sem_wait(&brightlock);
		ret = brightness;
	sem_post(&brightlock);
	return ret;
}
void setBrightness(unsigned int value){
	sem_wait(&brightlock);
		if(value >= 0 && value <= 255 ){
			brightness = value;
		}
	sem_post(&brightlock);
}
int getColorChanged(){
	int ret = -1;
	sem_wait(&colorchangedlock);
		ret = colorchanged;
	sem_post(&colorchangedlock);
	return ret;
}
void setColorChanged(){
	sem_wait(&colorchangedlock);
		colorchanged = 1;
	sem_post(&colorchangedlock);
}
void unsetColorChanged(){
	sem_wait(&colorchangedlock);
		colorchanged = 0;
	sem_post(&colorchangedlock);
}
int getThreadRunning(){
	int ret = -1;
	sem_wait(&threadrunninglock);
		ret = threadRunning;
	sem_post(&threadrunninglock);
	return ret;
}
void setThreadRunning(){
	sem_wait(&threadrunninglock);
		threadRunning = 1;
	sem_post(&threadrunninglock);
}
void unsetThreadRunning(){
	sem_wait(&threadrunninglock);
		threadRunning = 0;
	sem_post(&threadrunninglock);
}
void setColor(unsigned int r, unsigned int g, unsigned int b, unsigned int br){
	sem_wait(&colorlock);
	sem_wait(&redlock);
	sem_wait(&greenlock);
	sem_wait(&bluelock);
	sem_wait(&brightlock);
		if(r >= 0 && r <= 255)
			red = r;
		if(g >= 0 && g <= 255)
			green = g;
		if(b >= 0 && b <= 255)
			blue = b;
		if(br >= 0 && br <= 255)
			brightness = br;
	sem_post(&brightlock);
	sem_post(&bluelock);
	sem_post(&greenlock);
	sem_post(&redlock);
	sem_post(&colorlock);
}
void colorChangedActions(int pi){
	int br = getBrightness();
	int tmp_r = getRed();
	int tmp_g = getGreen();
	int tmp_b = getBlue();
	apply_brightness(&tmp_r, br);
	apply_brightness(&tmp_g, br);
	apply_brightness(&tmp_b, br);
	set_PWM_dutycycle(pi, RED_PIN, tmp_r);
	set_PWM_dutycycle(pi, GREEN_PIN, tmp_g);
	set_PWM_dutycycle(pi, BLUE_PIN, tmp_b);
	unsetColorChanged();
}
void* cycle(void* args){
	//thread color cycle function
	colorPair* tmp = (colorPair*)args;
	setThreadRunning();
	struct node* head;
	struct node* tail;
	colorlist_init(&head, &tail);
	generate_sin_cycle_list(&head, &tail, (float)10.0, &tmp, 50000000);
	return NULL;
}
int getSuspendThreadForNotif(){
	int ret = -1;
	sem_wait(&suspendthreadfornotiflock);
		ret = suspendThreadForNotif;
	sem_post(&suspendthreadfornotiflock);
	return ret;
}
int setSuspendThreadForNotif(){
	sem_wait(&suspendthreadfornotiflock);
		suspendThreadForNotif = 1;
	sem_post(&suspendthreadfornotiflock);
}
int unsetSuspendThreadForNotif(){
	sem_wait(&suspendthreadfornotiflock);
		suspendThreadForNotif = 0;
	sem_post(&suspendthreadfornotiflock);
}
void* threadHandleConnection(void* args){
	threadArgs* targs = (threadArgs*) args;
	int sock = targs->sock;
	int pi = targs->pi;
	
	process_request(pi, sock);
	return NULL;
}
int initLog(){
	int ret = -1;
	int canCreateFile = 0;
	DIR* dir = opendir(LOG_PATH);
	if(dir){
		closedir(dir);
		canCreateFile = 1;
	}else if(errno == ENOENT){
		mkdir(LOG_PATH, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
		canCreateFile = 1;
	}else{
		perror("unable to determine file existence.");
	}
	if(canCreateFile == 1){
		//create path for log file and place it in a 1024 byte buffer
		char buffer[BUFF_LEN];
		sprintf(buffer, "%s/%s", LOG_PATH, LOG_NAME);
		//init log file
		log = fopen(buffer, "w+");
		if(log != NULL){
			ret = 0;
		}
	}
	
	return ret;
}
int initMutex(){
	int ret = -1;
	int lockInitFailed = 0;
	if(log != NULL){
		//initialise locks
		if(sem_init(&redlock, 0, 1) == -1){
			log_output("ERROR: Failed to initialise semaphore \"redlock\"", log);
			lockInitFailed = 1;
		}
		if(sem_init(&greenlock, 0, 1) == -1){
			log_output("ERROR: Failed to initialise semaphore \"greenlock\"", log);
			lockInitFailed = 1;
		}
		if(sem_init(&bluelock, 0, 1) == -1){
			log_output("ERROR: Failed to initialise semaphore \"bluelock\"", log);
			lockInitFailed = 1;
		}
		if(sem_init(&brightlock, 0, 1) == -1){
			log_output("ERROR: Failed to initialise semaphore \"brightlock\"", log);
			lockInitFailed = 1;
		}
		if(sem_init(&colorchangedlock, 0, 1) == -1){
			log_output("ERROR: Failed to initialise semaphore \"colorchangedlock\"", log);
			lockInitFailed = 1;
		}
		if(sem_init(&threadrunninglock, 0, 1) == -1){
			log_output("ERROR: Failed to initialise semaphore \"threadrunninglock\"", log);
			lockInitFailed = 1;
		}
		if(sem_init(&colorlock, 0, 1) == -1){
			log_output("ERROR: Failed to initialise semaphore \"colorlock\"", log);
			lockInitFailed = 1;
		}
		if(sem_init(&suspendthreadfornotiflock, 0, 1) == -1){
			log_output("ERROR: Failed to initialise semaphore \"suspendthreadfornotiflock\"", log);
			lockInitFailed = 1;
		}
	}else{
		printf("CRITICAL ERROR: Log was not successfully initialized before attempting to initialise semaphores!\n");
		lockInitFailed = 1;
	}
	
	if(lockInitFailed == 0){
		ret = 0;
	}
	
	return ret;
}
void initAll(){
	int ret = -1;
	if(initLog()==0){
		if(initMutex()==0){
			ret = 0;
		}else{
			log_output("ERROR: Mutex initialization failed.");
		}
	}else{
		printf("ERROR: Log initialization failed.");
		fflush(stdout);
	}
	
	if(ret == -1){
		if(log != NULL){
			log_output("CRITICAL ERROR: Initialization failed, halting process...");
			fclose(log);
		}
		exit(1);
	}
}
