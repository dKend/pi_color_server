#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
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
int testWaitForThreadsThreadList();
void* dummyThreadOperation(void* arg);
void* dummyThreadOperationLong(void* arg);

int main(){
	int ret = 0;
	registerTest(testInitThreadList, "testInitThreadList");
	registerTest(testAddThreadList, "testAddThreadList");
	registerTest(testRemoveThreadList, "testRemoveThreadList");
	registerTest(testFreeThreadList, "testFreeThreadList");
	registerTest(testGetThreadList, "testGetThreadList");
	registerTest(testRemoveHeadThreadList, "testRemoveHeadThreadList");
	registerTest(testRemoveTailThreadList, "testRemoveTailThreadList");
	registerTest(testIsEmptyThreadList, "testIsEmptyThreadList");
	registerTest(testWaitForThreadsThreadList, "testWaitForThreadsThreadList");
	runTests();
	destroyTests();
	return ret;
}
void setUp(struct threadList** self){
	*self = (threadList*) malloc(sizeof(struct threadList));
	//temporary init function so that we can assure that the threadList we are working with is initialized as we expect it to be without the initThreadList function being called explicitly
	(*self)->head = NULL;
	(*self)->tail = NULL;
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
	assertTrue(self->nodeCount == 0, "initThreadList doesn't set nodeCount to zero.", &ret);
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
	assertTrue(self->nodeCount == 1, "addThreadList doesn't increase the node count integer.", &ret);
	assertTrue(addThreadList(self, tid1) != -1, "addThreadList fails when adding node to 1 element list.", &ret);
	assertTrue(self->head->data == tid1, "addThreadList doesn't set passed value as the value of created node when list has 1 node.", &ret);
	assertTrue(self->tail->data == tid0, "addThreadList doesn't set the original head node as the next node (tail in the case of one node list).", &ret);
	assertTrue(self->nodeCount == 2, "addThreadList doesn't increase the node count integer.", &ret);
	assertTrue(addThreadList(self, tid2) != -1, "addThreadList fails when adding node to 2 element list.", &ret);
	assertTrue(self->head->data == tid2, "addThreadList doesn't set passed value as the value of created node when list has 2 nodes.", &ret);
	assertTrue(self->head->next != NULL, "addThreadList doesn't set old head as the next node of the new head.", &ret);
	assertTrue(self->head->next->data == tid1, "addThreadList doesn't set the original head as the next node of the new head.", &ret);
	assertTrue(self->tail->data == tid0, "addThreadList doesn't set the original head node as the next node (tail in the case of one node list).", &ret);
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
	assertTrue(self->nodeCount == 0, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is empty.", &ret);
	assertTrue(removeThreadList(self, 0) == -1, "removeThreadList doesnt fail when given an unused index and the list is empty.", &ret);
	assertTrue(self->nodeCount == 0, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is empty.", &ret);
	assertTrue(removeThreadList(self, 200) == -1, "removeThreadList doesnt fail when given an unused index and the list is empty.", &ret);
	assertTrue(self->nodeCount == 0, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is empty.", &ret);
	//on 1 element list
	addThreadList(self, tid0); //0
	assertTrue(removeThreadList(self, 1) == -1, "removeThreadList doesnt fail when given an unused index and the list is NOT empty.", &ret);
	assertTrue(self->nodeCount == 1, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is NOT empty.", &ret);
	assertTrue(removeThreadList(self, 200) == -1, "removeThreadList doesnt fail when given an unused index and the list is NOT empty.", &ret);
	assertTrue(self->nodeCount == 1, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is NOT empty.", &ret);
	assertTrue(removeThreadList(self, 0) == 0, "removeThreadList doesnt return zero when a valid index is given and the list is NOT empty.", &ret);
	assertTrue(self->nodeCount == 0, "removeThreadList doesn't decrement the value of node count when a valid node is removed.", &ret);
	//on 3 element list
	addThreadList(self, tid0); //0
	addThreadList(self, tid1); //1
	addThreadList(self, tid2); //2
	assertTrue(removeThreadList(self, 0) == 0, "removeThreadList fails when given a valid index and the list is NOT empty.", &ret);
	assertTrue(self->nodeCount == 2, "removeThreadList doesnt change the value of node count when given a valid index and the list is NOT empty.", &ret);
	assertTrue(removeThreadList(self, 4) == -1, "removeThreadList doesnt fail when given an unused index and the list is NOT empty.", &ret);
	assertTrue(self->nodeCount == 2, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is NOT empty.", &ret);
	assertTrue(removeThreadList(self, 200) == -1, "removeThreadList doesnt fail when given an unused index and the list is NOT empty.", &ret);
	assertTrue(self->nodeCount == 2, "removeThreadList incorrectly changes the value of node count when given an unused index and the list is NOT empty.", &ret);
	assertTrue(removeThreadList(self, 1) == 0, "removeThreadList doesnt return zero when a valid index is given and the list is NOT empty.", &ret);
	assertTrue(self->nodeCount == 1, "removeThreadList doesn't decrement the value of node count when a valid node is removed.", &ret);
	assertTrue(removeThreadList(self, 2) == -1, "removeThreadList doesnt fail when given an invalid index and the list is NOT empty.", &ret);
	assertTrue(self->nodeCount == 1, "removeThreadList changes the value of nodeCount when an invalid index is given.", &ret);
	assertTrue(removeThreadList(self, 3) == -1, "removeThreadList doesnt fail when given an invalid index and the list is NOT empty.", &ret);
	assertTrue(self->nodeCount == 1, "removeThreadList changes the value of nodeCount when an invalid index is given.", &ret);
	assertTrue(removeThreadList(self, 0) == 0, "removeThreadList fails when given a valid index and the list is not empty.", &ret);
	assertTrue(self->nodeCount == 0, "removeThreadList doesnt change the value of nodeCount when given a valid index and the list is not empty.", &ret);
	
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
	assertTrue(getThreadList(self, 0, &sto)==0, "getThreadList fails when given a valid index and the list is NOT empty (3 element).", &ret);
	assertTrue(sto == tid0, "getThreadList doesn't set passed pointer to the correct value.", &ret);
	assertTrue(getThreadList(self, 1, &sto)==0, "getThreadList fails when given a valid index and the list is NOT empty (3 element). yes this test fails.", &ret);
	assertTrue(sto == tid1, "getThreadList doesn't set passed pointer to the correct value.", &ret);
	assertTrue(getThreadList(self, 2, &sto)==0, "getThreadList fails when given a valid index and the list is NOT empty (3 element).", &ret);
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
int testIsEmptyThreadList(){
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
	assertTrue(isEmptyThreadList(NULL) == -1, "isEmptyThreadList doesnt return -1 when given NULL.", &ret);
	assertTrue(isEmptyThreadList(self) == 1, "isEmptyThreadList doesnt return 1 when the list is empty.", &ret);
	addThreadList(self, tid0);
	assertTrue(isEmptyThreadList(self) == 0, "isEmptyThreadList doesnt return 0 when the list is NOT empty (1 element).", &ret);
	addThreadList(self, tid1);
	addThreadList(self, tid2);
	assertTrue(isEmptyThreadList(self) == 0, "isEmptyThreadList doesnt return 0 when the list is NOT empty (3 elements).", &ret);
	tearDown(&self);
	return ret;
}
int testWaitForThreadsThreadList(){
	int ret = -1;
	struct threadList* self;
	setUp(&self);
	pthread_t tid0, tid1, tid2;
	pthread_create(&tid0, NULL, dummyThreadOperation, NULL);
	pthread_create(&tid1, NULL, dummyThreadOperation, NULL);
	pthread_create(&tid2, NULL, dummyThreadOperation, NULL);
	assertTrue(waitForThreadsThreadList(NULL) == -1, "waitForThreadsThreadList doesnt fail when given NULL.", &ret);
	assertTrue(waitForThreadsThreadList(self) == -1, "waitForThreadsThreadList doesnt fail when given empty list.", &ret);
	addThreadList(self, tid0);
	sleep(2);
	assertTrue(waitForThreadsThreadList(self) == 1,	"waitForThreadsThreadList fails when attempting to join all threads and list is NOT empty (1 element).", &ret);
	assertTrue(self->head == NULL, "waitForThreadsThreadList doesnt remove joined threads.", &ret);
	addThreadList(self, tid1);
	addThreadList(self, tid2);
	sleep(2);
	assertTrue(waitForThreadsThreadList(self) == 2, "waitForThreadsThreadList fails when attemption to join all threads and list is NOT empty (2 element).", &ret);
	assertTrue(self->head == NULL, "waitForThreadsThreadList doesnt remove joined threads.", &ret);
	//cant check if all threads actually got joined because this is running on a single CPU system and the threads may not be scheduled to execute before this check is made
	if(self->head != NULL){
		//not all threads were successfully joined
		pthread_join(tid0, NULL);
		pthread_join(tid1, NULL);
		pthread_join(tid2, NULL);
	}
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