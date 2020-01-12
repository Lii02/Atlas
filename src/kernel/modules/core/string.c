#include "string.h"

uint_32 strlen(string str)
{
	unsigned int length = 0;
	while(str[length]) length++;
	return length;
}

string strreplace(string *str, char c0, char c1)
{
	for(int i = 0; i < strlen(*str); i++) if(str[i] == c0) str[i] = c1;
	return *str;
}