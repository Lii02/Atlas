#ifndef ATLAS_LIBC_MALLOC_H
#define ATLAS_LIBC_MALLOC_H
#include "stdint.h"
#include "stdbool.h"

typedef unsigned int uintptr_t;
typedef int8_t ALIGN[0x10];

union memheader
{
	struct
	{
		size_t size;
		bool is_free;
		union memheader* next;
	} s;
	ALIGN stub;
};
typedef union memheader memheader_t;

static void* sbrk(uintptr_t i);
static memheader_t* get_free_block(size_t size);
void* malloc(size_t size);
void free(void* block);
void* calloc(size_t num, size_t nsize);
void* realloc(void* block, size_t size);

#endif
