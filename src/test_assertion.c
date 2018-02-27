#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assertion.h"

int testAssertTrue();
int testAssertFalse();
int testAssertEqualsInt();
int testRegisterTest();
int dummyFunction();
int main(){
	testAssertTrue();
	testAssertFalse();
	testAssertEqualsInt();
	testRegisterTest();
	return 0;
}

int testAssertTrue(){
	int ret = -1;
	assertTrue(0 == 0, "assertTrue, 0==0 fails.", &ret);
	assertTrue(ret == 1, "assertTrue, ret isnt changed.", &ret);
	assertTrue(0 != 1, "assertTrue, 0!=1 succeeds.", &ret);
	assertTrue(0 != 0, "assertTrue, intended failure, working as intended.", &ret);
	assertTrue(true, "assertTrue, THIS SHOULD BE SKIPPED.", &ret);
	return ret;
}
int testAssertFalse(){
	int ret = -1;
	assertFalse(0 == 1, "assertFalse, 0==1 fails.", &ret);
	assertFalse(ret != 1, "assertFalse, ret isnt changed.", &ret);
	assertFalse(0 != 0, "assertFalse, 0!=0 fails.", &ret);
	assertFalse(0 == 0, "assertFalse, intended failure, working as intended.", &ret);
	assertFalse(false, "assertTrue, THIS SHOULD BE SKIPPED.", &ret);
	return ret;
}
int testAssertEqualsInt(){
	int ret = -1;
	assertEqualsInt(1, 1, "assertEqualsInt, 1 expected 1 actual fails.", &ret);
	assertEqualsInt(1, ret, "assertEqualsInt, ret isnt changed.", &ret);
	assertEqualsInt(0, 1, "assertEqualsInt, intended failure, working as intended.", &ret);
	return ret;
}
int testRegisterTest(){
	int ret = -1;
	assertTrue(registerTest(NULL, NULL) == -1, "registerTest, doesnt fail on NULL NULL.", &ret);
	assertTrue(registerTest(NULL, "testString") == -1, "registerTest, doesnt fail on NULL \"testString\".", &ret);
	assertTrue(registerTest(dummyFunction, NULL) == -1, "registerTest, doesnt fail on dummyFunction NULL.", &ret);
	assertTrue(registerTest(dummyFunction, "testString") == 0, "registerTest, fails on valid input dummyFunction \"testString\".", &ret);
	return ret;
}
int dummyFunction(){
	int ret = 0;
	ret++;
	return ret;
}
//dummyFunction