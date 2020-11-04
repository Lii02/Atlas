#ifndef ATLAS_LIBC_STRING_H
#define ATLAS_LIBC_STRING_H
#include "stdint.h"

size_t strlen(const char* str);
void itoa(int32_t n, char* str, uint32_t radix);
void* memset(void* ptr, int v, size_t s);

#endif
