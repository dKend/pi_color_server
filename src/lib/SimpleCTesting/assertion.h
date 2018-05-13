#include <stdbool.h>
typedef struct fPair{
	int (*f)();
	char* name;
} fPair;

int registerTest(int (*f)(), const char* str);
int runTests();
int destroyTests();
int assertTrue(bool statement, const char* error, int* val);
int assertFalse(bool statement, const char* error, int* val);
int assertEqualsInt(int expected, int actual, const char* error, int* val);