#ifndef _EXTRA
#define _EXTRA
#include "Arduino.h"
#include "string.h"
bool strcmp2(char* a, char* b);

int strcmp3(char* a, char* b, bool sensitive = false) ;

char* stristr( const char* str1, const char* str2 );
#endif