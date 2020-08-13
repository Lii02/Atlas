#ifndef ALLOC_H
#define ALLOC_H

#include <stdlib.h>
#include <stdint.h>

#define FREE (1 << 0)
#define ALLOCATED (1 << 1)

struct block
{
    size_t size;
    uint8_t flag;
    void* data;
};

struct alloc_table
{
    size_t width;
    size_t height;
    uint8_t flag;
    struct block* blocks;
};

#endif