#ifndef STRING_H
#define STRING_H

unsigned int strlen(char* str)
{
	unsigned int length = 0;
	while(str[length]) length++;
	return length;
}

#endif