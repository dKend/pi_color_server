#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Color.h"
#include "ColorErrorDef.h"
#include "assertion.h"

#define validWavelength 1
#define validDelay 10
#define invalWavelength 0
#define invalDelay1 1000000000.0
#define invalDelay2 0
void setUp(struct colorList** self);
void tearDown(struct colorList** self);
int testInitColorList();
int testAddColorList();
int testFreeColorList();
int testPrintColorList();
int testSinColorCycle();
int testGenSinCurveColorList();
int testGenerateFadeToColor();
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
int testColorFade();
int testColorFade_Error();
int testGenerateFadeToColor();
int testGenerateFadeToColor_Error();

void setUp2(struct colorListQueue** self);
void tearDown2(struct colorListQueue** self);
void setUp2_OneElement(struct colorListQueue** self);
void setUp2_TwoElement(struct colorListQueue** self);
void setUp2_ManyElement(struct colorListQueue** self);
int testInitColorListQueue_Error();
int testIntiColorListQueue();
int testQueueColorListQueue_Error();
int testQueueColorListQueue();
int testDequeueColorListQueue_Error();
int testDequeueColorListQueue();
int testRequeueColorListQueue_Error();
int testRequeueColorListQueue();
int testFreeColorListQueue_Error();
int testFreeColorListQueue();



int main(){
	//	tests that have already been written and pass
	registerTest(testGetCERR, "testGetCERR");
	registerTest(testGetCERRMessage_Error, "testGetCERRMessage_Error");
	registerTest(testGetCERRName_Error, "testGetCERRName_Error");
	registerTest(testInitColorList_Error, "testInitColorList_Error");
	registerTest(testAddColorList_Error, "testAddColorList_Error");
	registerTest(testFreeColorList_Error, "testFreeColorList_Error");
	registerTest(testPrintColorList_Error, "testPrintColorList_Error");
	registerTest(testSinColorCycle_Error, "testSinColorCycle_Error");
	registerTest(testGenSinCurveColorList_Error, "testGenSinCurveColorList_Error");
	registerTest(testFreeColor_Error, "testFreeColor_Error");
	registerTest(testInitColorList, "testInitColorList");
	registerTest(testAddColorList, "testAddColorList");
	registerTest(testFreeColorList, "testFreeColorList");
	registerTest(testSinColorCycle, "testSinColorCycle");
	registerTest(testGenSinCurveColorList, "testGenSinCurveColorList");
	registerTest(testDuplicateColor_Error, "testDuplicateColor_Error");
	registerTest(testDuplicateColor, "testDuplicateColor");
	registerTest(testColorFade_Error, "testColorFade_Error");
	registerTest(testColorFade, "testColorFade");
	registerTest(testGenerateFadeToColor_Error, "testGenerateFadeToColor_Error");
	registerTest(testGenerateFadeToColor, "testGenerateFadeToColor");
	
	runTests();
	//destroyTests();
	return 0;
}

void setUp(colorList** self){
	*self = (colorList*)malloc(sizeof(colorList));
	(*self)->head = NULL;
	(*self)->tail = NULL;
	(*self)->nodeCount = 0;
}

void tearDown(colorList** self){
	if(self!=NULL){
		if(*self!=NULL){
			free_list(&((*self)->head));	//free_list bug found! 3/9/18 bug#00, bug#01, bug#02
			(*self)->head = NULL;
			(*self)->tail = NULL;
			free(*self);
		}
	}
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
		self->tail = NULL;
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
	assertEqualsInt(128, sin_color_cycle(0.0, 100.0, 128, 255), "1", &ret);
	assertEqualsInt(255, sin_color_cycle(50.0, 100.0, 128, 255), "2", &ret);
	assertEqualsInt(128, sin_color_cycle(100.0, 100.0, 128, 255), "3", &ret);
	
	assertEqualsInt(255, sin_color_cycle(0.0, 1.0, 255, 0), "4", &ret);
	assertEqualsInt(0, sin_color_cycle(0.5, 1.0, 255, 0), "5", &ret);
	assertEqualsInt(255, sin_color_cycle(1.0, 1.0, 255, 0), "6", &ret);
	return ret;
}

int testGenSinCurveColorList(){
	int ret = -1;
	colorList* self = NULL;
	colorList* self2 = NULL;
	color c0 = {255, 255, 255, 255, 255};
	color c1 = {0, 0, 0, 0, 100};
	colorPair pair = {c0, c1};
	colorPair pair2 = {c1, c0};
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
	
	assertTrue(self == NULL, "NullTest1", &ret);
	assertTrue(genSinCurveColorList(&self, 0, pair, 0) == -1, "10", &ret);	// delay is zero
	assertTrue(self == NULL, "NullTest2", &ret);
	assertTrue(genSinCurveColorList(&self, 0, pair, 1) == -1, "11", &ret);	// delay > wavelength
	assertTrue(self == NULL, "NullTest3", &ret);
	assertTrue(genSinCurveColorList(&self, 0, pair, -1) == -1, "12", &ret);	// delay > wavelength
	assertTrue(self == NULL, "NullTest4", &ret);
	assertTrue(genSinCurveColorList(&self, 1, pair, 0) == -1, "13", &ret);	// delay is zero
	assertTrue(self == NULL, "NullTest5", &ret);
	assertTrue(genSinCurveColorList(&self, 1, pair, -1) == -1, "14", &ret);
	assertTrue(self == NULL, "NullTest6", &ret);
	assertTrue(genSinCurveColorList(&self, -1, pair, 0) == -1, "15", &ret);
	assertTrue(self == NULL, "NullTest7", &ret);
	//assertTrue(genSinCurveColorList(&self, -1, pair, 1) == -1, "16", &ret);	//I think this is where it starts working...
	//assertTrue(self == NULL, "NullTest8", &ret);
	assertTrue(genSinCurveColorList(&self, -1, pair, -1) == -1, "17", &ret);
	assertTrue(self == NULL, "NullTest9", &ret);
	unsigned int a = -1;
	float test = (float)a;
	assertTrue(test == 4294967296, "numTest1", &ret);
	assertTrue(self == NULL, "NullTest10", &ret);
	// success case
	if(!assertTrue(genSinCurveColorList(&self, 1, pair, 100000000) == 0, "18", &ret)){
		char* message = NULL;
		getCERRMessage(&message);
		printf("\nCERR#: %d Message: %s\n", getCERR(), message);
		free(message);
	}
	printColorList(self);
	
	if(!assertTrue(genSinCurveColorList(&self2, 1, pair2, 100000000) == 0, "18.5", &ret)){
		char* message = NULL;
		getCERRMessage(&message);
		printf("\nCERR#: %d Message: %s\n", getCERR(), message);
		free(message);
	}
	printColorList(self2);
	
	if(assertTrue(self!=NULL, "19", &ret)){
		
		// check the head and tail of the generated list
		if(assertTrue(self->head != NULL, "19.5", &ret)){
			if(assertTrue(self->head->data != NULL, "20", &ret)){
				assertTrue(((color*)(self->head->data))->red == c0.red, "21", &ret);
				assertTrue(((color*)(self->head->data))->green == c0.green, "22", &ret);
				assertTrue(((color*)(self->head->data))->blue == c0.blue, "23", &ret);
				assertTrue(((color*)(self->head->data))->brightness == c0.brightness, "24", &ret);
				assertTrue(((color*)(self->head->data))->delay == 100000000, "25", &ret);
			}
		}
		// check the tail
		if(assertTrue(self->tail != NULL, "26", &ret)){
			if(assertTrue(self->tail->data != NULL, "27", &ret)){
				assertTrue(((color*)(self->tail->data))->red == c0.red, "28", &ret);
				assertTrue(((color*)(self->tail->data))->green == c0.green, "29", &ret);
				assertTrue(((color*)(self->tail->data))->blue == c0.blue, "30", &ret);
				assertTrue(((color*)(self->tail->data))->brightness == c0.brightness, "31", &ret);
				assertTrue(((color*)(self->tail->data))->delay == 100000000, "32", &ret);
			}
		}
	}
	tearDown(&self2);
	tearDown(&self);
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
	}
		
	assertTrue(initColorList(&self) == -1, "5", &ret);
	assertEqualsInt(14, getCERR(), "6", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "7", &ret)){
		assertEqualsInt(0, strcmp(message, "InitColorList received a bad pointer, self* cannot be previously initialized."), "8", &ret);
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
		freeColorList(&self);
	}
	assertEqualsInt(-1, genSinCurveColorList(&self, 0, pair, 1), "5", &ret);
	assertEqualsInt(1, getCERR(), "6", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "7", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSirCurveColorList received a bad wavelength value, wavelength must be greater than zero."), "8", &ret);
		free(message);
		message = NULL;
		freeColorList(&self);
	}
	
	// all cases for CERR_GenSinCurveColorList_BadDelay_Zero (error 2)
	assertEqualsInt(-1, genSinCurveColorList(&self, 1, pair, 0), "9", &ret);
	assertEqualsInt(2, getCERR(), "10", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "11", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSirCurveColorList received a bad delay value, delay must be greater than zero."), "12", &ret);
		free(message);
		message = NULL;
		
		freeColorList(&self);
	}
	
	// all cases for CERR_GenSinCurveColorList_BadDelay_NotLessThanWavelength (error 3)
	assertEqualsInt(-1, genSinCurveColorList(&self, 1, pair, 1000000000), "13", &ret);
	assertEqualsInt(3, getCERR(), "14", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "15", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad delay value, delay must be less than wavelength."), "16", &ret);
		free(message);
		message = NULL;
		freeColorList(&self);
	}
	assertEqualsInt(-1, genSinCurveColorList(&self, 1, pair, 2000000000), "17", &ret);
	assertEqualsInt(3, getCERR(), "18", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "19", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad delay value, delay must be less than wavelength."), "20", &ret);
		free(message);
		message = NULL;
		freeColorList(&self);
	}
	
	// all cases for CERR_GenSinCurveColorList_BadColorList_NULL (error 4)
	assertEqualsInt(-1, genSinCurveColorList(NULL, 0, pair, 0), "21", &ret);
	assertEqualsInt(4, getCERR(), "22", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "23", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be NULL."), "24", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(NULL, 0, pair, 1), "25", &ret);
	assertEqualsInt(4, getCERR(), "26", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "27", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be NULL."), "28", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(NULL, 1, pair, 0), "29", &ret);
	assertEqualsInt(4, getCERR(), "30", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "31", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be NULL."), "32", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(NULL, 1, pair, 1), "33", &ret);
	assertEqualsInt(4, getCERR(), "34", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "35", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be NULL."), "36", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(NULL, 1, pair, 2), "37", &ret);
	assertEqualsInt(4, getCERR(), "38", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "39", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be NULL."), "40", &ret);
		free(message);
		message = NULL;
	}
	
	// all cases for CERR_GenSinCurveColorList_BadColorList_AlreadyInit (error 5)
	assertEqualsInt(-1, genSinCurveColorList(&bself, 0, pair, 0), "41", &ret);
	assertEqualsInt(5, getCERR(), "42", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "43", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be initialized."), "44", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(&bself, 0, pair, 1), "45", &ret);
	assertEqualsInt(5, getCERR(), "46", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "47", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be initialized."), "48", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(&bself, 1, pair, 0), "49", &ret);
	assertEqualsInt(5, getCERR(), "50", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "51", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be initialized."), "52", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(&bself, 1, pair, 1), "53", &ret);
	assertEqualsInt(5, getCERR(), "54", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "55", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be initialized."), "56", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genSinCurveColorList(&bself, 1, pair, 2), "57", &ret);
	assertEqualsInt(5, getCERR(), "58", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "59", &ret)){
		assertEqualsInt(0, strcmp(message, "GenSinCurveColorList received a bad pointer, self cannot be initialized."), "60", &ret);
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
		assertEqualsInt(0, strcmp(message, "GetCERRMessage received a bad pointer, str cannot be NULL."), "4", &ret);
	}
	if(assertEqualsInt(-1, getCERRMessage(&message), "5", &ret)){
		assertEqualsInt(17, getCERR(), "6", &ret);
		free(message);
		message = NULL;
		if(assertTrue(getCERRMessage(&message) != -1, "7", &ret)){
			assertEqualsInt(0, strcmp(message, "GetCERRMessage received a bad pointer, *str must be NULL."), "8", &ret);
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
	
	// all cases for CERR_GetCERRName_BadPointer (error 19)
	assertEqualsInt(-1, getCERRName(NULL), "13", &ret);
	assertEqualsInt(19, getCERR(), "14", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "15", &ret)){
		assertEqualsInt(0, strcmp(message, "GetCERRName received a bad pointer, str cannot be NULL."), "16", &ret);
		free(message);
		message = NULL;
	}
	
	// all cases for CERR_GetCERRMessage_BadPointer_NonNULL (error 20)
	getCERRName(&name);	// fill name with garbo data
	assertEqualsInt(-1, getCERRName(&name), "9", &ret);
	assertEqualsInt(20, getCERR(), "10", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "11", &ret)){
		assertEqualsInt(0, strcmp(message, "GetCERRName received a bad pointer, *str must be NULL."), "12", &ret);
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

int testGenerateFadeToColor(){
	int ret = -1;
	color c0 = {0, 0, 0, 0, 0};
	color c1 = {255, 255, 255, 255, 255};
	colorPair pair = {c0, c1};		//low to high
	colorList* self = NULL;
	
	// test low to high functionality -- this is done by color_fade
	if(!assertEqualsInt(0, genFadeColorList(&self, 100, pair, 1000000000), "1", &ret)){
		char* message = NULL;
		getCERRMessage(&message);
		printf("\nCERRMessage: %s\n", message);
		fflush(stdout);
		free(message);
	}
	if(assertTrue(self!=NULL, "nullCheck1", &ret)){
		if(assertTrue(self->head != NULL, "nullCheck2", &ret) && assertTrue(self->tail != NULL, "nullCheck3", &ret)){
			// check step 0
			if(assertTrue(self->head->data!=NULL, "nullCheck4", &ret)){
				assertTrue(((color*)(self->head->data))->red == c0.red, "2", &ret);
				assertTrue(((color*)(self->head->data))->green == c0.green, "3", &ret);
				assertTrue(((color*)(self->head->data))->blue == c0.blue, "4", &ret);
				assertTrue(((color*)(self->head->data))->brightness == c0.brightness, "5", &ret);
				assertTrue(((color*)(self->head->data))->delay == 1000000000, "6", &ret);
			}
			
			// check step 100
			if(assertTrue(self->tail->data != NULL, "nullCheck7", &ret)){
				assertTrue(((color*)(self->tail->data))->red == c1.red, "12", &ret);
				assertTrue(((color*)(self->tail->data))->green == c1.green, "13", &ret);
				assertTrue(((color*)(self->tail->data))->blue == c1.blue, "14", &ret);
				assertTrue(((color*)(self->tail->data))->brightness == c1.brightness, "15", &ret);
				assertTrue(((color*)(self->tail->data))->delay == 1000000000, "16", &ret);
			}
		}
		assertEqualsInt(101, self->nodeCount, "17", &ret);
	}
	tearDown(&self);
	return ret;
}

int testGenerateFadeToColor_Error(){
	int ret = -1;
	char* message = NULL;
	colorList* self = NULL;
	colorList* self2 = NULL;
	color c0 = {0, 0, 0, 0, 0};
	color c1 = {255, 255, 255, 255, 255};
	colorPair pair = {c0, c1};
	setUp(&self);
	
	/*
	*	Test that checking for NULL happens first.
	*	CERR_GenFadeColorList_SelfEqNULL error number 27
	*/
	assertEqualsInt(-1, genFadeColorList(NULL, validWavelength, pair, validDelay), "1", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfEqNULL, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfEqNULL"), "4", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(NULL, validWavelength, pair, invalDelay1), "5", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfEqNULL, getCERR(), "6", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "7", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfEqNULL"), "8", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(NULL, validWavelength, pair, invalDelay2), "9", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfEqNULL, getCERR(), "10", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "11", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfEqNULL"), "12", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(NULL, invalWavelength, pair, validDelay), "13", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfEqNULL, getCERR(), "14", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "15", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfEqNULL"), "16", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(NULL, invalWavelength, pair, invalDelay1), "17", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfEqNULL, getCERR(), "18", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "19", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfEqNULL"), "20", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(NULL, invalWavelength, pair, invalDelay2), "21", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfEqNULL, getCERR(), "22", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "23", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfEqNULL"), "24", &ret);
		free(message);
		message = NULL;
	}
	
	/*	
	*	Test that checking if the list is already initialized next.
	*	CERR_GenFadeColorList_SelfInit error number 28.
	*/
	assertEqualsInt(-1, genFadeColorList(&self, validWavelength, pair, validDelay), "25", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfInit, getCERR(), "26", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "27", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfInit"), "28", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(&self, validWavelength, pair, invalDelay1), "29", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfInit, getCERR(), "30", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "31", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfInit"), "32", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(&self, validWavelength, pair, invalDelay2), "33", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfInit, getCERR(), "34", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "35", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfInit"), "36", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(&self, invalWavelength, pair, validDelay), "37", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfInit, getCERR(), "38", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "39", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfInit"), "40", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(&self, invalWavelength, pair, invalDelay1), "41", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfInit, getCERR(), "42", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "43", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfInit"), "44", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(&self, invalWavelength, pair, invalDelay2), "45", &ret);
	assertEqualsInt(CERR_GenFadeColorList_SelfInit, getCERR(), "46", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "47", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_SelfInit"), "48", &ret);
		free(message);
		message = NULL;
	}
	
	/*
	*	Test checking that wavelength is greater than zero.
	*	CERR_GenFadeColorList_WavelengthZero
	*/
	assertEqualsInt(-1, genFadeColorList(&self2, invalWavelength, pair, validDelay), "49", &ret);
	assertEqualsInt(CERR_GenFadeColorList_WavelengthZero, getCERR(), "50", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "51", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_WavelengthZero"), "52", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(&self2, invalWavelength, pair, invalDelay1), "53", &ret);
	assertEqualsInt(CERR_GenFadeColorList_WavelengthZero, getCERR(), "54", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "55", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_WavelengthZero"), "56", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, genFadeColorList(&self2, invalWavelength, pair, invalDelay2), "57", &ret);
	assertEqualsInt(CERR_GenFadeColorList_WavelengthZero, getCERR(), "58", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "59", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_WavelengthZero"), "60", &ret);
		free(message);
		message = NULL;
	}
	
	/*
	*	Test that checking that delay > 0 occurs.
	*	CERR_GenFadeColorList_DelayZero
	*/
	assertEqualsInt(-1, genFadeColorList(&self2, validWavelength, pair, invalDelay2), "61", &ret);
	assertEqualsInt(CERR_GenFadeColorList_DelayZero, getCERR(), "62", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "63", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_DelayZero"), "64", &ret);
		free(message);
		message = NULL;
	}
	
	/*
	*	Test that checking that delay < wavelength occurs.
	*	CERR_GenFadeColorList_DelayGtWavelength
	*/
	assertEqualsInt(-1, genFadeColorList(&self2, validWavelength, pair, invalDelay1), "65", &ret);
	assertEqualsInt(CERR_GenFadeColorList_DelayGtWavelength, getCERR(), "66", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "67", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_GenFadeColorList_DelayGtWavelength"), "68", &ret);
		free(message);
		message = NULL;
	}
	
	tearDown(&self);
	return ret;
}

int testColorFade(){
	int ret = -1;
	assertEqualsInt(0, color_fade(0, 100, 0, 255), "1", &ret);
	assertEqualsInt(127, color_fade(50, 100, 0, 255), "2", &ret);
	assertEqualsInt(255, color_fade(100, 100, 0, 255), "3", &ret);
	assertEqualsInt(255, color_fade(0, 100, 255, 0), "4", &ret);
	assertEqualsInt(127, color_fade(50, 100, 255, 0), "5", &ret);
	assertEqualsInt(0, color_fade(100, 100, 255, 0), "6", &ret);
	return ret;
}

int testColorFade_Error(){
	int ret = -1;
	char* message = NULL;
	assertEqualsInt(-1, color_fade(0, 0, 0, 0), "1", &ret);
	assertEqualsInt(31, getCERR(), "2", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "3", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_ColorFade_WavelengthEqZero"), "4", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, color_fade(1, 0, 0, 0), "5", &ret);
	assertEqualsInt(29, getCERR(), "6", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "7", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_ColorFade_TimeGtWavelength"), "8", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, color_fade(-1, 0, 0, 0), "9", &ret);
	assertEqualsInt(29, getCERR(), "10", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "11", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_ColorFade_TimeGtWavelength"), "12", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, color_fade(0, 1, 0, 0), "13", &ret);
	assertEqualsInt(30, getCERR(), "14", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "15", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_ColorFade_StartEqEnd"), "16", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, color_fade(-1, 1, 0, 0), "17", &ret);
	assertEqualsInt(30, getCERR(), "18", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "19", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_ColorFade_StartEqEnd"), "20", &ret);
		free(message);
		message = NULL;
	}
	assertEqualsInt(-1, color_fade(1, 1, 0, 0), "21", &ret);
	assertEqualsInt(30, getCERR(), "22", &ret);
	if(assertTrue(getCERRMessage(&message) != -1, "23", &ret)){
		assertEqualsInt(0, strcmp(message, "[PH]CERR_ColorFade_StartEqEnd"), "24", &ret);
		free(message);
		message = NULL;
	}
	return ret;
}

//	Color Test 2: Electric Boog-a-Queue OwO
void setUp2(struct colorListQueue** self){
	(*self) = (colorListQueue*)malloc(sizeof(colorListQueue));
	(*self)->head = NULL;
	(*self)->tail = NULL;
	(*self)->nodeCount = 0;
}
void setUp2_OneElement(struct colorListQueue** self){
	setUp2(self);
	// create a node and some data
	node* n = NULL;
	colorList* l = NULL;
	color c0 = {255, 255, 255, 255, 1000};
	color c1 = {0, 0, 0, 0, 12};
	color c2 = {1, 1, 1, 1, 500};
	initColorList(&l);
	addColorList(l, &c0);
	addColorList(l, &c1);
	addColorList(l, &c2);
	colorListQueueData* d = (colorListQueueData*)malloc(sizeof(colorListQueueData));
	d->list = l;
	d->current = l->head;
	d->timesRepeated = 0;
	d->msPassedSinceNodeChange = 0;
	d->repeat_f = 0;
	d->prior_f = 0;
	d->reque_f = 0;
	create_node(&n, l, NULL, freeColorListQueueData);
	(*self)->head = n;
	(*self)->tail = n;
	(*self)->nodeCount = 1;
}
void setUp2_TwoElement(struct colorListQueue** self){
	
}
void setUp2_ManyElement(struct colorListQueue** self){
	
}
void tearDown2(struct colorListQueue** self){
	
}
int testInitColorListQueue_Error(){
	int ret = -1;
	
	return ret;
}
int testIntiColorListQueue(){
	int ret = -1;
	
	return ret;
}
int testQueueColorListQueue_Error(){
	int ret = -1;
	
	return ret;
}
int testQueueColorListQueue(){
	int ret = -1;
	
	return ret;
}
int testDequeueColorListQueue_Error(){
	int ret = -1;
	
	return ret;
}
int testDequeueColorListQueue(){
	int ret = -1;
	
	return ret;
}
int testRequeueColorListQueue_Error(){
	int ret = -1;
	
	return ret;
}
int testRequeueColorListQueue(){
	int ret = -1;
	
	return ret;
}
int testFreeColorListQueue_Error(){
	int ret = -1;
	
	return ret;
}
int testFreeColorListQueue(){
	int ret = -1;
	
	return ret;
}
