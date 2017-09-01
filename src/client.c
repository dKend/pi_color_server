#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "client.h"

/*
	COMMANDS:
	0	none (unused)
	1	halt
			-kills the server
	2	stclr <int red> <int green> <int blue>
			-sets color pins to the red green and blue values
			*all ints in args are between 0 and 255*
		
	
*/



int client_handle_input(int argc, const char* argv[])
{
	chdir("/");
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
		else if(strcmp(argv[1], "-stclr")==0 || strcmp(argv[1], "-sc")==0)
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
		else if(strcmp(argv[1], "-getcolor") == 0)
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
			}
		}else if(strcmp(argv[1], "-st")==0)
		{
			if(argc >= 4)
			{
				command = 6;
				int sock = establish_connection();
				if(sock!=-1)
				{
					int num = (int) *(argv[2]);
					int val = atoi(argv[3]);
					write(sock, &command, sizeof(int));
					write(sock, &num, sizeof(int));
					write(sock, &val, sizeof(int));
					close(sock);
				}	
			}
			
		}else if(strcmp(argv[1], "-tar")==0)
		{
			if(argc >= 5)
			{
				command = 8;
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
		}else if(strcmp(argv[1], "-scyc")==0)
		{
			command = 9;
			int sock = establish_connection();
			if(sock!=-1)
			{
				write(sock, &command, sizeof(int));
				close(sock);
			}
		}else if(strcmp(argv[1], "-new")==0)
		{
			if(argc >= 7)
			{
				command = 11;
				int sock = establish_connection();
				if(sock != -1)
				{
					
					const int tr = atoi(argv[2]);
					const int tg = atoi(argv[3]);
					const int tb = atoi(argv[4]);
					const float wl = atof(argv[5]);
					const int delay = atoi(argv[6]);
					
					write(sock, &command, sizeof(int));
					write(sock, &tr, sizeof(int));
					write(sock, &tg, sizeof(int));
					write(sock, &tb, sizeof(int));
					write(sock, &wl, sizeof(float));
					write(sock, &delay, sizeof(int));
					close(sock);
					
				}
			}
		}
		else
		{
			printf("\n%s: invalid command\n", argv[1]);
		}
	}
	return command;
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
			printf("\nerror: connection to server failed.");
			sock = -1;
		}
		
	}
	
	return sock;
}