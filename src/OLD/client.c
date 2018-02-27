#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include "client.h"
#define DIR_PATH "/tmp/pcs"
#define MAX 255
/*
	COMMANDS:
	0	none (unused)
	1	halt
			-kills the server
	2	stclr <int red> <int green> <int blue>
			-sets color pins to the red green and blue values
			*all ints in args are between 0 and 255*
		
	
*/
int main(int argc, const char* argv[])
{
	chdir(DIR_PATH); 
	int ret = client_handle_input(argc, argv);
	printf("ret: %d\n", ret);
	return ret;
}


int client_handle_input(int argc, const char* argv[])
{
	int ret = 0;
	int command = -1;
	if(argc >= 2)
	{		
		
		if(strcmp(argv[1], "-halt")==0 || strcmp(argv[1], "-h")==0 )
		{
			command = 1;
			int sock = establish_connection();
			if(sock !=-1)
			{
				write(sock, &command, sizeof(int));
				close(sock);
			}
			//do things
		}
		else if(strcmp(argv[1], "-setcolor")==0 || strcmp(argv[1], "-sc")==0)
		{
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
		}
		else if(strcmp(argv[1], "-getcolor") == 0 || strcmp(argv[1], "-gc") == 0)
		{
			command = 4;
			int sock = establish_connection();
			if(sock!=-1)
			{
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
		}
		else if(strcmp(argv[1], "-getred") == 0)
		{
			command = 5;
			int sock = establish_connection();
			if(sock!=-1)
			{
				int red;
				
				write(sock, &command, sizeof(int));
				read(sock, &red, sizeof(int));
				
				close(sock);
				
				printf("{red: %d}\n", red);
				
				ret = red;
				//printf("r: %d\n", red);
			}
		}
		else if(strcmp(argv[1], "-getgreen") == 0)
		{
			command = 6;
			int sock = establish_connection();
			if(sock!=-1)
			{
				int green;
				
				write(sock, &command, sizeof(int));
				read(sock, &green, sizeof(int));
				
				close(sock);
				
				printf("{green: %d}\n", green);
				
				ret = green;
				//printf("g: %d\n", green);
			}
		}
		else if(strcmp(argv[1], "-getblue") == 0)
		{
			command = 7;
			int sock = establish_connection();
			if(sock!=-1)
			{
				int blue;
				
				write(sock, &command, sizeof(int));
				read(sock, &blue, sizeof(int));
				
				close(sock);
				
				printf("{blue: %d}\n", blue);
				
				ret = blue;
				//printf("r: %d\n", blue);
			}
		}
		else if(strcmp(argv[1], "-setbright") == 0){
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
		{
			printf("\n%s: invalid command\n", argv[1]);
		}
	}
	return ret;
}

int establish_connection()
{
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