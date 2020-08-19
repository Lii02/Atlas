#ifndef STDLIB_H
#define STDLIB_H

#include "stdint.h"

typedef unsigned int size_t;
#define NULL ((void*)0x0)

#define EOF -1 

void* memcpy(uint8_t* src, uint8_t* dest, int32_t bytes);
void memset(uint8_t* dest, uint8_t val, uint8_t* length);

#endif