#ifndef ATLAS_MALLOC_H
#define ATLAS_MALLOC_H
#include "stdint.h"
#include "stdbool.h"

typedef unsigned int uintptr_t;
static uintptr_t current_brk;
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
static memheader_t *head, *tail;

void* sbrk(uintptr_t i);
memheader_t* get_free_block(size_t size);
void* malloc(size_t size);
void free(void* block);

#endif
