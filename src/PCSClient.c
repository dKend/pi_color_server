#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <getopt.h>
#include "PCSDefine.h"
#include "lib/SimpleCTesting/assertion.h"
//typedef struct ClientFlags{
//	int getClientVersion_print;
//	int getClientVersion_asReturnValue;
//	int setColor;
//	int setBrightness;
//	int getColor_print;
//	int getColor_asReturnValue;
//	int getBrightness_print;
//	int getBrightness_asReturnValue;
//	int haltServer;
//}
int establish_connection();
int client_handle_input(int argc, char **argv);
int client_handle_input2(int argc, char **argv);
int printw(const char* str);
int test_extract_functions();
unsigned int extract_red(unsigned int color);
unsigned int extract_green(unsigned int color);
unsigned int extract_blue(unsigned int color);
unsigned int extract_bright(unsigned int color);
void printHelp();
int main(int argc, char **argv){
	chdir(DIR_PATH);
	return client_handle_input2(argc, argv);
}
int client_handle_input2(int argc, char **argv){
	int ret = -1;
	int c;
	int command;
	int sock = -2;
	int while_condition = 1;
	while(while_condition){
		static struct option long_options[] = {
			{"version",		no_argument,		0, 	'v'},
			{"version-ret", no_argument,		0, 	'V'},
			{"set-color",	required_argument,	0,	'c'},
			{"set-red",		required_argument,	0,	'r'},
			{"set-green",	required_argument,	0,	'g'},
			{"set-blue",	required_argument,	0,	'b'},
			{"set-bright",	required_argument,	0,	't'},
			{"get-color",	no_argument,		0,	'C'},
			{"get-red",		no_argument,		0,	'R'},
			{"get-green",	no_argument,		0,	'G'},
			{"get-blue",	no_argument,		0,	'B'},
			{"get-bright",	no_argument,		0,	'T'},
			{"kill",		no_argument,		0,	'k'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		c = getopt_long(argc, argv, "vVCRGBTkc:r:g:b:t:", long_options, 
		&option_index);
		
		if(c == -1)
			break;
		switch(c){
			case 0:
				break;
			case 'v':
			case 'V':
				while_condition = 0;
				printf("Pi Color Server v%d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, BUILD_NUM);
				ret = 0;
				break;
			case 'c':
				
				printf("--set-color option with argument %s specified.\n", optarg);
				command = 2;
				sock = establish_connection();
				unsigned int color = (unsigned int)strtol(optarg, NULL, 16);
				if(sock!=-1){
					unsigned int red = extract_red(color);
					unsigned int green = extract_green(color);
					unsigned int blue = extract_blue(color);
					printf("red: %d green: %d blue: %d", red, green, blue);
					write(sock, &command, sizeof(int));
					write(sock, &red, sizeof(int));
					write(sock, &green, sizeof(int));
					write(sock, &blue, sizeof(int));
					
					close(sock);
					ret = 0;
				}else{
					printf("Socket connection failed.\n");
					fflush(stdout);
				}
				break;
			/**
			*	TODO: implement commands to set indivdual color values
			*/
			case 'r':
				printf("--set-red option with argument %s specified.\n", optarg);
				ret = 0;
				break;
			case 'g':
				printf("--set-green option with argument %s specified.\n", optarg);
				ret = 0;
				break;
			case 'b':
				printf("--set-blue option with argument %s specified.\n", optarg);
				ret = 0;
				break;
			case 't':
				printf("--set-bright option with argument %s specified.\n", optarg);
				command = 3;
				int br = atoi(argv[2]);
				if(br > MAX)
					br=MAX;
				else if(br < 0)
					br=0;
				
				sock = establish_connection();
				if(sock != -1){
					write(sock, &command, sizeof(int));
					write(sock, &br, sizeof(int));
					close(sock);
				}
				ret = 0;
				break;
			case 'C':
				printf("--get-color option specified.\n");
				command = 4;
				sock = establish_connection();
				if(sock!=-1){
					int red;
					int green;
					int blue;
					write(sock, &command, sizeof(int));
					read(sock, &red, sizeof(int));
					read(sock, &green, sizeof(int));
					read(sock, &blue, sizeof(int));
					close(sock);
					int r = red << 16;
					int g = green << 8;
					ret = r|g|blue;
				}
				break;
			case 'R':
				printf("--get-red option specified.\n");
				command = 5;
				int sock = establish_connection();
				if(sock!=-1){
					int red;
					write(sock, &command, sizeof(int));
					read(sock, &red, sizeof(int));
					close(sock);
					ret = red;
				}
				//ret = 0;
				break;
			case 'G':
				printf("--get-green option specified.\n");
				command = 6;
				int sock = establish_connection();
				if(sock!=-1){
					int green;
					
					write(sock, &command, sizeof(int));
					read(sock, &green, sizeof(int));
					
					close(sock);
					
					printf("{green: %d}\n", green);
					
					ret = green;
					//printf("g: %d\n", green);
				}
				//ret = 0;
				break;
			case 'B':
				printf("--get-blue option specified.\n");
				command = 7;
				int sock = establish_connection();
				if(sock!=-1){
					int blue;
					
					write(sock, &command, sizeof(int));
					read(sock, &blue, sizeof(int));
					
					close(sock);
					
					printf("{blue: %d}\n", blue);
					
					ret = blue;
					//printf("r: %d\n", blue);
				}
				break;
			case 'T':
				printf("--get-bright option specified.\n");
				command = 8;
				int sock = establish_connection();
				if(sock != -1 ){
					int br;
					
					write(sock, &command, sizeof(int));
					read(sock, &br, sizeof(int));
					
					close(sock);
					
					printf("{brightness: %d}\n", br);
					
					ret = br;
				}
				break;
			case 'k':
				printf("--kill option specified.\n");
				ret = 0;
				break;
			case '?':
				break;
			default:
				abort();
		}
	}
	
	return ret;
}
int client_handle_input(int argc, char **argv){
	int ret = 0;
	int command = -1;
	if(argc >= 2){
		
		if(strcmp(argv[1], "-halt")==0 || strcmp(argv[1], "-h")==0 ){
			//	Don't need to connect if the PID file is present and valid.
			command = 1;
			int sock = establish_connection();
			if(sock !=-1){
				write(sock, &command, sizeof(int));
				close(sock);
			}
			//do things
		}else if(strcmp(argv[1], "-setcolor")==0 || strcmp(argv[1], "-sc")==0){
			command = 2;
			if(argc >= 5)
			{
				//arguments beyond the 5th are ignored by this command
				int sock = establish_connection();
				if(sock!=-1)
				{
					const int red = atoi(argv[2]);
					const int green = atoi(argv[3]);
					const int blue = atoi(argv[4]);
					
					write(sock, &command, sizeof(int));
					write(sock, &red, sizeof(int));
					write(sock, &green, sizeof(int));
					write(sock, &blue, sizeof(int));
					
					close(sock);
				}
			}
		}else if(strcmp(argv[1], "-getcolor") == 0 || strcmp(argv[1], "-gc") == 0){
			command = 4;
			int sock = establish_connection();
			if(sock!=-1){
				int red;
				int green;
				int blue;
				
				write(sock, &command, sizeof(int));
				read(sock, &red, sizeof(int));
				read(sock, &green, sizeof(int));
				read(sock, &blue, sizeof(int));
				
				close(sock);
				
				printf("{red: %d, green: %d, blue: %d}\n", red, green, blue);
				
				int r = red << 16;
				int g = green << 8;
				
				ret = r|g|blue;
				printf("red as bits 17-24: %d\ngreen as bits 9-16: %d\nblue as bits 1-8: %d\nentire color as single integer: %d\n", r, g, blue, ret);
			}
		}else if(strcmp(argv[1], "-getred") == 0){
			command = 5;
			int sock = establish_connection();
			if(sock!=-1){
				int red;
				
				write(sock, &command, sizeof(int));
				read(sock, &red, sizeof(int));
				
				close(sock);
				
				printf("{red: %d}\n", red);
				
				ret = red;
				//printf("r: %d\n", red);
			}
		}else if(strcmp(argv[1], "-getgreen") == 0){
			command = 6;
			int sock = establish_connection();
			if(sock!=-1){
				int green;
				
				write(sock, &command, sizeof(int));
				read(sock, &green, sizeof(int));
				
				close(sock);
				
				printf("{green: %d}\n", green);
				
				ret = green;
				//printf("g: %d\n", green);
			}
		}else if(strcmp(argv[1], "-getblue") == 0){
			command = 7;
			int sock = establish_connection();
			if(sock!=-1){
				int blue;
				
				write(sock, &command, sizeof(int));
				read(sock, &blue, sizeof(int));
				
				close(sock);
				
				printf("{blue: %d}\n", blue);
				
				ret = blue;
				//printf("r: %d\n", blue);
			}
		}else if(strcmp(argv[1], "-setbright") == 0){
			if( argc >= 3 ){
				command = 3;
				int br = atoi(argv[2]);
				if(br > MAX)
					br=MAX;
				else if(br < 0)
					br=0;
				
				int sock = establish_connection();
				if(sock != -1){
					write(sock, &command, sizeof(int));
					write(sock, &br, sizeof(int));
					close(sock);
				}
			}
			
		}
		else if(strcmp(argv[1], "-getbright") == 0){
			command = 8;
			int sock = establish_connection();
			if(sock != -1 ){
				int br;
				
				write(sock, &command, sizeof(int));
				read(sock, &br, sizeof(int));
				
				close(sock);
				
				printf("{brightness: %d}\n", br);
				
				ret = br;
			}
			
		}
		else
			printf("\n%s: invalid command\n", argv[1]);
	}
	return ret;
}

int establish_connection(){
	int sock;
	
	struct sockaddr name = {AF_UNIX, "colorserver"};
	
	socklen_t namelen = sizeof(struct sockaddr)+12;
	
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	
	if(sock != -1)
	{
		if(connect(sock, &name, namelen)==-1)
		{
			printf("error: connection to server failed. ERRNO:%d\n", errno);
			sock = -1;
		}
		
	}
	
	return sock;
}
unsigned int extract_red(unsigned int color){
	return color >> 16;
}
unsigned int extract_green(unsigned int color){
	unsigned int ret = color << 16;
	return ret >> 24;
}
unsigned int extract_blue(unsigned int color){
	unsigned int ret = color << 24;
	return ret >> 24;
}
unsigned int extract_bright(unsigned int color){
	unsigned int ret = color << 24;
	return ret >> 24;
}

int test_extract_functions(){
	int ret = -1;
	char* teststring[] = {
		"ff0000",
		"00ff00",
		"0000ff"
	};
	
	unsigned int color0 = (unsigned int)strtol(teststring[0], NULL, 16);
	unsigned int color1 = (unsigned int)strtol(teststring[1], NULL, 16);
	unsigned int color2 = (unsigned int)strtol(teststring[2], NULL, 16);
	
	assertEqualsInt(255, extract_red(color0), "1", &ret);
	assertEqualsInt(0, extract_red(color1), "2", &ret);
	assertEqualsInt(0, extract_red(color2), "3", &ret);
	
	assertEqualsInt(0, extract_green(color0), "4", &ret);
	assertEqualsInt(255, extract_green(color1), "5", &ret);
	assertEqualsInt(0, extract_green(color2), "6", &ret);
	
	assertEqualsInt(0, extract_blue(color0), "7", &ret);
	assertEqualsInt(0, extract_blue(color1), "8", &ret);
	assertEqualsInt(255, extract_blue(color2), "9", &ret);
	return ret;
}

void printHelp(){
	const char* helpText[] = {
		"Usage: pcs [OPTIONS]",
		"Options:"
	};
}