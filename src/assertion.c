#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "assertion.h"

int assertTrue(bool statement, const char* error){
	if(statement){
		printf("AssertPass\n");
	
		return 1;
	}
	printf("AssertFail: %s\n", error);
	return 0;
}
int assertFalse(bool statement, const char* error){
	return assertTrue(!statement, error);
}
int assertEqualsInt(int expected, int actual, const char* error){
	return assertTrue(expected == actual, const char* error);
}