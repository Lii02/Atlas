#include "atlas_std.h"

uint32 strlen(string str)
{
    uint32 len = 0;

    while (str[len]) {
        len++;
    }

    return len;
}
