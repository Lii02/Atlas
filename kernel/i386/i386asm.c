#include <atlas/i386/i386asm.h>

void i386outb(uint16_t port, uint8_t data)
{
    __asm__("outb %%al, %%dx" : : "a" (data), "d" (port));
}

void i386outw(uint16_t port, uint16_t data)
{
    __asm__("outw %%ax, %%dx" : : "a" (data), "d" (port));
}

void i386outl(uint16_t port, uint32_t data)
{
    __asm__("outl %%eax, %%dx" : : "a" (data), "d" (port));
}

uint8_t i386inb(uint16_t port)
{
    uint8_t result;
	__asm__("inb %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

uint16_t i386inw(uint16_t port)
{
    uint16_t result;
	__asm__("inw %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

uint32_t i386inl(uint16_t port)
{
    uint32_t result;
	__asm__("inl %%dx, %%eax" : "=a" (result) : "d" (port));
	return result;
}

int8_t i386getbyte(const char* addr)
{
	int8_t v;
	__asm__("movb %%fs:%1, %0":"=q" (v):"m" (*addr));
	return v;
}

int16_t i386getword(const short* addr)
{
	int16_t v;
	__asm__("movw %%fs:%1, %0":"=q" (v):"m" (*addr));
	return v;
}

int32_t i386getdword(const int* addr)
{
	int32_t v;
	__asm__("movl %%fs:%1, %0":"=q" (v):"m" (*addr));
	return v;
}

void i386putbyte(int8_t v, char* addr)
{
	__asm__("movb %0, %%fs:%1" : : "iq"(v), "m"(*addr));
}

void i386putword(int16_t v, short* addr)
{
	__asm__("movw %0, %%fs:%1" : : "ir"(v), "m"(*addr));

}

void i386putdword(int32_t v, int* addr)
{
	__asm__("movl %0, %%fs:%1" : : "ir"(v), "m"(*addr));
}
