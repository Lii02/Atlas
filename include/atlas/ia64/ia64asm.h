#ifndef IA64ASM_H
#define IA64ASM_H
#include "../standard/stdint.h"

#if defined(IA64)
#define CPUOUTB ia64outb
#define CPUOUTW ia64outw
#define CPUOUTL ia64outl
#define CPUINB ia64inb
#define CPUINW ia64intw
#define CPUINL ia64inl
#define CPUGETBYTE ia64getbyte
#define CPUGETWORD ia64getword
#define CPUGETDWORD ia64getdword
#define CPUPUTBYTE ia64putbyte
#define CPUPUTWORD ia64putword
#define CPUPUTDWORD ia64putdword
#define CPULO16(addr) (int16_t)(addr & 0xFFFF)
#define CPUHI16(addr) (int16_t)((addr >> 16) & 0xFFFF)
#endif

struct ia64registers_t
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};
#if defined(IA64)
typedef struct ia64registers_t cpuregisters_t;
#endif

void ia64outb(uint16_t port, uint8_t data);
void ia64outw(uint16_t port, uint16_t data);
void ia64outl(uint16_t port, uint32_t data);

uint8_t ia64inb(uint16_t port);
uint16_t ia64inw(uint16_t port);
uint32_t ia64inl(uint16_t port);

int8_t ia64getbyte(const char* addr);
int16_t ia64getword(const short* addr);
int32_t ia64getdword(const int* addr);

void ia64putbyte(int8_t v, char* addr);
void ia64putword(int16_t v, short* addr);
void ia64putdword(int32_t v, int* addr);

#endif
