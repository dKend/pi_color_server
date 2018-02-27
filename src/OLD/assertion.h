#include <stdbool.h>
int assertTrue(bool statement, const char* error, int* val);
int assertFalse(bool statement, const char* error, int* val);
int assertEqualsInt(int expected, int actual, const char* error, int* val);