#include "defs.h"
#include "string.h"

#ifndef VGA_H
#define VGA_H

typedef enum
{
	VGA_BLACK = 0,
	VGA_BLUE = 1,
	VGA_GREEN = 2,
	VGA_CYAN = 3,
	VGA_RED = 4,
	VGA_MAGENTA = 5,
	VGA_BROWN = 6,
	VGA_LIGHT_GREY = 7,
	VGA_DARK_GREY = 8,
	VGA_LIGHT_BLUE = 9,
	VGA_LIGHT_GREEN = 10,
	VGA_LIGHT_CYAN = 11,
	VGA_LIGHT_RED = 12,
	VGA_LIGHT_MAGENTA = 13,
	VGA_LIGHT_BROWN = 14,
	VGA_WHITE = 15,
} vga_color;

typedef struct
{
	size_t width;
	size_t height;
	uint_32 cursor_x;
	uint_32 cursor_y;
} vga_terminal;

void init_terminal(size_t width, size_t height);

void iputc(char c, vga_color fg, vga_color bg);
void iputs(string str, size_t length, vga_color fg, vga_color bg);

void putc(char c);
void puts(string str, size_t length);

void vga_write(char c, int x, int y, vga_color fg, vga_color bg);

#endif
