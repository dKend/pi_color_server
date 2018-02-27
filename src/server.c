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
#define MAX_THREADS 1024

void* cycle(void* args){
	//thread color cycle function
	//unfinished
	colorPair* tmp = (colorPair*)args;
	//setThreadRunning();
	struct node* head;
	struct node* tail;
	colorlist_init(&head, &tail);
	generate_sin_cycle_list(&head, &tail, (float)10.0, &tmp, 50000000);
	return NULL;
}
int createServer(Server** self){
	int ret = -1;
	
	return -1;
}
int freeServer(Server** self){
	int ret = -1;
	
	return -1;
}
int initServer(Server* self){
	int ret = -1;
	int logInit = 0;
	int mutInit = 0;
	if(self != NULL){
		//log initialization
		DIR* dir = opendir(LOG_PATH);
		int canCreateFile = 0;
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
			self->log = fopen(buffer, "w+");
			if(self->log != NULL){
				logInit = 1;
			}
		}
		
		//Mutex initialization
		if(self->log != NULL){
			//initialise locks
			if(sem_init(&(self->sLLock, 0, 1) == -1){
				log_output("ERROR: Failed to initialise semaphore \"sLLock\"", log);
			}else{
				mutInit = 1
			}
			
		}else{
			printf("CRITICAL ERROR: Log was not successfully initialized before attempting to initialise semaphores!\n");
		}
		
		if(mutInit == 1 && logInit == 1){
			ret = 0;
		}
	}
	return ret;
}
int runServer(Server* self){
	int ret = -1;
	
	return -1;
}
int getColorChanged(Server* self){
	int ret = -1;
	sem_wait(&(self->colorchangedlock));
		ret = self->sColorChanged;
	sem_post(&(self->colorchangedlock));
	return ret;
}
void setColorChanged(Server* self){
	sem_wait(&(self->colorchangedlock));
		self->sColorChanged = 1;
	sem_post(&(self->colorchangedlock));
}
void unsetColorChanged(Server* self){
	sem_wait(&(self->colorchangedlock));
		self->sColorChanged = 0;
	sem_post(&(self->colorchangedlock));
}
void setColor(Server* self, unsigned int r, unsigned int g, unsigned int b, unsigned int br){
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
void colorChangedActions(Server* self){
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
//doesnt return an int yet
int listenLoop(Server* self){
	/*
	*	Check that the Server has initialized to the best of our ability
	*/
	
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
			setVar();
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
				}else if(multi_thread == 1 && tlist->nodeCount < MAX_THREADS){
					int tid;
					*sockptr = NULL;
					initThreadArgs(&sockptr, pi, sock2);
					pthread_create(&tid, NULL, threadHandleConnection, sockptr);
					addThreadList(tlist, tid);
				}
				
			}
			waitForThreadsThreadList(tlist);
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
		
	}else{
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
int processRequest(Server* self){
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
			pid_t pid = getpid();
			kill(pid, SIGINT);
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
int log_output(const char * str, FILE* f){
	int ret = 0;
	if (f != NULL){
		ret = strlen(str);
		fwrite(str, sizeof(char), ret, f);
	}
	return ret;
}
void apply_brightness(int* n, int bright){
	if ( n != NULL  ){
		*n = ((*n) * bright)/MAX;
	}
}
int setListen(Server* self){
	sem_wait(&varlock);
		var = 1;
	sem_post(&varlock);
}
int unsetListen(Server* self){
	int ret = -1;
	
	return ret;
}
void* threadHandleConnection(void* args){
	threadArgs* targs = (threadArgs*) args;
	int sock = targs->sock;
	int pi = targs->pi;
	process_request(pi, sock);
	return NULL;
}
