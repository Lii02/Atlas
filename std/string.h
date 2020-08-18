#ifndef STRING_H
#define STRING_H

unsigned int strlen(char* str)
{
	unsigned int length = 0;
	while(str[length]) length++;
	return length;
}

void itoa(int n, char* str, int radix)
{
	int i, sign, strt;
	switch(radix)
	{
		case 16:
		{
			str[0] = '0';
			str[1] = 'x';
			i = 2;
			strt = 2;
		} break;
		default:
		{
    		i = 0;
			strt = 0;
		} break;
	}

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
	int i;
    for (i = 0; str1[i] == str2[i]; i++) {
        if (str1[i] == '\0') return 0;
    }
    return str1[i] - str2[i];
}

#endif