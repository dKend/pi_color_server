#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "threadList.h"


int initThreadList(struct threadList** self){
	int ret = -1;
	if(self != NULL){
		*self = (threadList*) malloc(sizeof(struct threadList));
		//temporary init function so that we can assure that the threadList we are working with is initialized as we expect it to be without the initThreadList function being called explicitly
		(*self)->head = NULL;
		(*self)->tail = NULL;
		(*self)->nodeCount = 0;
		ret = 0;
	}
	return ret;
}
int freeThreadList(struct threadList** self){
	int ret = -1;
	void freedomHelper(threadListNode** node){
		if(*node != NULL){
			freedomHelper(&((*node)->next));
			free(*node);
			*node = NULL;
		}
	}
	if(self != NULL){
		if(*self != NULL){
			freedomHelper(&((*self)->head));
			free(*self);
			*self = NULL;
			ret = 0;
		}
	}
	
	return ret;
}
int addThreadList(struct threadList* self, pthread_t data){
	int ret = -1;
	if(self != NULL){
		threadListNode* tmp = (threadListNode*) malloc(sizeof(threadListNode));
		tmp->data = data;
		tmp->next = NULL;
		tmp->f_joined = 0;
		if(self->head == NULL && self->tail == NULL){
			self->head = tmp;
			self->tail = tmp;
		}else{
			tmp->next = self->head;
			self->head = tmp;
		}
		self->nodeCount++;
		ret = 0;
	}
	return ret;
}
int removeThreadList(struct threadList* self, unsigned int index){
	int ret = -1;
	if(self != NULL){
		if(index < self->nodeCount){
			if(self->head != NULL){
				if(index==0 && self->head == self->tail){
					threadListNode* tmp = self->head;
					self->head = NULL;
					self->tail = NULL;
					free(tmp);
					ret = 0;
					self->nodeCount--;
				}else if(index == 0 && self->head != self->tail){
					threadListNode* tmp = self->head;
					self->head = self->head->next;
					free(tmp);
					ret = 0;
					self->nodeCount--;
				}else{
					threadListNode* current = self->head;
					int i = 0;
					threadListNode* last = NULL;
					while(current != NULL && i != index && i < self->nodeCount){
						i++;
						last = current;
						current = current->next;
					}
					if(current != NULL && i < self->nodeCount){
						if(index == (self->nodeCount-1)){
							self->tail = last;
						}
						last->next = current->next;
						free(current);
						ret = 0;
						self->nodeCount--;
					}
				}
				
			}
		}
	}
	return ret;
}
int getThreadList(struct threadList* self, unsigned int index, pthread_t* var){
	int ret = -1;
	if(self != NULL){
		if(index < self->nodeCount){
			if(self->head != NULL){
				if(index == 0){
					//printf("message: index is for head.\n");
					//fflush(stdout);
					*var = self->head->data;
					ret = 0;
				}else if(index == (self->nodeCount-1)){
					//printf("message: index is for tail.\n");
					//fflush(stdout);
					*var = self->tail->data;
					ret = 0;
				}else{
					int i = 0;
					threadListNode* current = self->head;
					while(current != NULL && i < self->nodeCount){
						if(i == index){
							//printf("message: enters if statement in while loop.\n");
							//fflush(stdout);
							*var = current->data;
							ret = 0;
							break;
						}
						i++;
						current = current->next;
					}
					//printf("\nmessage: made it to the end of getThreadList with index %d and i %d, nodecount: %d\n", index, i, self->nodeCount);
					//fflush(stdout);
				}
			}
		}
		
	}
	return ret;
}
int removeHeadThreadList(struct threadList* self){
	int ret = -1;
	if(self != NULL){
		if(self->head != NULL){
			ret = removeThreadList(self, 0);
		}
	}
	return ret;
}
int removeTailThreadList(struct threadList* self){
	int ret = -1;
	if(self != NULL){
		if(self->tail != NULL){
			ret = removeThreadList(self, (self->nodeCount-1));
		}
	}
	return ret;
}

int isEmptyThreadList(struct threadList* self){
	int ret = -1;
	if(self != NULL){
		if(self->head == NULL && self->tail == NULL){
			ret = 1;
		}else{
			ret = 0;
		}
	}
	return ret;
}
int waitForThreadsThreadList(struct threadList* self){
	int ret = -1;
	if(isEmptyThreadList(self) == 0){
		ret = 0;
		int i;
		threadListNode* current = self->head;
		while(current != NULL){
			//following join is non-portable and non-POSIX standard
			if(current->f_joined == 0 && current->f_removed == 0){
				if(pthread_tryjoin_np(current->data, NULL) == 0){
					current->f_joined = 1;
					ret++;
				}
			}
			current = current->next;
		}
		i = ret;
		current = self->head;
		int j = 0;
		while(i > 0 && current != NULL){
			if(current->f_joined == 1){
				/*
					The line below cannot be factored out without having another pointer. do not try to factor out!
					If the thread has been joined then it should be removed, and if its removed before we extract the 
					next node's pointer, we loose the reference to the next node.
				*/
				current = current->next; 
				removeThreadList(self, j);
				i--;
			}else{
				current = current->next;
				j++;
			}
		}
		
	}
	return ret;
}