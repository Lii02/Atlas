#ifndef ATLAS_LIBC_STRING_H
#define ATLAS_LIBC_STRING_H
#include "stdint.h"

size_t strlen(const char* str);
void itoa(int32_t n, char* str, uint32_t radix);
void* memset(void* ptr, int v, size_t s);
void* memcpy(void* dest, void* src, int32_t len);
char* strcpy(char* dest, char* src);
char* strcat(char* dest, char* src);
void strrev(uint8_t* str);

#endif
