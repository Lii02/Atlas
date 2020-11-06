#include <libc/string.h>

size_t strlen(const char* str)
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

void* memset(void* ptr, int v, size_t s)
{
	uint8_t* temp = (uint8_t*)ptr;
	for(; s != 0; s--)
	{
		(*temp++) = v;
	}
	ptr = (void*)temp;
	return ptr;
}

void* memcpy(void* dest, void* src, int32_t len)
{
	char *d = dest;
	const char *s = src;
	while (len--)
		*d++ = *s++;
	return dest;
}

char* strcpy(char* dest, char* src)
{
	return memcpy(dest, src, strlen(src) + 1);
}

char* strcat(char* dest, char* src)
{
	strcpy(dest + strlen(dest), src);
	return dest;
}
