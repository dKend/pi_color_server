#include "dstruct/lists.h"
typedef struct ListQueue{
	node* head;
	node* tail;
} ListQueue;
int createListQueue(ListQueue** self);
int freeListQueue(ListQueue** self);
int qListQueue(ListQueue* self, void* data);
int deqListQueue(ListQueue* self, void* data);
int isEmptyListQueue(ListQueue* self);