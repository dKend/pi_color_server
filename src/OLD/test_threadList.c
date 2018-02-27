#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "assertion.h"
#include "threadList.h"
void setUp(struct threadList** self);
void tearDown(struct threadList** self);
int testInitThreadList();
int testFreeThreadList();
int testAddThreadList();
int testRemoveThreadList();
int testGetThreadList();
int testRemoveHeadThreadList();
int testRemoveTailThreadList();
int testGetAndIncrNextIndexThreadList();
int testIsEmptyThreadList();
int testWaitForThreadsNoBlockThreadList();
int testWaitForThreadsBlockThreadList();

void* dummyThreadOperation(void* arg);
void* dummyThreadOperationLong(void* arg);

int main(){
	int ret = 0;
	testInitThreadList();
	printf("1\n");
	fflush(stdout);
	testAddThreadList();
	printf("2\n");
	fflush(stdout);
	testRemoveThreadList();
	printf("3\n");
	fflush(stdout);
	testFreeThreadList();
	printf("4\n");
	fflush(stdout);
	testGetThreadList();
	printf("5\n");
	fflush(stdout);
	testRemoveHeadThreadList();
	printf("6\n");
	fflush(stdout);
	testRemoveTailThreadList();
	printf("7\n");
	fflush(stdout);
	
	return ret;
}
void setUp(struct threadList** self){
	*self = (threadList*) malloc(sizeof(struct threadList));
	//temporary init function so that we can assure that the threadList we are working with is initialized as we expect it to be without the initThreadList function being called explicitly
	(*self)->head = NULL;
	(*self)->tail = NULL;
	(*self)->nextIndex = 0;
	(*self)->nodeCount = 0;
}
void tearDown(struct threadList** self){
	//temporary free method to allow testing without depending on a method that hasn't been tested/defined yet
	void freedomHelper(threadListNode** node){
		if(*node != NULL){
			freedomHelper(&((*node)->next));
			free(*node);
		}
	}
	freedomHelper(&((*self)->head));
	free(*self);
}
int testInitThreadList(){
	int ret = -1;
	struct threadList* self;
	assertTrue(initThreadList(&self) == 0, "initThreadList doesn't succeed with a valid thread list argument.", &ret);
	assertTrue(self->head == NULL, "initThreadList doesn't set head to null.", &ret);
	assertTrue(self->tail == NULL, "initThreadList doesn't set head to null.", &ret);
	assertTrue(self->nextIndex == 0, "initThreadList doesn't set nextIndex to zero.", &ret);
	assertTrue(self->nodeCount == 0, "initThreadList doesn't set nextIndex to zero.", &ret);
	free(self);
	return ret;
}
int testAddThreadList(){
	int ret = -1;
	struct threadList* self;
	setUp(&self);
	pthread_t tid0, tid1, tid2;
	pthread_create(&tid0, NULL, dummyThreadOperation, NULL);
	pthread_join(tid0, NULL);
	pthread_create(&tid1, NULL, dummyThreadOperation, NULL);
	pthread_join(tid1, NULL);
	pthread_create(&tid2, NULL, dummyThreadOperation, NULL);
	pthread_join(tid2, NULL);
	assertTrue(addThreadList(self, tid0) != -1, "addThreadList fails with empty list.", &ret);
	assertTrue(self->head->data == tid0, "addThreadList doesn't set passed value as the value of the create node.", &ret);
	assertTrue(self->tail->data == tid0, "addThreadList doesn't set tail to be the same as head when called on empty list.", &ret);
	assertTrue(self->head->next == NULL, "addThreadList doesn't set next to NULL.", &ret);
	assertTrue(self->nextIndex == 1, "addThreadList doest change the next index.", &ret);
	assertTrue(self->nodeCount == 1, "addThreadList doesn't increase the node count integer.", &ret);
	assertTrue(addThreadList(self, tid1) != -1, "addThreadList fails when adding node to 1 element list.", &ret);
	assertTrue(self->head->data == tid1, "addThreadList doesn't set passed value as the value of created node when list has 1 node.", &ret);
	assertTrue(self->tail->data == tid0, "addThreadList doesn't set the original head node as the next node (tail in the case of one node list).", &ret);
	assertTrue(self->nextIndex == 2, "addThreadList doest change the next index.", &ret);
	assertTrue(self->nodeCount == 2, "addThreadList doesn't increase the node count integer.", &ret);
	assertTrue(addThreadList(self, tid2) != -1, "addThreadList fails when adding node to 2 element list.", &ret);
	assertTrue(self->head->data == tid2, "addThreadList doesn't set passed value as the value of created node when list has 2 nodes.", &ret);
	assertTrue(self->head->next != NULL, "addThreadList doesn't set old head as the next node of the new head.", &ret);
	assertTrue(self->head->next->data == tid1, "addThreadList doesn't set the original head as the next node of the new head.", &ret);
	assertTrue(self->tail->data == tid0, "addThreadList doesn't set the original head node as the next node (tail in the case of one node list).", &ret);
	assertTrue(self->nextIndex == 3, "addThreadList doest change the next index.", &ret);
	assertTrue(self->nodeCount == 3, "addThreadList doesn't increase the node count integer.", &ret);
	tearDown(&self);
	return ret;
}
int testRemoveThreadList(){
	int ret = -1;
	struct threadList* self;
	setUp(&self);
	pthread_t tid0, tid1, tid2;
	pthread_create(&tid0, NULL, dummyThreadOperation, NULL);
	pthread_join(tid0, NULL);
	pthread_create(&tid1, NULL, dummyThreadOperation, NULL);
	pthread_join(tid1, NULL);
	pthread_create(&tid2, NULL, dummyThreadOperation, NULL);
	pthread_join(tid2, NULL);
	//on empty list
	assertTrue(removeThreadList(self, 1) == -1, "removeThreadList doesnt fail when given an unused index and the list is empty.", &ret);
	assertTrue(self->nextIndex == 0, "removeThreadList incorrectly changes the value of next index when given an unused index and the list is empty.", &ret);
	assertTrue(self->nodeCount == 0, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is empty.", &ret);
	assertTrue(removeThreadList(self, 0) == -1, "removeThreadList doesnt fail when given an unused index and the list is empty.", &ret);
	assertTrue(self->nextIndex == 0, "removeThreadList incorrectly changes the value of next index when given an unused index and the list is empty.", &ret);
	assertTrue(self->nodeCount == 0, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is empty.", &ret);
	assertTrue(removeThreadList(self, 200) == -1, "removeThreadList doesnt fail when given an unused index and the list is empty.", &ret);
	assertTrue(self->nextIndex == 0, "removeThreadList incorrectly changes the value of next index when given an unused index and the list is empty.", &ret);
	assertTrue(self->nodeCount == 0, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is empty.", &ret);
	//on 1 element list
	addThreadList(self, tid0);
	assertTrue(removeThreadList(self, 1) == -1, "removeThreadList doesnt fail when given an unused index and the list is NOT empty.", &ret);
	assertTrue(self->nextIndex == 1, "removeThreadList incorrectly changes the next index value when it should not.", &ret);
	assertTrue(self->nodeCount == 1, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is NOT empty.", &ret);
	assertTrue(removeThreadList(self, 200) == -1, "removeThreadList doesnt fail when given an unused index and the list is NOT empty.", &ret);
	assertTrue(self->nextIndex == 1, "removeThreadList incorrectly changes the next index value when it should not.", &ret);
	assertTrue(self->nodeCount == 1, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is NOT empty.", &ret);
	assertTrue(removeThreadList(self, 0) == 0, "removeThreadList doesnt return zero when a valid index is given and the list is NOT empty.", &ret);
	assertTrue(self->nextIndex == 1, "removeThreadList incorrectly changes the next index value when it should not.", &ret);
	assertTrue(self->nodeCount == 0, "removeThreadList doesn't decrement the value of node count when a valid node is removed.", &ret);
	//on 3 element list
	addThreadList(self, tid0);
	addThreadList(self, tid1);
	addThreadList(self, tid2);
	assertTrue(removeThreadList(self, 0) == -1, "removeThreadList doesnt fail when given an index that was used but has been removed and the list is NOT empty.", &ret);
	assertTrue(self->nextIndex == 4, "removeThreadList incorrectly changes the next index value when it should not.", &ret);
	assertTrue(self->nodeCount == 3, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is NOT empty.", &ret);
	assertTrue(removeThreadList(self, 4) == -1, "removeThreadList doesnt fail when given an unused index and the list is NOT empty.", &ret);
	assertTrue(self->nextIndex == 4, "removeThreadList incorrectly changes the next index value when it should not.", &ret);
	assertTrue(self->nodeCount == 3, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is NOT empty.", &ret);
	assertTrue(removeThreadList(self, 200) == -1, "removeThreadList doesnt fail when given an unused index and the list is NOT empty.", &ret);
	assertTrue(self->nextIndex == 4, "removeThreadList incorrectly changes the next index value when it should not.", &ret);
	assertTrue(self->nodeCount == 3, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is NOT empty.", &ret);
	assertTrue(removeThreadList(self, 1) == 0, "removeThreadList doesnt return zero when a valid index is given and the list is NOT empty.", &ret);
	assertTrue(self->nextIndex == 4, "removeThreadList incorrectly changes the next index value when it should not.", &ret);
	assertTrue(self->nodeCount == 2, "removeThreadList doesn't decrement the value of node count when a valid node is removed.", &ret);
	assertTrue(removeThreadList(self, 2) == 0, "removeThreadList doesnt return zero when a valid index is given and the list is NOT empty.", &ret);
	assertTrue(self->nextIndex == 4, "removeThreadList incorrectly changes the next index value when it should not.", &ret);
	assertTrue(self->nodeCount == 1, "removeThreadList doesn't decrement the value of node count when a valid node is removed.", &ret);
	assertTrue(removeThreadList(self, 3) == 0, "removeThreadList doesnt return zero when a valid index is given and the list is NOT empty.", &ret);
	assertTrue(self->nextIndex == 4, "removeThreadList incorrectly changes the next index value when it should not.", &ret);
	assertTrue(self->nodeCount == 0, "removeThreadList doesn't decrement the value of node count when a valid node is removed.", &ret);
	
	tearDown(&self);
	return ret;
}
int testFreeThreadList(){
	int ret = -1;
	struct threadList* self;
	setUp(&self);
	pthread_t tid0, tid1, tid2;
	pthread_create(&tid0, NULL, dummyThreadOperation, NULL);
	pthread_join(tid0, NULL);
	pthread_create(&tid1, NULL, dummyThreadOperation, NULL);
	pthread_join(tid1, NULL);
	pthread_create(&tid2, NULL, dummyThreadOperation, NULL);
	pthread_join(tid2, NULL);
	addThreadList(self, tid0);
	addThreadList(self, tid1);
	addThreadList(self, tid2);
	assertTrue(freeThreadList(&self) != -1, "freeThreadList returns -1 when it should succeed.", &ret);
	assertTrue(self == NULL, "freeThreadList doesnt set given list to NULL before returning", &ret);
	assertTrue(freeThreadList(&self) == -1, "freeThreadList doesn't return -1 when given a pointer to NULL.", &ret);
	return ret;
}


int testGetThreadList(){
	int ret = -1;
	struct threadList* self;
	setUp(&self);
	pthread_t tid0, tid1, tid2, sto;
	pthread_create(&tid0, NULL, dummyThreadOperation, NULL);
	pthread_join(tid0, NULL);
	pthread_create(&tid1, NULL, dummyThreadOperation, NULL);
	pthread_join(tid1, NULL);
	pthread_create(&tid2, NULL, dummyThreadOperation, NULL);
	pthread_join(tid2, NULL);
	assertTrue(getThreadList(self, 0, &sto)==-1, "getThreadList doesn't fail when given an unused index and the list is empty.", &ret);
	assertTrue(getThreadList(self, 1, &sto)==-1, "getThreadList doesn't fail when given an unused index and the list is empty.", &ret);
	assertTrue(getThreadList(self, 200, &sto)==-1, "getThreadList doesn't fail when given an unused index and the list is empty.", &ret);
	addThreadList(self, tid0);
	assertTrue(getThreadList(self, 1, &sto)==-1, "getThreadList doesn't fail when given an unused index and the list is NOT empty (1 element).", &ret);
	assertTrue(getThreadList(self, 200, &sto)==-1, "getThreadList doesn't fail when given an unused index and the list is NOT empty (1 element).", &ret);
	assertTrue(getThreadList(self, 0, &sto)==0, "getThreadList fails when given a valid index and the list is NOT empty (1 element).", &ret);
	assertTrue(sto == tid0, "getThreadList doesn't set passed pointer to the correct value.", &ret);
	addThreadList(self, tid1);
	addThreadList(self, tid2);
	assertTrue(getThreadList(self, 3, &sto)==-1, "getThreadList doesn't fail when given an unused index and the list is NOT empty (3 elements).", &ret);
	assertTrue(getThreadList(self, 200, &sto)==-1, "getThreadList doesn't fail when given an unused index and the list is NOT empty (3 elements).", &ret);
	assertTrue(getThreadList(self, 0, &sto)==0, "getThreadList fails when given a valid index and the list is NOT empty (1 element).", &ret);
	assertTrue(sto == tid0, "getThreadList doesn't set passed pointer to the correct value.", &ret);
	assertTrue(getThreadList(self, 1, &sto)==0, "getThreadList fails when given a valid index and the list is NOT empty (1 element).", &ret);
	assertTrue(sto == tid1, "getThreadList doesn't set passed pointer to the correct value.", &ret);
	assertTrue(getThreadList(self, 2, &sto)==0, "getThreadList fails when given a valid index and the list is NOT empty (1 element).", &ret);
	assertTrue(sto == tid2, "getThreadList doesn't set passed pointer to the correct value.", &ret);
	tearDown(&self);
	return ret;
}
int testRemoveHeadThreadList(){
	int ret = -1;
	struct threadList* self;
	setUp(&self);
	pthread_t tid0, tid1, tid2;
	pthread_create(&tid0, NULL, dummyThreadOperation, NULL);
	pthread_join(tid0, NULL);
	pthread_create(&tid1, NULL, dummyThreadOperation, NULL);
	pthread_join(tid1, NULL);
	pthread_create(&tid2, NULL, dummyThreadOperation, NULL);
	pthread_join(tid2, NULL);
	assertTrue(removeHeadThreadList(self) == -1, "removeHeadThreadList doesnt return -1 when the list is empty.", &ret);
	addThreadList(self, tid0);
	assertTrue(removeHeadThreadList(self) == 0, "removeHeadThreadList fails when the list is NOI empty.", &ret);
	assertTrue(self->head == NULL, "removeHeadThreadList didnt remove the head node when list was NOT empty.", &ret);
	assertTrue(removeHeadThreadList(self) == -1, "removeHeadThreadList doesnt fail when the list is empty and/or head was not removed by previous call.", &ret);
	addThreadList(self, tid0);
	addThreadList(self, tid1);
	addThreadList(self, tid2);
	assertTrue(removeHeadThreadList(self) == 0, "removeHeadThreadList fails when the list is NOT empty.", &ret);
	assertTrue(self->head->data == tid1, "removeHeadThreadList didnt remove the head node when list was NOT empty.", &ret);
	assertTrue(removeHeadThreadList(self) == 0, "removeHeadThreadList fails when the list is NOT empty.", &ret);
	assertTrue(self->head->data == tid2, "removeHeadThreadList didnt remove the head node when list was NOT empty.", &ret);
	assertTrue(removeHeadThreadList(self) == 0, "removeHeadThreadList fails when the list is NOT empty.", &ret);
	assertTrue(self->head == NULL, "removeHeadThreadList didnt remove the head node when list was NOT empty.", &ret);
	assertTrue(removeHeadThreadList(self) == -1, "removeHeadThreadList doesnt fail when the list is empty.", &ret);
	tearDown(&self);
	return ret;
}
int testRemoveTailThreadList(){
	int ret = -1;
	struct threadList* self;
	setUp(&self);
	pthread_t tid0, tid1, tid2;
	pthread_create(&tid0, NULL, dummyThreadOperation, NULL);
	pthread_join(tid0, NULL);
	pthread_create(&tid1, NULL, dummyThreadOperation, NULL);
	pthread_join(tid1, NULL);
	pthread_create(&tid2, NULL, dummyThreadOperation, NULL);
	pthread_join(tid2, NULL);
	assertTrue(removeTailThreadList(self) == -1, "removeTailThreadList doesnt return -1 when the list is empty.", &ret);
	addThreadList(self, tid0);
	assertTrue(removeTailThreadList(self) == 0, "removeTailThreadList fails when the list is NOT empty.", &ret);
	assertTrue(self->tail == NULL, "removeTailThreadList didnt remove the tail node when the list was not empty.", &ret);
	assertTrue(removeTailThreadList(self) == -1, "removeTailThreadList doesnt return -1 when the list is empty OR the tail node was not removed by the previous call.", &ret);
	addThreadList(self, tid0);
	addThreadList(self, tid1);
	addThreadList(self, tid2);
	assertTrue(removeTailThreadList(self) == 0, "removeTailThreadList fails when the list is NOT empty.", &ret);
	assertTrue(self->tail->data == tid1, "removeTailThreadList didnt remove the tail node when the list was NOT empty.", &ret);
	assertTrue(removeTailThreadList(self) == 0, "removeTailThreadList fails when the list is NOT empty.", &ret);
	assertTrue(self->tail->data == tid2, "removeTailThreadList didnt remove the tail node when the list was NOT empty.", &ret);
	assertTrue(removeTailThreadList(self) == 0, "removeTailThreadList fails when the list is NOT empty.", &ret);
	assertTrue(self->tail == NULL, "removeTailThreadList didnt remove the tail node when the list was NOT empty.", &ret);
	assertTrue(removeTailThreadList(self) == -1, "removeTailThreadList doesnt return -1 when the list is empty.", &ret);
	tearDown(&self);
	return ret;
}
/*
int testGetAndIncrNextIndexThreadList(){
	int ret = -1;
	struct threadList* self;
	setUp(&self);
	assertTrue(getAndIncrNextIndexThreadList(NULL) == -1, "getAndIncrNextIndexThreadList doesnt fail when given NULL.", &ret);
	assertTrue(getAndIncrNextIndexThreadList(self) == 0, "getAndIncrNextIndexThreadList doesnt return zero on first call.", &ret);
	assertTrue(self->nextIndex == 1, "getAndIncrNextIndexThreadList doesnt incr next index.", &ret);
	assertTrue(getAndIncrNextIndexThreadList(self) == 1, "getAndIncrNextIndexThreadList doesnt return 1 on 2nd call.", &ret);
	assertTrue(self->nextIndex == 2, "getAndIncrNextIndexThreadList doesnt incr next index.", &ret);
	assertTrue(getAndIncrNextIndexThreadList(self) == 2, "getAndIncrNextIndexThreadList doesnt return 2 on 3rd call.", &ret);
	assertTrue(self->nextIndex == 3, "getAndIncrNextIndexThreadList doesnt incr next index.", &ret);
	assertTrue(getAndIncrNextIndexThreadList(self) == 3, "getAndIncrNextIndexThreadList doesnt return 3 on 4th call.", &ret);
	assertTrue(self->nextIndex == 4, "getAndIncrNextIndexThreadList doesnt incr next index.", &ret);
	assertTrue(getAndIncrNextIndexThreadList(self) == 4, "getAndIncrNextIndexThreadList doesnt return 4 on 5th call.", &ret);
	assertTrue(self->nextIndex == 5, "getAndIncrNextIndexThreadList doesnt incr next index.", &ret);
	assertTrue(getAndIncrNextIndexThreadList(self) == 5, "getAndIncrNextIndexThreadList doesnt return 5 on 6th call.", &ret);
	assertTrue(self->nextIndex == 6, "getAndIncrNextIndexThreadList doesnt incr next index.", &ret);
	assertTrue(getAndIncrNextIndexThreadList(self) == 6, "getAndIncrNextIndexThreadList doesnt return 6 on 7th call.", &ret);
	assertTrue(self->nextIndex == 7, "getAndIncrNextIndexThreadList doesnt incr next index.", &ret);
	assertTrue(getAndIncrNextIndexThreadList(self) == 7, "getAndIncrNextIndexThreadList doesnt return 7 on 8th call.", &ret);
	assertTrue(self->nextIndex == 8, "getAndIncrNextIndexThreadList doesnt incr next index.", &ret);
	assertTrue(getAndIncrNextIndexThreadList(self) == 8, "getAndIncrNextIndexThreadList doesnt return 8 on 9th call.", &ret);
	assertTrue(self->nextIndex == 9, "getAndIncrNextIndexThreadList doesnt incr next index.", &ret);
	assertTrue(getAndIncrNextIndexThreadList(self) == 9, "getAndIncrNextIndexThreadList doesnt return 9 on 10th call.", &ret);
	assertTrue(self->nextIndex == 10, "getAndIncrNextIndexThreadList doesnt incr next index.", &ret);
	tearDown(&self);
	return ret;
}
*/
int testIsEmptyThreadList(){
	int ret = -1;
	struct threadList* self;
	setUp(&self);
	assertTrue()
	tearDown(&self);
	return ret;
}
int testWaitForThreadsNoBlockThreadList(){
	int ret = -1;
	struct threadList* self;
	setUp(&self);
	
	tearDown(&self);
	return ret;
}
int testWaitForThreadsBlockThreadList(){
	int ret = -1;
	struct threadList* self;
	setUp(&self);
	
	tearDown(&self);
	return ret;
}
void* dummyThreadOperation(void* arg){
	//this does nothing but allow tests to use pthread_create to generate pthread_t values besides the default value set at declaration.
	return NULL;
}
void* dummyThreadOperationLong(void* arg){
	int i;
	for(i=0;i<100;i++);
	return NULL;
}