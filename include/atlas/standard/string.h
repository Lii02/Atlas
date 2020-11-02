#ifndef ATLAS_STRING_H
#define ATLAS_STRING_H

#include "stdint.h"

size_t strlen(char* str)
{
	size_t len = 0;
	while(str[len]) len++;
	return len;
}

void itoa(int32_t n, char* str, uint32_t radix)
{
	int i, sign;
	
	i = 0;
    if ((sign = n) < 0) n = -n;
    do
	{
        str[i++] = n % radix + '0';
    } while ((n /= radix) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

	int c, j;
    for (i = 0, j = strlen(str) - 1; i < j; i++, j--)
	{
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

#endif
