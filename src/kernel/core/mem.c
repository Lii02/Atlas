#include "mem.h"

void memcpy(uint8_t* src, uint8_t* dest, int32_t bytes)
{
    int i;
    for (i = 0; i < bytes; i++) {
        *(dest + i) = *(src + i);
    }
}

void memset(uint8_t* dest, uint8_t val, uint8_t* length)
{
    uint8_t* temp = (uint8_t*)dest;
    for ( ; length != 0; length--) *temp++ = val;
}

uintptr_t current_break;

void* sbrk(uintptr_t i)
{
    uintptr_t old_break = current_break;
    current_break += i;
    return (void*)old_break;
}

header_t *head, *tail;

void* malloc(size_t size)
{
	size_t total_size;
	void* block;
	header_t* header;
	if (!size)
		return NULL;
	header = get_free_block(size);
	if (header) {
		header->s.is_free = 0;
		return (void*)(header + 1);
	}
	total_size = sizeof(header_t) + size;
	block = sbrk(total_size);
	if (block == (void*) -1) {
		return NULL;
	}
	header = block;
	header->s.size = size;
	header->s.is_free = 0;
	header->s.next = NULL;
	if (!head)
		head = header;
	else if (tail)
		tail->s.next = header;
	tail = header;
	return (void*)(header + 1);
}

header_t* get_free_block(size_t size)
{
	header_t *curr = head;
	while(curr) {
		if (curr->s.is_free && curr->s.size >= size)
			return curr;
		curr = curr->s.next;
	}
	return NULL;
}

void* calloc(size_t num, size_t nsize)
{
    size_t size;
	void* block;
	if (!num || !nsize)
		return NULL;
	size = num * nsize;
	if (nsize != size / num)
		return NULL;
	block = malloc(size);
	if (!block)
		return NULL;
	memset(block, 0, size);
	return block;
}

void* realloc(void* block, size_t size)
{
    header_t* header;
	void* ret;
	if (!block || !size)
		return malloc(size);
	header = (header_t*)block - 1;
	if (header->s.size >= size)
		return block;
	ret = malloc(size);
	if (ret) {
		
		memcpy(ret, block, header->s.size);
		free(block);
	}
	return ret;
}

void free(void* block)
{
    header_t* header, *tmp;
	void* programbreak;

	if (!block)
		return;
	header = (header_t*)block - 1;

	programbreak = sbrk(0);
	if ((char*)block + header->s.size == programbreak) {
		if (head == tail) {
			head = tail = NULL;
		} else {
			tmp = head;
			while (tmp) {
				if(tmp->s.next == tail) {
					tmp->s.next = NULL;
					tail = tmp;
				}
				tmp = tmp->s.next;
			}
		}
		sbrk(0 - sizeof(header_t) - header->s.size);
		return;
	}
	header->s.is_free = true;
}