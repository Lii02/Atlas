#include <libc/malloc.h>

memheader_t *head, *tail;
uintptr_t current_brk;

static void* sbrk(uintptr_t i)
{
	uintptr_t old_brk = current_brk;
	current_brk += i;
	return (void*)old_brk;
}

static memheader_t* get_free_block(size_t size)
{
	memheader_t* curr = head;
	while(curr)
	{
		if(curr->s.is_free && curr->s.size >= size)
			return curr;
		curr = curr->s.next;
	}
	return NULL;
}

void* malloc(size_t size)
{
	size_t total_size;
	void* block;
	memheader_t* header;
	if (!size)
		return NULL;
	header = get_free_block(size);
	if (header)
	{
		header->s.is_free = 0;
		return (void*)(header + 1);
	}
	total_size = sizeof(memheader_t) + size;
	block = sbrk(total_size);
	if (block == (void*) -1)
	{
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

void free(void* block)
{
	memheader_t* header, *tmp;
	void* programbreak;

	if (!block)
		return;
	header = (memheader_t*)block - 1;

	programbreak = sbrk(0);
	if ((char*)block + header->s.size == programbreak)
	{
		if (head == tail)
		{
			head = tail = NULL;
		} else
		{
			tmp = head;
			while (tmp)
			{
				if(tmp->s.next == tail)
				{
					tmp->s.next = NULL;
					tail = tmp;
				}
				tmp = tmp->s.next;
			}
		}
		sbrk(0 - sizeof(memheader_t) - header->s.size);
		return;
	}
	header->s.is_free = true;
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
    memheader_t* header;
	void* ret;
	if (!block || !size)
		return malloc(size);
	header = (memheader_t*)block - 1;
	if (header->s.size >= size)
		return block;
	ret = malloc(size);
	if (ret) {
		
		memcpy(ret, block, header->s.size);
		free(block);
	}
	return ret;
}
