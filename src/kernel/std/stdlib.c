#include "stdlib.h"

void* memcpy(uint8_t* src, uint8_t* dest, int32_t bytes)
{
    int i;
    for (i = 0; i < bytes; i++) {
        *(dest + i) = *(src + i);
    }
    return dest;
}

void memset(uint8_t* dest, uint8_t val, uint8_t* length)
{
    uint8_t* temp = (uint8_t*)dest;
    for ( ; length != 0; length--) *temp++ = val;
}