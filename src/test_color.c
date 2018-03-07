#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int testGetCERR();
int testInitColorList_Error();
int testAddColorList_Error();
int testFreeColorList_Error();
int testPrintColorList_Error();
int testSinColorCycle_Error();
int testGenSinCurveColorList_Error();
int testFreeColor_Error();
int testGetCERRMessage_Error();
int testGetCERRName_Error();
int testDuplicateColor();
int testDuplicateColor_Error();

int main(){
	registerTest(testGetCERR, "testGetCERR");
	registerTest(testInitColorList_Error, "testInitColorList_Error");
	registerTest(testAddColorList_Error, "testAddColorList_Error");
	registerTest(testFreeColorList_Error, "testFreeColorList_Error");
	registerTest(testPrintColorList_Error, "testPrintColorList_Error");
	registerTest(testSinColorCycle_Error, "testSinColorCycle_Error");
	registerTest(testGenSinCurveColorList_Error, "testGenSinCurveColorList_Error");
	registerTest(testFreeColor_Error, "testFreeColor_Error");
	registerTest(testGetCERRMessage_Error, "testGetCERRMessage_Error");
	registerTest(testGetCERRName_Error, "testGetCERRName_Error");
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
	free_list(&((*self)->head));
	(*self)->head = NULL;
	(*self)->tail = NULL;
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
	color c0 = {255, 255, 255, 255, 1000};
	color c1 = {0, 0, 0, 0, 12};
	color c2 = {1, 1, 1, 1, 500};
	setUp(&self);
	//adding to empty list
	assertTrue(addColorList(NULL, NULL) == -1, "addColorList doesnt fail when given 2 NULL pointers.", &ret);
	assertTrue(addColorList(self, NULL) == -1, "addColorList doesnt fail when give a NULL pointer for color.", &ret);
	assertTrue(addColorList(NULL, &c0) == -1, "addColorList doesnt fail when given a NULL pointer for list.", &ret);
	assertTrue(addColorList(self, &c0) == 0, "addColorList fails when given valid arguments.", &ret);
	if(assertTrue(self->head != NULL, "addColorList fails to add a new node to the color list upon success being returned with an empty list.", &ret) == 1){
		assertTrue(self->head->next == NULL, "addColorList fails to set new node's next pointer to null when list is empty.", &ret);
		if(assertTrue(self->head->data != NULL, "addColorList fails to set the color within the colorNode when adding to an empty list.", &ret) == 1){
			assertEqualsInt(c0.red, ((color*)(self->head->data))->red, "addColorList fails to set the color of new node to passed color0.", &ret);
			assertEqualsInt(c0.green, ((color*)(self->head->data))->green, "addColorList fails to set the color of new node to passed color1.", &ret);
			assertEqualsInt(c0.blue, ((color*)(self->head->data))->blue, "addColorList fails to set the color of new node to passed color2.", &ret);
			assertEqualsInt(c0.brightness, ((color*)(self->head->data))->brightness, "addColorList fails to set the color of new node to passed color3.", &ret);
			assertEqualsInt(c0.delay, ((color*)(self->head->data))->delay, "addColorList fails to set the color of new node to passed color4.", &ret);
		}
	}
	if(assertTrue(self->tail != NULL, "addColorList fails to set tail to the new node when adding a node to an emty list.", &ret) == 1){
		assertTrue(self->tail == self->head, "addColorList fails to set tail to the same address as head when adding to an empty list.", &ret);
	}
	//adding to single node list
	
	assertTrue(addColorList(self, NULL) == -1, "addColorList doesnt fail when give a NULL pointer for color where self is a single ndoe list.", &ret);
	assertTrue(addColorList(self, &c1) == 0, "addColorList fails when given valid arguments where self is a single node list.", &ret);
	if(assertTrue(self->head != NULL && self->tail != NULL, "addColorList changes head/tail to null unexpectedly.", &ret) == 1){
		assertTrue(self->head->next == self->tail, "addColorList fails to set the next pointer of head to the new tail node when self is a single node list.", &ret);
		assertTrue(self->tail->next == NULL, "addColorList fails to set the next node of tail to null when self is a single node list.", &ret);
		if(assertTrue(self->tail->data != NULL && self->head->data != NULL, "addColorList fails to set the color within the colorNode when adding to an empty list OR head's data pointer is NULL.", &ret) == 1){
			assertEqualsInt(c1.red, ((color*)(self->tail->data))->red, "addColorList fails to set the color of new node to passed color5.", &ret);
			assertEqualsInt(c1.green, ((color*)(self->tail->data))->green, "addColorList fails to set the color of new node to passed color6.", &ret);
			assertEqualsInt(c1.blue, ((color*)(self->tail->data))->blue, "addColorList fails to set the color of new node to passed color7.", &ret);
			assertEqualsInt(c1.brightness, ((color*)(self->tail->data))->brightness, "addColorList fails to set the color of new node to passed color8.", &ret);
			assertEqualsInt(c1.delay, ((color*)(self->tail->data))->delay, "addColorList fails to set the color of new node to passed color9.", &ret);
			
			assertEqualsInt(c0.red, ((color*)(self->head->data))->red, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0.green, ((color*)(self->head->data))->green, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0.blue, ((color*)(self->head->data))->blue, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0.brightness, ((color*)(self->head->data))->brightness, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0.delay, ((color*)(self->head->data))->delay, "addColorList fails to retain the data within the head node.", &ret);
		}
	}
	if(assertTrue(self->tail != NULL, "addColorList fails to set tail to the new node when adding a node to a single node list.", &ret) == 1){
		assertTrue(self->tail != self->head, "addColorList sets tail to the same address as head when adding to a single node list.", &ret);
	}
	//adding to a n node list
	assertTrue(addColorList(self, NULL) == -1, "addColorList doesnt fail when give a NULL pointer for color where self is a n ndoe list.", &ret);
	assertTrue(addColorList(self, &c2) == 0, "addColorList fails when given valid arguments where self is a n node list.", &ret);
	if(assertTrue(self->head != NULL && self->tail != NULL, "addColorList changes head/tail to null unexpectedly.", &ret) == 1){
		assertTrue(self->head->next != self->tail, "addColorList fails to change the tail pointer to the new node when self is a n node list.", &ret);
		assertTrue(self->tail->next == NULL, "addColorList fails to set the next node of tail to null when self is a n node list.", &ret);
		if(assertTrue(self->tail->data != NULL && self->head->data != NULL, "addColorList fails to set the color within the colorNode when adding to an empty list OR head's data pointer is NULL.", &ret) == 1){
			assertEqualsInt(c2.red, ((color*)(self->tail->data))->red, "addColorList fails to set the color of new node to passed color10.", &ret);
			assertEqualsInt(c2.green, ((color*)(self->tail->data))->green, "addColorList fails to set the color of new node to passed color11.", &ret);
			assertEqualsInt(c2.blue, ((color*)(self->tail->data))->blue, "addColorList fails to set the color of new node to passed color12.", &ret);
			assertEqualsInt(c2.brightness, ((color*)(self->tail->data))->brightness, "addColorList fails to set the color of new node to passed color13.", &ret);
			assertEqualsInt(c2.delay, ((color*)(self->tail->data))->delay, "addColorList fails to set the color of new node to passed color14.", &ret);
			
			if(assertTrue(self->head->next != NULL, "next is null.", &ret)){
				if(assertTrue(self->head->next->data != NULL, "data is null.", &ret)){
					assertEqualsInt(c1.red, ((color*)(self->head->next->data))->red, "addColorList fails to set the color of new node to passed color15.", &ret);
					assertEqualsInt(c1.green, ((color*)(self->head->next->data))->green, "addColorList fails to set the color of new node to passed color16.", &ret);
					assertEqualsInt(c1.blue, ((color*)(self->head->next->data))->blue, "addColorList fails to set the color of new node to passed color17.", &ret);
					assertEqualsInt(c1.brightness, ((color*)(self->head->next->data))->brightness, "addColorList fails to set the color of new node to passed color18.", &ret);
					assertEqualsInt(c1.delay, ((color*)(self->head->next->data))->delay, "addColorList fails to set the color of new node to passed color19.", &ret);
				}
			}
			assertEqualsInt(c0.red, ((color*)(self->head->data))->red, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0.green, ((color*)(self->head->data))->green, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0.blue, ((color*)(self->head->data))->blue, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0.brightness, ((color*)(self->head->data))->brightness, "addColorList fails to retain the data within the head node.", &ret);
			assertEqualsInt(c0.delay, ((color*)(self->head->data))->delay, "addColorList fails to retain the data within the head node.", &ret);
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
	color c0 = {255, 255, 255, 255, 1000};
	color c1 = {0, 0, 0, 0, 12};
	color c2 = {1, 1, 1, 1, 500};
	
	assertTrue(freeColorList(NULL) == -1, "freeColorList doesnt fail when given NULL.", &ret);
	assertTrue(freeColorList(&self) == -1, "freeColorList doesnt fail when given an uninitialized list.", &ret);
	// first setUp call
	setUp(&self);
	assertTrue(freeColorList(&self) == 0, "freeColorList fails when given a valid list and list is empty.", &ret);
	if(assertTrue(self == NULL, "freeColorList doesnt set given pointer to NULL on success and list is empty.", &ret)){
		// second call, should only be called if the last assert succeded
		setUp(&self);
		addColorList(self, &c0);
		assertTrue(freeColorList(&self) == 0, "freeColorList fails when given a valid list and list has one element.", &ret);
		if(assertTrue(self == NULL, "freeColorList doesnt set given pointer to NULL on success and list has one element.", &ret)){
			// third setup call
			setUp(&self);
			addColorList(self, &c0);
			addColorList(self, &c1);
			addColorList(self, &c2);
			assertTrue(freeColorList(&self) == 0, "freeColorList fails with valid list.", &ret);
			if(!assertTrue(self == NULL, "freeColorList doesnt set given pointer to NULL when its a valid list.", &ret)){
				tearDown(&self);
			}
		}else{
			tearDown(&self);
		}
	}else{
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
	
	assertTrue(genSinCurveColorList(&self, 0, pair, 0) == -1, "10", &ret);	// delay is zero
	assertTrue(genSinCurveColorList(&self, 0, pair, 1) == -1, "11", &ret);	// delay > wavelength
	assertTrue(genSinCurveColorList(&self, 0, pair, -1) == -1, "12", &ret);	// delay > wavelength
	
	assertTrue(genSinCurveColorList(&self, 1, pair, 0) == -1, "13", &ret);	// delay is zero
	
	assertTrue(genSinCurveColorList(&self, 1, pair, -1) == -1, "14", &ret);
	
	assertTrue(genSinCurveColorList(&self, -1, pair, 0) == -1, "15", &ret);
	assertTrue(genSinCurveColorList(&self, -1, pair, 1) == -1, "16", &ret);
	assertTrue(genSinCurveColorList(&self, -1, pair, -1) == -1, "17", &ret);
	
	// success case
	assertTrue(genSinCurveColorList(&self, 1.0, pair, 1000000) == 0, "18", &ret);
	
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

int testGetCERR(){
	int ret = -1;
	//ensure that the error is set to 0 at the start of testing
	assertTrue(getCERR() == 0, "1", &ret);
	//not really anything else we can test without assuming that another function properly set the CERR value on failure.
	return ret;
}

int testInitColorList_Error(){
	int ret = -1;
	colorList* self = NULL;
	char* message = NULL;
	
	setUp(&self);
	// have to assume that getCERR is working for this test.
	assertTrue(initColorList(NULL) == -1, "1", &ret);
	assertEqualsInt(13, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "InitColorList received a bad pointer, self cannot be NULL."), "4", &ret);
		free(message);
		message = NULL;
	}else{
		printf("cerr: %d\n", getCERR());
		fflush(stdout);
	}
		
	assertTrue(initColorList(&self) == -1, "4", &ret);
	assertEqualsInt(14, getCERR(), "5", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "6", &ret)){
		assertEqualsInt(0, strcmp(message, "InitColorList received a bad pointer, self* cannot be previously initialized."), "7", &ret);
		free(message);
		message = NULL;
	}
	tearDown(&self);
	return ret;
}

int testAddColorList_Error(){
	int ret = -1;
	colorList* self = NULL;
	char* message = NULL;
	color c = {25, 5, 255, 255, 0};
	setUp(&self);
	assertEqualsInt(-1, addColorList(NULL, NULL), "1", &ret);
	assertEqualsInt(11, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "AddColorList received a bad pointer, self cannot be NULL."), "4", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, addColorList(NULL, &c), "5", &ret);
	assertEqualsInt(11, getCERR(), "6", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "7", &ret)){
		assertEqualsInt(0, strcmp(message, "AddColorList received a bad pointer, self cannot be NULL."), "8", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, addColorList(self, NULL), "9", &ret);
	assertEqualsInt(12, getCERR(), "10", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "11", &ret)){
		assertEqualsInt(0, strcmp(message, "AddColorList received a bad pointer, n cannot be NULL."), "8", &ret);
		free(message);
		message = NULL;
	}
	tearDown(&self);
	return ret;
}

int testFreeColorList_Error(){
	int ret = -1;
	colorList* self = NULL;
	char* message = NULL;
	assertEqualsInt(-1, freeColorList(NULL), "1", &ret);
	assertEqualsInt(9, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "FreeColorList received a bad pointer, self cannot be NULL."), "4", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, freeColorList(&self), "4", &ret);
	assertEqualsInt(10, getCERR(), "5", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "FreeColorList received a bad pointer, self cannot point to NULL."), "4", &ret);
		free(message);
		message = NULL;
	}
	return ret;
}

int testPrintColorList_Error(){
	int ret = -1;
	char* message = NULL;
	assertEqualsInt(-1, printColorList(NULL), "1", &ret);
	assertEqualsInt(8, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "PrintColorList received a bad pointer, self cannot be NULL."), "4", &ret);
		free(message);
		message = NULL;
	}
	return ret;
}

int testSinColorCycle_Error(){
	int ret = -1;
	char* message = NULL;
	assertEqualsInt(-1, sin_color_cycle(0, 0, 1, 1), "1", &ret);
	assertEqualsInt(7, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "SinColorCycle received a bad wavelength value, wavelength cannot be zero."), "4", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, sin_color_cycle(0, 0, 1, 0), "4", &ret);
	assertEqualsInt(7, getCERR(), "5", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "6", &ret)){
		assertEqualsInt(0, strcmp(message, "SinColorCycle received a bad wavelength value, wavelength cannot be zero."), "7", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, sin_color_cycle(0, 1, 1, 1), "8", &ret);
	assertEqualsInt(6, getCERR(), "9", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "10", &ret)){
		assertEqualsInt(0, strcmp(message, "SinColorCycle received bad start and end values, start cannot equal end."), "11", &ret);
		free(message);
		message = NULL;
	}
	return ret;
}

int testGenSinCurveColorList_Error(){
	int ret = -1;
	colorList* self = NULL;
	colorList* bself = NULL;
	color c1 = {1, 2, 3, 4, 5};
	color c2 = {6, 7, 8, 9, 10};
	colorPair pair = {c1, c2};
	char* message = NULL;
	setUp(&bself);
	// all cases for CERR_GenSinCurveColorList_BadWavelength_Zero (error 1).
	assertEqualsInt(-1, genSinCurveColorList(&self, 0, pair, 0), "1", &ret);
	assertEqualsInt(1, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSirCurveColorList received a bad wavelength value, wavelength must be greater than zero."), "4", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(&self, 0, pair, 1), "5", &ret);
	assertEqualsInt(1, getCERR(), "6", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "7", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSirCurveColorList received a bad wavelength value, wavelength must be greater than zero."), "8", &ret);
		free(message);
		message = NULL;
	}
	
	// all cases for CERR_GenSinCurveColorList_BadDelay_Zero (error 2)
	assertEqualsInt(-1, genSinCurveColorList(&self, 1, pair, 0), "13", &ret);
	assertEqualsInt(2, getCERR(), "14", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "15", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSirCurveColorList received a bad delay value, delay must be greater than zero."), "16", &ret);
		free(message);
		message = NULL;
	}
	
	// all cases for CERR_GenSinCurveColorList_BadDelay_NotLessThanWavelength (error 3)
	assertEqualsInt(-1, genSinCurveColorList(&self, 1, pair, 1), "17", &ret);
	assertEqualsInt(3, getCERR(), "18", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "19", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad delay value, delay must be less than wavelength."), "20", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(&self, 1, pair, 2), "21", &ret);
	assertEqualsInt(3, getCERR(), "22", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "23", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad delay value, delay must be less than wavelength."), "24", &ret);
		free(message);
		message = NULL;
	}
	
	// all cases for CERR_GenSinCurveColorList_BadColorList_NULL (error 4)
	assertEqualsInt(-1, genSinCurveColorList(NULL, 0, pair, 0), "25", &ret);
	assertEqualsInt(4, getCERR(), "26", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "27", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be NULL."), "28", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(NULL, 0, pair, 1), "29", &ret);
	assertEqualsInt(4, getCERR(), "30", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "31", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be NULL."), "32", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(NULL, 1, pair, 0), "33", &ret);
	assertEqualsInt(4, getCERR(), "34", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "35", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be NULL."), "36", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(NULL, 1, pair, 1), "37", &ret);
	assertEqualsInt(4, getCERR(), "38", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "39", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be NULL."), "40", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(NULL, 1, pair, 2), "41", &ret);
	assertEqualsInt(4, getCERR(), "42", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "43", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be NULL."), "44", &ret);
		free(message);
		message = NULL;
	}
	
	// all cases for CERR_GenSinCurveColorList_BadColorList_AlreadyInit (error 5)
	assertEqualsInt(-1, genSinCurveColorList(&bself, 0, pair, 0), "45", &ret);
	assertEqualsInt(5, getCERR(), "46", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "47", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be initialized."), "48", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(&bself, 0, pair, 1), "49", &ret);
	assertEqualsInt(5, getCERR(), "50", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "51", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be initialized."), "52", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(&bself, 1, pair, 0), "53", &ret);
	assertEqualsInt(5, getCERR(), "54", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "55", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be initialized."), "56", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(&bself, 1, pair, 1), "57", &ret);
	assertEqualsInt(5, getCERR(), "58", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "59", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be initialized."), "60", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(&bself, 1, pair, 2), "61", &ret);
	assertEqualsInt(5, getCERR(), "62", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "63", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be initialized."), "64", &ret);
		free(message);
		message = NULL;
	}
	
	tearDown(&bself);
	return ret;
}

int testFreeColor_Error(){
	int ret = -1;
	char* message = NULL;
	assertEqualsInt(-1, freeColor(NULL), "1", &ret);
	assertEqualsInt(15, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "FreeColor received a bad pointer, self cannot be NULL."), "4", &ret);
		free(message);
		message = NULL;
	}
	return ret;
}

int testGetCERRMessage_Error(){
	int ret = -1;
	char* message = NULL;
	assertEqualsInt(-1, getCERRMessage(NULL), "1", &ret);	// 3/5/18, segmentation fault in this function call.
	assertEqualsInt(16, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "CERR_GetCERRMessage_BadPointer"), "4", &ret);
	}
	if(assertEqualsInt(-1, getCERRMessage(&message), "5", &ret)){
		assertEqualsInt(17, getCERR(), "6", &ret);
		free(message);
		message = NULL;
		if(assertTrue(getCERRMessage(&message) != -1, "7", &ret)){
			assertEqualsInt(0, strcmp(message, "GetCERRName received a bad pointer, str cannot be NULL."), "8", &ret);
			free(message);
			message = NULL;
		}
	}
	
	return ret;
}

int testGetCERRName_Error(){
	int ret = -1;
	char* message = NULL;
	char* name = NULL;
	
	// all cases for CERR_GetCERRName_BadE (error 18)
	assertEqualsInt(-1, getCERRName(24, NULL), "1", &ret);
	assertEqualsInt(18, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "GetCERRName received a bad e value, e is not a valid cerror number."), "4", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, getCERRName(24, &name), "5", &ret);
	assertEqualsInt(18, getCERR(), "6", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "7", &ret)){
		assertEqualsInt(0, strcmp(message, "GetCERRName received a bad e value, e is not a valid cerror number."), "8", &ret);
		free(message);
		message = NULL;
	}
	getCERRName(0, &name);
	assertEqualsInt(-1, getCERRName(24, &name), "9", &ret);
	assertEqualsInt(18, getCERR(), "10", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "11", &ret)){
		assertEqualsInt(0, strcmp(message, "GetCERRName received a bad e value, e is not a valid cerror number."), "12", &ret);
		free(message);
		message = NULL;
	}
	
	// all cases for CERR_GetCERRName_BadPointer (error 19)
	assertEqualsInt(-1, getCERRName(0, NULL), "13", &ret);
	assertEqualsInt(19, getCERR(), "14", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "15", &ret)){
		assertEqualsInt(0, strcmp(message, "GetCERRName received a bad pointer, str cannot be NULL."), "16", &ret);
		free(message);
		message = NULL;
	}
	
	// all cases for CERR_GetCERRName_BadPointer_NonNULL (error 20)
	assertEqualsInt(-1, getCERRName(0, &name), "13", &ret);
	assertEqualsInt(20, getCERR(), "14", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "15", &ret)){
		assertEqualsInt(0, strcmp(message, "GetCERRName received a bad pointer, *str must be NULL."), "16", &ret);
		free(message);
		message = NULL;
	}
	
	return ret;
}
int testDuplicateColor_Error(){
	int ret = -1;
	color c0 = {255, 128, 64, 32, 16};
	color* c1 = NULL;
	color* c2 = &c0;
	char* message = NULL;
	assertEqualsInt(-1, duplicateColor(NULL, NULL), "1", &ret);
	assertEqualsInt(21, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "DuplicateColor received a bad pointer, src cannot be NULL."), "4", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, duplicateColor(NULL, &c2), "5", &ret);
	assertEqualsInt(21, getCERR(), "6", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "7", &ret)){
		assertEqualsInt(0, strcmp(message, "DuplicateColor received a bad pointer, src cannot be NULL."), "8", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, duplicateColor(NULL, &c1), "9", &ret);
	assertEqualsInt(21, getCERR(), "10", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "11", &ret)){
		assertEqualsInt(0, strcmp(message, "DuplicateColor received a bad pointer, src cannot be NULL."), "12", &ret);
		free(message);
		message = NULL;
	}
	
	assertEqualsInt(-1, duplicateColor(&c0, NULL), "13", &ret);
	assertEqualsInt(22, getCERR(), "14", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "15", &ret)){
		assertEqualsInt(0, strcmp(message, "DuplicateColor received a bad pointer, dest cannot be NULL."), "16", &ret);
		free(message);
		message = NULL;
	}
	
	assertEqualsInt(-1, duplicateColor(&c0, &c2), "17", &ret);
	assertEqualsInt(23, getCERR(), "18", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "19", &ret)){
		assertEqualsInt(0, strcmp(message, "DuplicateColor received a bad pointer, *dest must be NULL."), "20", &ret);
		free(message);
		message = NULL;
	}
	
	
	return ret;
}
int testDuplicateColor(){
	int ret = -1;
	color c0 = {255, 128, 64, 32, 16};
	color* c1 = NULL;
	if(assertEqualsInt(0, duplicateColor(&c0, &c1), "1", &ret)){
		assertEqualsInt(255, c1->red, "2", &ret);
		assertEqualsInt(128, c1->green, "3", &ret);
		assertEqualsInt(64, c1->blue, "4", &ret);
		assertEqualsInt(32, c1->brightness, "5", &ret);
		assertEqualsInt(16, c1->delay, "6", &ret);
		free(c1);
	}
	
	return ret;
}
