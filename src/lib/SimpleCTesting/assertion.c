#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assertion.h"
#define MAX_TESTS 256

static fPair *tests[MAX_TESTS];
static int nextIndex = 0;

int assertTrue(bool statement, const char* error, int* val){
	if(*val == 0){
		printf("#");
		fflush(stdout);
		return 0;
	}
	if(statement){
		printf(".");
		fflush(stdout);
		*val = 1;
		return 1;
	}
	printf("\n!: %s\n", error);
	fflush(stdout);
	*val = 0;
	return 0;
}

int assertFalse(bool statement, const char* error, int* val){
	return assertTrue(!statement, error, val);
}

int assertEqualsInt(int expected, int actual, const char* error, int* val){
	char fullError[4096];
	//possible overflow!
	sprintf(fullError, "Error: expected \"%d\", got \"%d\"\nMessage: %s", expected, actual, error);
	
	return assertTrue(expected == actual, fullError, val);
}

int registerTest(int (*f)(), const char* str){
	int ret = -1;
	if(f != NULL && str != NULL && nextIndex < MAX_TESTS){
		tests[nextIndex] = (fPair*)malloc(sizeof(fPair));
		tests[nextIndex]->name = strdup(str);
		tests[nextIndex]->f = f;
		nextIndex++;
	}
	return ret;
}

int runTests(){
	int ret = 0;
	if(nextIndex > 0){
		int i = 0;
		while(i < nextIndex && i < MAX_TESTS){
			printf("Running test: %s\n", tests[i]->name);
			fflush(stdout);
			int passes = (tests[i]->f)();
			printf("\n");
			if(passes == 1){
				printf("%s Passed\n", tests[i]->name);
				fflush(stdout);
			}else{
				printf("%s Failed\n", tests[i]->name);
				fflush(stdout);
				ret = -1;
			}
			fflush(stdout);
			i++;
		}
		
	}
	destroyTests();
	return ret;
}

int destroyTests(){
	int ret = -1;
	if(nextIndex > 0){
		int i = 0;
		while(i < nextIndex && i < MAX_TESTS){
			//free(tests[i]->name);
			free(tests[i]);
			i++;
		}
	}
	return ret;
}