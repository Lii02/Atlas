#include "string.h"

int str_len(string str)
{
    int len;
    while(str[len])
    {
        len++;
    }
    return len;
}
