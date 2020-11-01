#ifndef I32ASM_H
#define I32ASM_H

#include "../intstd/stdint.h"

void i32outb(uint16_t port, uint8_t data);
void i32outw(uint16_t port, uint16_t data);
void i32outl(uint16_t port, uint32_t data);

uint8_t i32inb(uint16_t port);
uint16_t i32inw(uint16_t port);
uint32_t i32inl(uint16_t port);

#endif