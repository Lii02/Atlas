#ifndef ALLOC_H
#define ALLOC_H

#include "../std/stdlib.h"
#include "../std/stdint.h"
#include "../std/stdbool.h"

typedef unsigned int uintptr_t;

void* sbrk(uintptr_t i);

typedef char ALIGN[0x10];

union header
{
    struct
    {
        size_t size;
        bool is_free;
        union header* next;
    } s;
    ALIGN stub;
};
typedef union header header_t;

// allocation functions
void* malloc(size_t size);
header_t* get_free_block(size_t size);
void* calloc(size_t num, size_t nsize);
void* realloc(void* block, size_t size);
void free(void* block);

#endif