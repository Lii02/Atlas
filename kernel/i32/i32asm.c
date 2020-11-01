#include "i32asm.h"

void i32outb(uint16_t port, uint8_t data)
{
    __asm__("outb %%al, %%dx" : : "a" (data), "d" (port));
}

void i32outw(uint16_t port, uint16_t data)
{
    __asm__("outw %%ax, %%dx" : : "a" (data), "d" (port));
}

void i32outl(uint16_t port, uint32_t data)
{
    __asm__("outl %%eax, %%dx" : : "a" (data), "d" (port));
}

uint8_t i32inb(uint16_t port)
{
    uint8_t result;
	__asm__("inb %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

uint16_t i32inw(uint16_t port)
{
    uint16_t result;
	__asm__("inw %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

uint32_t i32inl(uint16_t port)
{
    uint32_t result;
	__asm__("inl %%dx, %%eax" : "=a" (result) : "d" (port));
	return result;
}