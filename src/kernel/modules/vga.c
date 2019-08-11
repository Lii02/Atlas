#include "vga.h"

uint8 vga_entry_color(vga_color fg, vga_color bg)
{
    return fg | bg << 4;
}

uint16 vga_entry(unsigned char c, uint8 color)
{
    return (uint16)c | (uint8) color << 8;
}
