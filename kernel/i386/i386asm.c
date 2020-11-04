#include <atlas/i386/i386asm.h>

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

int8_t ia64getbyte(const char* addr)
{
	int8_t v;
	__asm__("movb %%fs:%1, %0":"=q" (v):"m" (*addr));
	return v;
}

int16_t ia64getword(const short* addr)
{
	int16_t v;
	__asm__("movw %%fs:%1, %0":"=q" (v):"m" (*addr));
	return v;
}

int32_t ia64getdword(const int* addr)
{
	int32_t v;
	__asm__("movl %%fs:%1, %0":"=q" (v):"m" (*addr));
	return v;
}

void ia64putbyte(int8_t v, char* addr)
{
	__asm__("movb %0, %%fs:%1" : : "iq"(v), "m"(*addr));
}

void ia64putword(int16_t v, short* addr)
{
	__asm__("movw %0, %%fs:%1" : : "ir"(v), "m"(*addr));

}

void ia64putdword(int32_t v, int* addr)
{
	__asm__("movl %0, %%fs:%1" : : "ir"(v), "m"(*addr));
}
