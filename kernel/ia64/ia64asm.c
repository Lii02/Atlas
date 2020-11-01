#include "ia64asm.h"

void ia64outb(uint16_t port, uint8_t data)
{
    __asm__("outb %%al, %%dx" : : "a" (data), "d" (port));
}

void ia64outw(uint16_t port, uint16_t data)
{
    __asm__("outw %%ax, %%dx" : : "a" (data), "d" (port));
}

void ia64outl(uint16_t port, uint32_t data)
{
    __asm__("outl %%eax, %%dx" : : "a" (data), "d" (port));
}

uint8_t ia64inb(uint16_t port)
{
    uint8_t result;
	__asm__("inb %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

uint16_t ia64inw(uint16_t port)
{
    uint16_t result;
	__asm__("inw %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

uint32_t ia64inl(uint16_t port)
{
    uint32_t result;
	__asm__("inl %%dx, %%eax" : "=a" (result) : "d" (port));
	return result;
}
