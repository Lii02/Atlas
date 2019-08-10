#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef enum
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LGRAY = 7,
    VGA_COLOR_DGRAY = 8,
    VGA_COLOR_LBLUE = 9,
    VGA_COLOR_LGREEN = 10,
    VGA_COLOR_LCYAN = 11,
    VGA_COLOR_LRED = 12,
    VGA_COLOR_LMAGENTA = 13,
    VGA_COLOR_LBROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color;

static inline uint8_t vga_entry_color(vga_color fg, vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char c, uint8_t color)
{
    return (uint16_t) c | (uint16_t) color << 8;
}

#define RES_WIDTH 200
#define RES_HEIGHT 100

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize()
{
}
