#ifndef i386ASM_H
#define i386ASM_H
#include "../../libc/stdint.h"

#if defined(I386)
#define CPUOUTB i386outb
#define CPUOUTW i386outw
#define CPUOUTL i386outl
#define CPUINB i386inb
#define CPUINW i386intw
#define CPUINL i386inl
#define CPUGETBYTE i386getbyte
#define CPUGETWORD i386getword
#define CPUGETDWORD i386getdword
#define CPUPUTBYTE i386putbyte
#define CPUPUTWORD i386putword
#define CPUPUTDWORD i386putdword
#define CPULO16(addr) (int16_t)(addr & 0xFFFF)
#define CPUHI16(addr) (int16_t)((addr >> 16) & 0xFFFF)
#endif

struct i386registers_t
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};
#if defined(I386)
typedef struct i386registers_t cpuregisters_t;
#endif

void i386outb(uint16_t port, uint8_t data);
void i386outw(uint16_t port, uint16_t data);
void i386outl(uint16_t port, uint32_t data);

uint8_t i386inb(uint16_t port);
uint16_t i386inw(uint16_t port);
uint32_t i386inl(uint16_t port);

int8_t i386getbyte(const char* addr);
int16_t i386getword(const short* addr);
int32_t i386getdword(const int* addr);

void i386putbyte(int8_t v, char* addr);
void i386putword(int16_t v, short* addr);
void i386putdword(int32_t v, int* addr);

#endif
