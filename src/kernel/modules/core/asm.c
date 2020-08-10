#include "asm.h"

uint8_t inb(uint16_t port)
{
	uint8_t result;
	__asm__("inb %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void outb(uint16_t port, uint8_t data)
{
	__asm__("outb %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t inw(uint16_t port) {
	uint16_t result;
	__asm__("inw %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

void outw(uint16_t port, uint16_t data) {
	__asm__("outw %%ax, %%dx" : : "a" (data), "d" (port));
}

uint32_t inl(uint16_t port) {
	uint32_t result;
	__asm__("inl %%dx, %%eax" : "=a" (result) : "d" (port));
	return result;
}

void outl(uint16_t port, uint32_t data) {
	__asm__("outl %%eax, %%dx" : : "a" (data), "d" (port));
}