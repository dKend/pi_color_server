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
#include <pthread.h>
#include <dirent.h>
#include <errno.h>
#include <semaphore.h>
#include "color.h"
#include "threadArgs.h"

#define RED_PIN 17
#define GREEN_PIN 22
#define BLUE_PIN 24
#define MAX 255

#define LOG_PATH "/var/log/pcs-server"
#define LOG_NAME "pcs-server.log"
#define DIR_PATH "/tmp/pcs"
#define BUFF_LEN 1024
#define SAV_NAME "pcs-server.color"

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
				
				//check if pcs directory exists and create it if it doesnt
				DIR* dir = opendir(DIR_PATH);
				if(dir)
					closedir(dir);
				else if(errno == ENOENT)
				{
					mkdir(DIR_PATH, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH|S_IWOTH);
				}else
					perror("unable to determine file existence.");
				
				chdir(DIR_PATH);
				
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
	
	//create path for log file and place it in a 1024 byte buffer
	char buffer[BUFF_LEN];
	sprintf(buffer, "%s/%s", LOG_PATH, LOG_NAME);
	//init log file
	log = fopen(buffer, "w+");
	int lockInitFailed = 0;
	
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
	
	if(lockInitFailed == 1){
		pigpio_stop(pi);
		fclose(log);
		exit(1);
	}
	
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
		if(access(SAV_NAME, R_OK|F_OK) !=-1 )
		{
			lastcolor = fopen(SAV_NAME, "r");
			lc_exists = 1;
		}
		if(lc_exists == 1)
		{
			int r, g, b;
			fread(&r, sizeof(int), 1, lastcolor);
			fread(&g, sizeof(int), 1, lastcolor);
			fread(&b, sizeof(int), 1, lastcolor);
			setColor(r, g, b, MAX);
			colorChangedActions(pi);
			fclose(lastcolor);
			lastcolor = NULL;
		}
		while(var == 0)
		{
			int sock2 = accept(sock1, NULL, NULL);
			threadArgs* sockptr;
			*sockptr = NULL;
			initThreadArgs(&sockptr, pi, sock2);
			
			if(sock2!=-1)
			{
				log_output("Socket Connection Accepted!\n", log);
				int resp_code = process_request(pi, sock2);
				char tmp[BUFF_LEN];
				sprintf(tmp,"%d response code.\n",resp_code);
				log_output(tmp, log);
				unsigned int r, g, b, br;
				r = getRed();
				g = getGreen();
				b = getBlue();
				br = getBrightness();
				switch(resp_code)
				{
					case 1:
						//halt
						close(sock2);
						ctrlc();
						break;
					
					case 2:
						//setcolor
						read(sock2, &r, sizeof(int));
						read(sock2, &g, sizeof(int));
						read(sock2, &b, sizeof(int));
						close(sock2);
						setColor(r, g, b, br);
						setColorChanged();
						break;
					case 3:
						//setbrightness
						read(sock2, &br, sizeof(int));
						close(sock2);
						setBrightness(br);
						setColorChanged();
						break;
					case 4:
						//getcolor
						write(sock2, &r, sizeof(int));
						write(sock2, &g, sizeof(int));
						write(sock2, &b, sizeof(int));
						close(sock2);
						break;
					case 5:
						//getred
						write(sock2, &r, sizeof(int));
						close(sock2);
						break;
					case 6:
						//getgreen
						write(sock2, &g, sizeof(int));
						close(sock2);
						break;
					case 7:
						//getblue
						write(sock2, &b, sizeof(int));
						close(sock2);
						break;
					case 8:
						//getbrightness
						write(sock2, &br, sizeof(int));
						close(sock2);
						break;
					default:
						//error
						close(sock2);
						break;
				}
			}
			
			if(getColorChanged()==1)
			{
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

int log_output(const char * str, FILE* f)
{
	int ret = 0;
	if (f != NULL)
	{
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
