#ifndef ATLAS_MALLOC_H
#define ATLAS_MALLOC_H
#include "standard/stdint.h"
#include "standard/stdbool.h"

typedef unsigned int uintptr_t;
static uintptr_t current_brk;
typedef int8_t ALIGN[0x10];

union kmemheader
{
	struct
	{
		size_t size;
		bool is_free;
		union memheader* next;
	} s;
	ALIGN stub;
};
typedef union kmemheader kmemheader_t;
static kmemheader_t *head, *tail;

static void* sbrk(uintptr_t i);
static kmemheader_t* get_free_block(size_t size);
void* kmalloc(size_t size);
void kfree(void* block);

#endif
