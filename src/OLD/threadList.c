#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "threadList.h"

int initThreadList(struct threadList** self){
	int ret = -1;
	if(self != NULL){
		*self = (threadList*) malloc(sizeof(struct threadList));
		//temporary init function so that we can assure that the threadList we are working with is initialized as we expect it to be without the initThreadList function being called explicitly
		(*self)->head = NULL;
		(*self)->tail = NULL;
		(*self)->nextIndex = 0;
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
		tmp->index = getAndIncrNextIndexThreadList(self);
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
		if(index < self->nextIndex){
			if(self->head != NULL){
				if(self->head->index == index && self->head->index == self->tail->index){
					threadListNode* tmp = self->head;
					self->head = NULL;
					self->tail = NULL;
					free(tmp);
					ret = 0;
					self->nodeCount--;
				}else if(self->head->index == index){
					threadListNode* tmp = self->head;
					self->head = self->head->next;
					free(tmp);
					ret = 0;
					self->nodeCount--;
				}else{
					threadListNode* current = self->head;
					threadListNode* last = NULL;
					while(current != NULL){
						if(current->index == index){
							break;
						}
						last = current;
						current = current->next;
					}
					if(current != NULL){
						if(current->index == self->tail->index){
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
		if(index < self->nextIndex){
			if(self->head != NULL){
				if(self->head->index == index){
					*var = self->head->data;
					ret = 0;
				}else if(self->tail->index == index){
					*var = self->tail->data;
					ret = 0;
				}else{
					threadListNode* current = self->head->next;
					while(current != NULL){
						if(current->index == index){
							*var = current->data;
							ret = 0;
							break;
						}
						current = current->next;
					}
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
			ret = removeThreadList(self, self->head->index);
		}
	}
	return ret;
}
int removeTailThreadList(struct threadList* self){
	int ret = -1;
	if(self != NULL){
		if(self->tail != NULL){
			ret = removeThreadList(self, self->tail->index);
		}
	}
	return ret;
}
/*
int getAndIncrNextIndexThreadList(struct threadList* self){
	int ret = -1;
	if(self != NULL){
		ret = self->nextIndex;
		self->nextIndex++;
	}
	return ret;
}
*/
int isEmptyThreadList(struct threadList* self){
	int ret = -1;
	
	return ret;
}
int waitForThreadsNoBlockThreadList(struct threadList* self){
	int ret = -1
	
	return ret;
}
int waitForThreadsBlockThreadList(struct threadList* self){
	int ret = -1;
	
	return ret;
}
