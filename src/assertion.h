#include <stdbool.h>
int assertTrue(bool statement, const char* error);
int assertFalse(bool statement, const char* error);
int assertEqualsInt(int expected, int actual, const char* error);