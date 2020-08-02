#ifndef DEFS_H
#define DEFS_H

#define NULL ((void*)0x0)

#define FALSE 0x0
#define TRUE 0x1

typedef unsigned int size_t;

typedef unsigned char uchar;
typedef unsigned short int uint_16;
typedef unsigned int uint_32;
typedef unsigned long int uint_64;

typedef short int int_16;
typedef int int_32;
typedef long int int_64;

void outb(uint_16 port, uchar data);
void outw(uint_16 port, uint_16 data);
void outl(uint_16 port, uint_32 data);

uchar inb(uint_16 port);
uint_16 inw(uint_16 port);
uint_32 inl(uint_16 port);

#endif
