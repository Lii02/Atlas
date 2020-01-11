#include "atlas_string.h"

unsigned int strlen(atlas_string str)
{
    unsigned int length = 0;

    while(str[length])
    {
        length++;
    }

    return length;
}

atlas_string strreplace(atlas_string *str, char rep, char with)
{
    for(int i = 0; i < strlen(*str); i++)
    {
        if(str[i] == rep) str[i] = with;
    }
    return *str;
}