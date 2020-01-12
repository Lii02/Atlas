#include "defs.h"

uchar inb(uint_16 port)
{
	uchar result;
	__asm__("inb %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void outb(uint_16 port, uchar data)
{
	__asm__("outb %%al, %%dx" : : "a" (data), "d" (port));
}

uint_16 inw(uint_16 port) {
	uint_16 result;
	__asm__("inw %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

void outw(uint_16 port, uint_16 data) {
	__asm__("outw %%ax, %%dx" : : "a" (data), "d" (port));
}

uint_32 inl(uint_16 port) {
	uint_32 result;
	__asm__("inl %%dx, %%eax" : "=a" (result) : "d" (port));
	return result;
}

void outl(uint_16 port, uint_32 data) {
	__asm__("outl %%eax, %%dx" : : "a" (data), "d" (port));
}
