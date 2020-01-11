#include "atlas_std.h"

#ifndef VGA_H
#define VGA_H

typedef enum
{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color;

typedef struct
{
	size_t width;
	size_t height;
	unsigned int terminal_row;
	unsigned int terminal_column;
	vga_color terminal_color;
	unsigned int *terminal_buffer;
} atlas_vga_terminal;

unsigned int vga_entry_color(vga_color back, vga_color fore);
unsigned int vga_entry(uchar c, unsigned int color);

atlas_vga_terminal terminal_initialize(unsigned int background, unsigned int foreground, size_t width, size_t height);
void free_terminal(atlas_vga_terminal *terminal);
void terminal_set_color(atlas_vga_terminal *terminal, unsigned int color);
void terminal_putentry(atlas_vga_terminal *terminal, char c, unsigned int color, unsigned int x, unsigned int y);
void terminal_putchar(atlas_vga_terminal *terminal, char c);
void terminal_write(atlas_vga_terminal *terminal, atlas_string str, unsigned int size);
void terminal_writestring(atlas_vga_terminal *terminal, atlas_string str);

#endif