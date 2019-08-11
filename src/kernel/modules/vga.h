#include "atlas_std.h"

#ifndef VGA_H
#define VGA_H

typedef enum
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15,
} vga_color;

// will use default size for now
#define VGA_WIDTH 80
#define VGA_HEIGHT 24

uint8 vga_entry_color(vga_color fg, vga_color bg);
uint16 vga_entry(unsigned char c, uint8 color);

#endif
