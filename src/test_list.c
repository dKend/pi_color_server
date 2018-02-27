#include <stdio.h>
#include <stdlib.h>
#include "dstruct/lists.h"
#include "assertion.h"

//setup and tear down functions
void setUpList(node** self);
void tearDownList(node** self);
//tests for node struct functions
int testCreateNode();
int testAddFront();
int testAddEnd();
int testRemFront();
int testRemEnd();
int testListLen();
int testFreeNode();
int testFreeList();

int main(){
	int ret = 0;
	registerTest(testCreateNode, "testCreateNode");
	registerTest(testAddFront, "testAddFront");
	registerTest(testAddEnd, "testAddEnd");
	registerTest(testRemFront, "testRemFront");
	registerTest(testRemEnd, "testRemEnd");
	registerTest(testListLen, "testListLen");
	registerTest(testFreeNode, "testFreeNode");
	registerTest(testFreeList, "testFreeList");
	ret = runTests();
	//destroyTests();
	return ret;
}
void setUpList(node** self){
	//set up for a 1 element list
	*self = (node*)malloc(sizeof(node));
	int* ptr = (int*)malloc(sizeof(int));
	*ptr = 100;
	(*self)->data = ptr;
	(*self)->next = NULL;
}

void tearDownList(node** self){
	free((*self)->data);
	free(*self);
	*self = NULL;
}

int testCreateNode(){
	int ret = -1;
	node* self;
	node* next;
	self = NULL;
	next = NULL;
	setUpList(&next);
	int* testPointer = (int*)malloc(sizeof(int));
	assertFalse(create_node(NULL, NULL, NULL, NULL) == 0, "create_node succeeds with bad input (all NULL).", &ret);
	assertFalse(create_node(NULL, NULL, next, NULL) == 0, "create_node succeeds with bad input (all NULL but next).", &ret);
	assertFalse(create_node(NULL, (void*) testPointer, NULL, NULL) == 0, "create_node succeeds with bad input (self and next are NULL).", &ret);
	assertFalse(create_node(NULL, (void*) testPointer, next, NULL) == 0, "create_node succeeds with bad input (self if NULL).", &ret);
	assertFalse(create_node(&self, NULL, NULL, NULL) == 0, "create_node succeeds with bad input (all NULL but self).", &ret);
	assertFalse(create_node(&self, NULL, next, NULL) == 0, "create_node succeeds with bad input (all NULL but self and next).", &ret);
	assertTrue(create_node(&self, (void*) testPointer, NULL, NULL) == 0, "create_node Fails with valid input (only next is NULL).", &ret);
	if(ret != -1){
		//reset self if previous assert passed (self SHOULD have been allocated some heap space)
		free(self);
		self = NULL;
	}
	assertTrue(create_node(&self, (void*) testPointer, next, NULL) == 0, "create_node Fails with valid input (nothing is NULL).", &ret);
	tearDownList(&self);
	tearDownList(&next);
	return ret;
}
int testAddFront(){
	int ret = -1;
	node* node0;
	node* node1;
	node* node2;
	node* head;
	head = NULL;
	node0 = NULL;	//working node
	node1 = NULL;
	node2 = NULL;
	int* ptr0;
	int* ptr1;
	int* ptr2;
	ptr0 = (int*)malloc(sizeof(int));
	ptr1 = (int*)malloc(sizeof(int));
	ptr2 = (int*)malloc(sizeof(int));
	*ptr0 = 0;
	*ptr1 = 1;
	*ptr2 = 2;
	node0 = (node*)malloc(sizeof(node));
	node1 = (node*)malloc(sizeof(node));
	node2 = (node*)malloc(sizeof(node));
	node0->data = (void*)ptr0;
	node0->next = NULL;
	node1->data = (void*)ptr1;
	node1->next = NULL;
	node2->data = (void*)ptr2;
	node2->next = NULL;
	//empty list
	assertFalse(add_front(NULL, NULL) == 0, "add_front succeeds with bad input (NULL NULL).", &ret);
	assertFalse(add_front(NULL, node0) == 0, "add_front succeeds with bad input (NULL node0).", &ret);
	assertFalse(add_front(&head, NULL) == 0, "add_front succeeds with bad input (head NULL).", &ret);
	assertTrue(head == NULL, "add_front changes head on failure (head NULL).", &ret);
	assertTrue(add_front(&head, node0) == 0, "add_front fails with valid input (head, node0).", &ret);
	assertTrue(head == node0, "add_front doesnt set head node to passed node on valid input and list is empty (NULL) (head, node1).", &ret);
	assertTrue((int*)head->data == ptr0, "add_front doesnt set the new_node to the ehad node when the list is empty.", &ret);
	assertTrue(head->next == NULL, "add_front doesnt set the new_node to the head node when the list is empty.", &ret);
	//1 node list
	assertFalse(add_front(&head, NULL) == 0, "add_front succeeds with bad input on a single node list (head NULL).", &ret);
	assertTrue(head == node0, "add_front changes head on failure (head NULL).", &ret);
	assertTrue(add_front(&head, node1) == 0, "add_front fails with valid input (head, node1).", &ret);
	assertTrue(head == node1, "add_front doesnt change head on success (head, node1).", &ret);
	assertTrue(head->next == node0, "add_front doesnt change the next pointer of new_node the the previous head on a one node list.", &ret);
	assertTrue((int*)head->data == ptr1, "add_front doesnt change the head to the new_node on a single node list.", &ret);
	//n node list
	assertFalse(add_front(&head, NULL) == 0, "add_front succeeds with bad input on a two node list (head NULL).", &ret);
	assertTrue(head == node1, "add_front changes head on failure on a two node list (head NULL).", &ret);
	assertTrue(add_front(&head, node2) == 0, "add_front fails with valid input on a two node list (head, node2).", &ret);
	assertTrue(head == node2, "add_front doesnt change head on success on a two node list (head, node2).", &ret);
	assertTrue(head->next == node1, "add_front doesnt change the next pointer of new_node the the previous head on a two node list (head, node2).", &ret);
	assertTrue((int*)head->data == ptr2, "add_front doesnt change the head to the new_node on a two node list (head, node2).", &ret);
	head = NULL;
	node0->next = NULL;
	node1->next = NULL;
	node2->next = NULL;
	node0->data = NULL;
	node1->data = NULL;
	node2->data = NULL;
	free(ptr0);
	free(ptr1);
	free(ptr2);
	free(node0);
	free(node1);
	free(node2);
	return ret;
}
int testAddEnd(){
	int ret = -1;
	node* node0;
	node* node1;
	node* node2;
	node* head;
	head = NULL;
	node0 = NULL;	//working node
	node1 = NULL;
	node2 = NULL;
	int* ptr0;
	int* ptr1;
	int* ptr2;
	ptr0 = (int*)malloc(sizeof(int));
	ptr1 = (int*)malloc(sizeof(int));
	ptr2 = (int*)malloc(sizeof(int));
	*ptr0 = 0;
	*ptr1 = 1;
	*ptr2 = 2;
	node0 = (node*)malloc(sizeof(node));
	node1 = (node*)malloc(sizeof(node));
	node2 = (node*)malloc(sizeof(node));
	node0->data = (void*)ptr0;
	node0->next = NULL;
	node1->data = (void*)ptr1;
	node1->next = NULL;
	node2->data = (void*)ptr2;
	node2->next = NULL;
	//empty list (should be same behavior as add_front)
	assertFalse(add_end(NULL, NULL) == 0, "add_end succeeds with bad input on empty list (NULL NULL).", &ret);
	assertFalse(add_end(NULL, node0) == 0, "add_end succeeds with bad input on empty list (NULL node0).", &ret);
	assertFalse(add_end(&head, NULL) == 0, "add_end succeeds with bad input on empty list (head NULL).", &ret);
	assertTrue(head == NULL, "add_end changes head on failure on empty list (head NULL).", &ret);
	assertTrue(add_end(&head, node0) == 0, "add_end fails with valid input (head, node0).", &ret);
	assertTrue(head == node0, "add_end doesnt set head node to passed node on valid input and list is empty (NULL) (head, node1).", &ret);
	assertTrue((int*)head->data == ptr0, "add_end doesnt set the new_node to the ehad node when the list is empty.", &ret);
	assertTrue(head->next == NULL, "add_end doesnt set the new_node to the head node when the list is empty.", &ret);
	//1 node list
	assertFalse(add_end(&head, NULL) == 0, "add_end succeeds with bad input on a one node list (head NULL).", &ret);
	assertTrue(head->next == NULL, "add_end changes head on failure on a one node list (head NULL).", &ret);
	assertTrue(add_end(&head, node1) == 0, "add_end fails with valid input on a one node list (head, node1).", &ret);
	assertFalse(head->next == NULL, "add_end doesnt change head->next on success on a one node list (head, node1).", &ret);
	assertTrue(head->next == node1, "add_end doesnt change the next pointer of head to the new_node on a one node list.", &ret);
	//n node list
	if(ret != -1){
		assertFalse(add_end(&head, NULL) == 0, "add_end succeeds with bad input on a 2 node list (head NULL).", &ret);
		assertTrue(head->next == node1, "add_end changes head on failure on a 2 node list (head NULL).", &ret);
		assertTrue(add_end(&head, node2) == 0, "add_end fails with valid input on a 2 node list (head, node2).", &ret);
		assertFalse(head->next->next == NULL, "add_end doesnt change head->next on success on a 2 node list (head, node1).", &ret);
		assertTrue(head->next == node1, "add_end changes the next pointer of head to the new_node on a 2 node list.", &ret);
		assertTrue(head->next->next == node2, "add_end doesnt add new_node to the end of the list on a 2 node list.", &ret);
	}
	head = NULL;
	node0->next = NULL;
	node1->next = NULL;
	node2->next = NULL;
	node0->data = NULL;
	node1->data = NULL;
	node2->data = NULL;
	free(ptr0);
	free(ptr1);
	free(ptr2);
	free(node0);
	free(node1);
	free(node2);
	return ret;
}
int testRemFront(){
	int ret = -1;
	node* node0;
	node* node1;
	node* node2;
	node* head;
	node* retval;
	head = NULL; //working node
	node0 = NULL;	
	node1 = NULL;
	node2 = NULL;
	retval = NULL;
	int* ptr0;
	int* ptr1;
	int* ptr2;
	ptr0 = (int*)malloc(sizeof(int));
	ptr1 = (int*)malloc(sizeof(int));
	ptr2 = (int*)malloc(sizeof(int));
	*ptr0 = 0;
	*ptr1 = 1;
	*ptr2 = 2;
	node0 = (node*)malloc(sizeof(node));
	node1 = (node*)malloc(sizeof(node));
	node2 = (node*)malloc(sizeof(node));
	node0->data = (void*)ptr0;
	node0->next = NULL;
	node1->data = (void*)ptr1;
	node1->next = NULL;
	node2->data = (void*)ptr2;
	node2->next = NULL;
	head = node0;
	head->next = node1;
	head->next->next = node2;
	//n node list
	retval = rem_front(&head);
	assertTrue(retval == node0, "rem_front doesnt return the original head node on a 3 node list.", &ret);
	assertTrue(head == node1, "rem_front doesnt change the ehad pointer to the next node.", &ret);
	//2 node list
	retval = rem_front(&head);
	assertTrue(retval == node1, "rem_front doesnt return the original head node on a 2 node list.", &ret);
	assertTrue(head == node2, "rem_front doesnt change the ehad pointer to the next node.", &ret);
	//1 node list
	retval = rem_front(&head);
	assertTrue(retval == node2, "rem_front doesnt return the original head node on a 1 node list.", &ret);
	assertTrue(head == NULL, "rem_front doesnt change the ehad pointer to the next node.", &ret);
	//0 node list
	assertTrue(rem_front(&head) == NULL, "rem_front doesnt fail on an empty list.", &ret);
	assertTrue(rem_front(NULL) == NULL, "rem_front doesnt fail on NULL.", &ret);
	head = NULL;
	node0->next = NULL;
	node1->next = NULL;
	node2->next = NULL;
	node0->data = NULL;
	node1->data = NULL;
	node2->data = NULL;
	free(ptr0);
	free(ptr1);
	free(ptr2);
	free(node0);
	free(node1);
	free(node2);
	return ret;
}
int testRemEnd(){
	int ret = -1;
	node* node0;
	node* node1;
	node* node2;
	node* head;
	node* retval;
	head = NULL; //working node
	node0 = NULL;	
	node1 = NULL;
	node2 = NULL;
	retval = NULL;
	int* ptr0;
	int* ptr1;
	int* ptr2;
	ptr0 = (int*)malloc(sizeof(int));
	ptr1 = (int*)malloc(sizeof(int));
	ptr2 = (int*)malloc(sizeof(int));
	*ptr0 = 0;
	*ptr1 = 1;
	*ptr2 = 2;
	node0 = (node*)malloc(sizeof(node));
	node1 = (node*)malloc(sizeof(node));
	node2 = (node*)malloc(sizeof(node));
	node0->data = (void*)ptr0;
	node0->next = NULL;
	node1->data = (void*)ptr1;
	node1->next = NULL;
	node2->data = (void*)ptr2;
	node2->next = NULL;
	head = node0;
	head->next = node1;
	head->next->next = node2;
	//n node list
	retval = rem_end(&head);
	assertTrue(retval == node2, "rem_end doesnt return the original head node on a 3 node list.", &ret);
	assertTrue(head == node0, "rem_end doesnt change the ehad pointer to the next node.", &ret);
	//2 node list
	retval = rem_end(&head);
	assertTrue(retval == node1, "rem_end doesnt return the original head node on a 2 node list.", &ret);
	assertTrue(head == node0, "rem_end doesnt change the ehad pointer to the next node.", &ret);
	//1 node list
	retval = rem_end(&head);
	assertTrue(retval == node0, "rem_end doesnt return the original head node on a 1 node list.", &ret);
	assertTrue(head == NULL, "rem_end doesnt change the ehad pointer to the next node.", &ret);
	//0 node list
	assertTrue(rem_end(&head) == NULL, "rem_end doesnt fail on an empty list.", &ret);
	assertTrue(rem_end(NULL) == NULL, "rem_end doesnt fail on NULL.", &ret);
	head = NULL;
	node0->next = NULL;
	node1->next = NULL;
	node2->next = NULL;
	node0->data = NULL;
	node1->data = NULL;
	node2->data = NULL;
	free(ptr0);
	free(ptr1);
	free(ptr2);
	free(node0);
	free(node1);
	free(node2);
	return ret;
}
int testListLen(){
	int ret = -1;
	node* node0;
	node* node1;
	node* node2;
	node* head;
	head = NULL; //working node
	node0 = NULL;	
	node1 = NULL;
	node2 = NULL;
	int* ptr0;
	int* ptr1;
	int* ptr2;
	ptr0 = (int*)malloc(sizeof(int));
	ptr1 = (int*)malloc(sizeof(int));
	ptr2 = (int*)malloc(sizeof(int));
	*ptr0 = 0;
	*ptr1 = 1;
	*ptr2 = 2;
	node0 = (node*)malloc(sizeof(node));
	node1 = (node*)malloc(sizeof(node));
	node2 = (node*)malloc(sizeof(node));
	node0->data = (void*)ptr0;
	node0->next = NULL;
	node1->data = (void*)ptr1;
	node1->next = NULL;
	node2->data = (void*)ptr2;
	node2->next = NULL;
	head = node0;
	//empty list or NULL
	assertTrue(list_len(NULL) == 0, "list_len doesnt return 0 on empty list.", &ret);
	//1 element list
	assertTrue(list_len(head) == 1, "list_len doesnt return 1 for a one element list.", &ret);
	//2 element list
	head->next = node1;
	assertTrue(list_len(head) == 2, "list_len doesnt return 2 for a two element list.", &ret);
	//3 element list
	head->next->next = node2;
	assertTrue(list_len(head) == 3, "list_len doesnt return 3 for a three element list.", &ret);
	//cyclic tests were here, removed for reasons
	//create hard limit on how many nodes a list can have to prevent overflows.
	head = NULL;
	node0->next = NULL;
	node1->next = NULL;
	node2->next = NULL;
	node0->data = NULL;
	node1->data = NULL;
	node2->data = NULL;
	free(ptr0);
	free(ptr1);
	free(ptr2);
	free(node0);
	free(node1);
	free(node2);
	return ret;
}
int testFreeNode(){
	int ret = -1;
	int* ptr = (int*)malloc(sizeof(int));
	*ptr = 100;
	node* self;
	self = (node*)malloc(sizeof(node));
	self->data = ptr;
	self->next = NULL;
	assertTrue(free_node(NULL) == -1, "free_node doesnt fail when given invalid input NULL.", &ret);
	assertTrue(free_node(&self) == 0, "free_node fails for valid input.", &ret);
	assertTrue(self == NULL, "free_node doesnt set the passed node pointer to NULL on valid input.", &ret);
	if(ret == 0){
		free(self->data);
		self->data = NULL;
		free(self);
		self = NULL;
	}
	return ret;
}
int testFreeList(){
	int ret = -1;
	int* ptr0;
	int* ptr1;
	int* ptr2;
	ptr0 = (int*)malloc(sizeof(int));
	ptr1 = (int*)malloc(sizeof(int));
	ptr2 = (int*)malloc(sizeof(int));
	node* node0;
	node* node1;
	node* node2;
	node0 = (node*)malloc(sizeof(node));
	node1 = (node*)malloc(sizeof(node));
	node2 = (node*)malloc(sizeof(node));
	node0->data = ptr0;
	node1->data = ptr1;
	node2->data = ptr2;
	node0->next = node1;
	node1->next = node2;
	node2->next = NULL;
	assertTrue(free_list(NULL) == -1, "free_list doesnt fail on NULL.", &ret);
	assertTrue(free_list(&node0) == 0, "free_list fails on a 3 element list.", &ret);
	assertTrue(node0 == NULL, "free_list doesnt NULL the passed head node.", &ret);
	if(ret == 0){
		node0->next = NULL;
		node1->next = NULL;
		node2->next = NULL;
		node0->data = NULL;
		node1->data = NULL;
		node2->data = NULL;
		free(ptr0);
		free(ptr1);
		free(ptr2);
		free(node0);
		free(node1);
		free(node2);
	}
	return ret;
}
