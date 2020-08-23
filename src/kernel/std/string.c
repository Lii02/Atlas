#include "string.h"
#include "stdlib.h"
#include "../core/mem.h"

unsigned int strlen(char* str)
{
	unsigned int length = 0;
	while(str[length]) length++;
	return length;
}

void itoa(int n, char* str, int radix)
{
	int i, sign, strt;

	switch (radix)
	{
	case 16:
		str[0] = '0';
		str[1] = 'x';
		strt = 2;
		i = strt;
		break;
	default:
		strt = 0;
		break;
	}
	
	i = strt;
    if ((sign = n) < 0) n = -n;
    do
	{
        str[i++] = n % radix + '0';
    } while ((n /= radix) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

	int c, j;
    for (i = strt, j = strlen(str) - 1; i < j; i++, j--)
	{
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

int strcmp(char* str1, char* str2)
{
	const unsigned char *s1 = (const unsigned char *) str1;
	const unsigned char *s2 = (const unsigned char *) str2;
  	unsigned char c1, c2;
  	do
    {
		c1 = (unsigned char) *s1++;
		c2 = (unsigned char) *s2++;
		if (c1 == '\0')
        	return c1 - c2;
    }
  	while (c1 == c2);
	return c1 - c2;
}

char* strcpy(char* dest, const char* src)
{
	return (char*)memcpy(dest, src, strlen(src) + 1);
}

char* strcat(char* dest, const char* src)
{
	strcpy(dest + strlen(dest), src);
	return dest;
}

char* strncpy(char* dst, const char* src, unsigned int n)
{
	if (n != 0)
	{
		char* d = dst;
		const char* s = src;

		do
		{
			if ((*d++ = *s++) == 0)
			{
				while (--n != 0)
					*d++ = 0;
				break;
			}
		} while (--n != 0);
	}
	return (dst);
}

char* strncat(char* s1, const char* s2, unsigned int n)
{
    unsigned len1 = strlen(s1);
    unsigned len2 = strlen(s2);
    
    if (len2 < n)
	{
	    strcpy(&s1[len1], s2);
    }
	else
	{
	    strncpy(&s1[len1], s2, n);
	    s1[len1 + n] = '\0';
    }
    return s1;
}

char* strchr(const char* p, int ch)
{
	char c;

	c = ch;
	for (;; ++p) {
		if (*p == c)
			return ((char *)p);
		if (*p == '\0')
			return (NULL);
	}
}

char* strncmp(char* str1, char* str2, int n)
{
	if (n == 0)
		return 0;
	do
	{
		if (*str1 != *str2++)
			return (*(unsigned char *)str1 - *(unsigned char *)--str2);
		if (*str1++ == 0)
			break;
	} while (--n != 0);
	return 0;
}

unsigned int strcspn (const char* str, const char* reject)
{
	
}