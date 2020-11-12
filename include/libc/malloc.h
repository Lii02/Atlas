#ifndef ATLAS_LIBC_MALLOC_H
#define ATLAS_LIBC_MALLOC_H
#include "stdint.h"
#include "stdbool.h"

void init_memory_paging();
void* malloc(size_t size);
void free(void* block);
void* calloc(size_t num, size_t nsize);
void* realloc(void* block, size_t size);

#endif
