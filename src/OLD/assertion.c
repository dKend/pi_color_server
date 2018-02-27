#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assertion.h"

int assertTrue(bool statement, const char* error, int* val){
	if(*val == 0){
		printf("AssertSkipped\n");
		return 0;
	}
	if(statement){
		printf("AssertPass\n");
		*val = 1;
		return 1;
	}
	printf("AssertFail: %s\n", error);
	*val = 0;
	return 0;
}
int assertFalse(bool statement, const char* error, int* val){
	return assertTrue(!statement, error, val);
}
int assertEqualsInt(int expected, int actual, const char* error, int* val){
	return assertTrue(expected == actual, error, val);
}