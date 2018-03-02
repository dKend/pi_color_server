#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "assertion.h"
int testInitColorList();
int testAddColorList();
int testFreeColorList();
int testPrintColorList();
int testSinColorCycle();
int testGenSinCurveColorList();
int testGenerateFadeToColor();
void setUp(struct colorList** self);
void tearDown(struct colorList** self);
int testInitColorListQueue();
int testQueueColorListQueue();
int testDequeueColorListQueue();
int testFreeColorListQueue();
int testInitColorListQueueNode();
int testUpdateColorListQueueNode();
int testIsComplete();
void setUp2(struct colorListQueue** self);
void tearDown2(struct colorListQueue** self);

int main(){
	registerTest(testInitColorList, "testInitColorList");
	registerTest(testAddColorList, "testAddColorList");
	registerTest(testFreeColorList, "testFreeColorList");
	registerTest(testSinColorCycle, "testSinColorCycle");
	registerTest(testGenSinCurveColorList, "testGenSinCurveColorList");
	//registerTest(testGenerateFadeToColor, "testGenerateFadeToColor");
	runTests();
	destroyTests();
	return 0;
}
void setUp(colorList** self){
	*self = (colorList*)malloc(sizeof(colorList));
	(*self)->head = NULL;
	(*self)->tail = NULL;
	(*self)->nodeCount = 0;
}
void tearDown(colorList** self){
	//temporary free method to allow testing without depending on a method that hasn't been tested/defined yet
	/*
	void freedomHelper(node** node){
		if(*node != NULL){
			freedomHelper(&((*node)->next));
			free(*node);
		}
	}
	*/
	free_list(&((*self)->head));
	free(*self);
}
int testInitColorList(){
	int ret = -1;
	colorList* self = NULL;
	assertEqualsInt(-1, initColorList(NULL), "initColorList doesnt fail when given NULL.", &ret);
	assertEqualsInt(0, initColorList(&self), "initColorList fails when given a valid pointer.", &ret);
	assertEqualsInt(-1, initColorList(&self), "initColorList doesnt fail when given a previously initialized colorList.", &ret);
	if(self != NULL){
		assertTrue(self->head == NULL, "initColorList doesnt set head to NULL.", &ret);
		assertTrue(self->tail == NULL, "initColorList doesnt set tail to NULL.", &ret);
		assertEqualsInt(0, self->nodeCount, "initColorList doesnt set nodeCount to 0.", &ret);
	}else{
		assertTrue(0!=0, "initColorList didnt allocate memory for given pointer.", &ret);
	}
	tearDown(&self);
	return ret;
}
int testAddColorList(){
	int ret = -1;
	colorList* self = NULL;
	color* c0 = (color*)malloc(sizeof(color));
	c0->red = 255;
	c0->green = 255;
	c0->blue = 255;
	c0->brightness = 255;
	c0->delay = 1000;
	color* c1 = (color*)malloc(sizeof(color));
	c1->red = 0;
	c1->green = 0;
	c1->blue = 0;
	c1->brightness = 0;
	c1->delay = 12;
	color* c2 = (color*)malloc(sizeof(color));
	c2->red = 1;
	c2->green = 1;
	c2->blue = 1;
	c2->brightness = 1;
	c2->delay = 500;
	setUp(&self);
	//adding to empty list
	assertTrue(addColorList(NULL, NULL) == -1, "addColorList doesnt fail when given 2 NULL pointers.", &ret);
	assertTrue(addColorList(self, NULL) == -1, "addColorList doesnt fail when give a NULL pointer for color.", &ret);
	assertTrue(addColorList(NULL, c0) == -1, "addColorList doesnt fail when given a NULL pointer for list.", &ret);
	assertTrue(addColorList(self, c0) == 0, "addColorList fails when given valid arguments.", &ret);
	if(assertTrue(self->head != NULL, "addColorList fails to add a new node to the color list upon success being returned with an empty list.", &ret) == 1){
		assertTrue(self->head->next == NULL, "addColorList fails to set new node's next pointer to null when list is empty.", &ret);
		if(assertTrue(self->head->data != NULL, "addColorList fails to set the color within the colorNode when adding to an empty list.", &ret) == 1){
			assertEqualsInt(c0->red, ((color*)(self->head->data))->red, "addColorList fails to set the color of new node to passed color0.", &ret);
			assertEqualsInt(c0->green, ((color*)(self->head->data))->green, "addColorList fails to set the color of new node to passed color1.", &ret);
			assertEqualsInt(c0->blue, ((color*)(self->head->data))->blue, "addColorList fails to set the color of new node to passed color2.", &ret);
			assertEqualsInt(c0->brightness, ((color*)(self->head->data))->brightness, "addColorList fails to set the color of new node to passed color3.", &ret);
			assertEqualsInt(c0->delay, ((color*)(self->head->data))->delay, "addColorList fails to set the color of new node to passed color4.", &ret);
		}
	}
	if(assertTrue(self->tail != NULL, "addColorList fails to set tail to the new node when adding a node to an emty list.", &ret) == 1){
		assertTrue(self->tail == self->head, "addColorList fails to set tail to the same address as head when adding to an empty list.", &ret);
	}
	//adding to single node list
	
	assertTrue(addColorList(self, NULL) == -1, "addColorList doesnt fail when give a NULL pointer for color where self is a single ndoe list.", &ret);
	assertTrue(addColorList(self, c1) == 0, "addColorList fails when given valid arguments where self is a single node list.", &ret);
	if(assertTrue(self->head != NULL && self->tail != NULL, "addColorList changes head/tail to null unexpectedly.", &ret) == 1){
		assertTrue(self->head->next == self->tail, "addColorList fails to set the next pointer of head to the new tail node when self is a single node list.", &ret);
		assertTrue(self->tail->next == NULL, "addColorList fails to set the next node of tail to null when self is a single node list.", &ret);
		if(assertTrue(self->tail->data != NULL && self->head->data != NULL, "addColorList fails to set the color within the colorNode when adding to an empty list OR head's data pointer is NULL.", &ret) == 1){
			assertEqualsInt(c1->red, ((color*)(self->tail->data))->red, "addColorList fails to set the color of new node to passed color5.", &ret);
			assertEqualsInt(c1->green, ((color*)(self->tail->data))->green, "addColorList fails to set the color of new node to passed color6.", &ret);
			assertEqualsInt(c1->blue, ((color*)(self->tail->data))->blue, "addColorList fails to set the color of new node to passed color7.", &ret);
			assertEqualsInt(c1->brightness, ((color*)(self->tail->data))->brightness, "addColorList fails to set the color of new node to passed color8.", &ret);
			assertEqualsInt(c1->delay, ((color*)(self->tail->data))->delay, "addColorList fails to set the color of new node to passed color9.", &ret);
			
			assertEqualsInt(c0->red, ((color*)(self->head->data))->red, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0->green, ((color*)(self->head->data))->green, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0->blue, ((color*)(self->head->data))->blue, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0->brightness, ((color*)(self->head->data))->brightness, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0->delay, ((color*)(self->head->data))->delay, "addColorList fails to retain the data within the head node.", &ret);
		}
	}
	if(assertTrue(self->tail != NULL, "addColorList fails to set tail to the new node when adding a node to a single node list.", &ret) == 1){
		assertTrue(self->tail != self->head, "addColorList sets tail to the same address as head when adding to a single node list.", &ret);
	}
	//adding to a n node list
	assertTrue(addColorList(self, NULL) == -1, "addColorList doesnt fail when give a NULL pointer for color where self is a n ndoe list.", &ret);
	assertTrue(addColorList(self, c2) == 0, "addColorList fails when given valid arguments where self is a n node list.", &ret);
	if(assertTrue(self->head != NULL && self->tail != NULL, "addColorList changes head/tail to null unexpectedly.", &ret) == 1){
		assertTrue(self->head->next != self->tail, "addColorList fails to change the tail pointer to the new node when self is a n node list.", &ret);
		assertTrue(self->tail->next == NULL, "addColorList fails to set the next node of tail to null when self is a n node list.", &ret);
		if(assertTrue(self->tail->data != NULL && self->head->data != NULL, "addColorList fails to set the color within the colorNode when adding to an empty list OR head's data pointer is NULL.", &ret) == 1){
			assertEqualsInt(c2->red, ((color*)(self->tail->data))->red, "addColorList fails to set the color of new node to passed color10.", &ret);
			assertEqualsInt(c2->green, ((color*)(self->tail->data))->green, "addColorList fails to set the color of new node to passed color11.", &ret);
			assertEqualsInt(c2->blue, ((color*)(self->tail->data))->blue, "addColorList fails to set the color of new node to passed color12.", &ret);
			assertEqualsInt(c2->brightness, ((color*)(self->tail->data))->brightness, "addColorList fails to set the color of new node to passed color13.", &ret);
			assertEqualsInt(c2->delay, ((color*)(self->tail->data))->delay, "addColorList fails to set the color of new node to passed color14.", &ret);
			
			if(assertTrue(self->head->next != NULL, "next is null.", &ret)){
				if(assertTrue(self->head->next->data != NULL, "data is null.", &ret)){
					assertEqualsInt(c1->red, ((color*)(self->head->next->data))->red, "addColorList fails to set the color of new node to passed color15.", &ret);
					assertEqualsInt(c1->green, ((color*)(self->head->next->data))->green, "addColorList fails to set the color of new node to passed color16.", &ret);
					assertEqualsInt(c1->blue, ((color*)(self->head->next->data))->blue, "addColorList fails to set the color of new node to passed color17.", &ret);
					assertEqualsInt(c1->brightness, ((color*)(self->head->next->data))->brightness, "addColorList fails to set the color of new node to passed color18.", &ret);
					assertEqualsInt(c1->delay, ((color*)(self->head->next->data))->delay, "addColorList fails to set the color of new node to passed color19.", &ret);
				}
			}
			assertEqualsInt(c0->red, ((color*)(self->head->data))->red, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0->green, ((color*)(self->head->data))->green, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0->blue, ((color*)(self->head->data))->blue, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0->brightness, ((color*)(self->head->data))->brightness, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0->delay, ((color*)(self->head->data))->delay, "addColorList fails to retain the data within the head node.", &ret);
		}
	}
	if(assertTrue(self->tail != NULL, "addColorList fails to set tail to the new node when adding a node to a n node list.", &ret) == 1){
		assertTrue(self->tail != self->head, "addColorList sets tail to the same address as head when adding to a n node list.", &ret);
	}
	tearDown(&self);
	return ret;
}
int testFreeColorList(){
	int ret = -1;
	colorList* self = NULL;
	color* c0 = (color*)malloc(sizeof(color));
	c0->red = 255;
	c0->green = 255;
	c0->blue = 255;
	c0->brightness = 255;
	c0->delay = 1000;
	color* c1 = (color*)malloc(sizeof(color));
	c1->red = 0;
	c1->green = 0;
	c1->blue = 0;
	c1->brightness = 0;
	c1->delay = 12;
	color* c2 = (color*)malloc(sizeof(color));
	c2->red = 1;
	c2->green = 1;
	c2->blue = 1;
	c2->brightness = 1;
	c2->delay = 500;
	assertTrue(freeColorList(NULL) == -1, "freeColorList doesnt fail when given NULL.", &ret);
	assertTrue(freeColorList(&self) == -1, "freeColorList doesnt fail when given an uninitialized list.", &ret);
	setUp(&self);
	assertTrue(freeColorList(&self) == 0, "freeColorList fails when given a valid list and list is empty.", &ret);
	assertTrue(self == NULL, "freeColorList doesnt set given pointer to NULL on success and list is empty.", &ret);
	setUp(&self);
	addColorList(self, c0);
	assertTrue(freeColorList(&self) == 0, "freeColorList fails when given a valid list and list has one element.", &ret);
	assertTrue(self == NULL, "freeColorList doesnt set given pointer to NULL on success and list has one element.", &ret);
	if(ret == 1){
		c0 = (color*)malloc(sizeof(color));
		c0->red = 255;
		c0->green = 255;
		c0->blue = 255;
		c0->brightness = 255;
		c0->delay = 1000;
	}
	setUp(&self);
	addColorList(self, c0);
	addColorList(self, c1);
	addColorList(self, c2);
	if(assertTrue(freeColorList(&self) == 0, "freeColorList fails with valid list.", &ret) == 1){
		assertTrue(self == NULL, "freeColorList doesnt set given pointer to NULL when its a valid list.", &ret);
	}else{
		//self hasnt been freed since freeColorList failed
		tearDown(&self);
	}
	return ret;
}
int testSinColorCycle(){
	int ret = -1;
	assertTrue(sin_color_cycle(0, 0, 255, 255) == -1, "sin_color_cycle doesnt fail when given matching start and end values and 0 for a wavelength", &ret);
	assertTrue(sin_color_cycle(0, 0, 128, 255) == -1, "sin_color_cycle doesnt fail when given an invalid wavelength value.", &ret);
	assertTrue(sin_color_cycle(0, 1, 255, 255) == -1, "sin_color_cycle doesnt fail when given identical start and end values.", &ret);
	int i;
	for(i=0; i<100; i++){
		assertTrue(sin_color_cycle((float)i, 100, 0, 255) != -1, "sin_color_cycle fails when given a time value between 0 & 99.", &ret);
	}
	return ret;
}

int testGenSinCurveColorList(){
	int ret = -1;
	colorList* self = NULL;
	color c0 = {255, 255, 255, 255, 255};
	color c1 = {0, 0, 0, 0, 100};
	colorPair pair = {c0, c1};
	setUp(&self);
	//wavelength is in SECONDS, delay_ns is in NANOSECONDS
	
	//failure cases
	assertTrue(genSinCurveColorList(NULL, 0, pair, 0) == -1, "1", &ret);	// delay and wavelength are both zero
	assertTrue(genSinCurveColorList(NULL, 0, pair, 1) == -1, "2", &ret);	// delay is greater than wavelength
	assertTrue(genSinCurveColorList(NULL, 0, pair, -1) == -1, "3", &ret);	// delay is greater than wavelength
	
	assertTrue(genSinCurveColorList(NULL, 1, pair, 0) == -1, "4", &ret);	// delay is zero
	assertTrue(genSinCurveColorList(NULL, 1, pair, 1) == -1, "5", &ret);	// list is null
	assertTrue(genSinCurveColorList(NULL, 1, pair, -1) == -1, "6", &ret);	// delay > wavelength
	
	assertTrue(genSinCurveColorList(NULL, -1, pair, 0) == -1, "7", &ret);	// delay is zero
	assertTrue(genSinCurveColorList(NULL, -1, pair, 1) == -1, "8", &ret);	// list is null
	assertTrue(genSinCurveColorList(NULL, -1, pair, -1) == -1, "9", &ret);	// list is null
	
	assertTrue(genSinCurveColorList(self, 0, pair, 0) == -1, "10", &ret);	// delay is zero
	assertTrue(genSinCurveColorList(self, 0, pair, 1) == -1, "11", &ret);	// delay > wavelength
	assertTrue(genSinCurveColorList(self, 0, pair, -1) == -1, "12", &ret);	// delay > wavelength
	
	assertTrue(genSinCurveColorList(self, 1, pair, 0) == -1, "13", &ret);	// delay is zero
	
	assertTrue(genSinCurveColorList(self, 1, pair, -1) == -1, "14", &ret);
	
	assertTrue(genSinCurveColorList(self, -1, pair, 0) == -1, "15", &ret);
	assertTrue(genSinCurveColorList(self, -1, pair, 1) == -1, "16", &ret);
	assertTrue(genSinCurveColorList(self, -1, pair, -1) == -1, "17", &ret);
	
	// success case
	assertTrue(genSinCurveColorList(self, 1.0, pair, 1000000) == 0, "18", &ret);
	
	if(assertTrue(self!=NULL, "14", &ret)){
		
		// check the head and tail of the generated list
		if(assertTrue(self->head != NULL, "19", &ret)){
			if(assertTrue(self->head->data != NULL, "20", &ret)){
				assertTrue(((color*)(self->head->data))->red == c0.red, "21", &ret);
				assertTrue(((color*)(self->head->data))->green == c0.green, "22", &ret);
				assertTrue(((color*)(self->head->data))->blue == c0.blue, "23", &ret);
				assertTrue(((color*)(self->head->data))->brightness == c0.brightness, "24", &ret);
				assertTrue(((color*)(self->head->data))->delay == 1000000, "25", &ret);
			}
		}
		// check the tail
		if(assertTrue(self->tail != NULL, "26", &ret)){
			if(assertTrue(self->tail->data != NULL, "27", &ret)){
				assertTrue(((color*)(self->tail->data))->red == c1.red, "28", &ret);
				assertTrue(((color*)(self->tail->data))->green == c1.green, "29", &ret);
				assertTrue(((color*)(self->tail->data))->blue == c1.blue, "30", &ret);
				assertTrue(((color*)(self->tail->data))->brightness == c1.brightness, "31", &ret);
				assertTrue(((color*)(self->tail->data))->delay == 1, "32", &ret);
			}
		}
	}
	tearDown(&self);
	return ret;
}
int testGenerateFadeToColor(){
	int ret = -1;
	
	return ret;
}
