#ifndef IA64ASM_H
#define IA64ASM_H

#include "../standard/stdint.h"

void ia64outb(uint16_t port, uint8_t data);
void ia64outw(uint16_t port, uint16_t data);
void ia64outl(uint16_t port, uint32_t data);

uint8_t ia64inb(uint16_t port);
uint16_t ia64inw(uint16_t port);
uint32_t ia64inl(uint16_t port);

#endif
