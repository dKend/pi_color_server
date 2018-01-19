#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "threadArgs.h"
#include "assertion.h"

void initThreadArgs(threadArgs** self, int pi, int sock){
	if(self != NULL){
		*self = (threadArgs*)malloc(sizeof(threadArgs));
		(*self)->pi = pi;
		(*self)->sock = sock;
	}
}
void freeThreadArgs(threadArgs** self){
	if(self != NULL){
		if(*self != NULL){
			if((*self)->sock!=-1){
				closeSock(self);
			}
			free(*self);
			*self = NULL;
		}
	}
}
int getPi(struct threadArgs* self){
	return self->pi;
}
int getSock(struct threadArgs* self){
	return self->sock
}
void closeSock(struct threadArgs** self){
	if(self != NULL){
		if(*self != NULL){
			if((*self)->sock!=-1){
				close((*self)->sock);
				(*self)->sock = -1;
			}
		}
	}
	
}