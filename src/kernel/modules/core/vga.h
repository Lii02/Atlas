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
void cl_terminal(vga_color fg, vga_color bg);

void iputc(char c, vga_color fg, vga_color bg);
void iputs(string str, vga_color fg, vga_color bg);

void putc(char c);
void puts(string str);

void vga_write(char c, uint_32 x, uint_32 y, vga_color fg, vga_color bg);
void vga_write_raw(uint_16 value, uint_32 x, uint_32 y);
uint_16 vga_read(uint_32 x, uint_32 y);
char vga_read_char(uint_32 x, uint_32 y);
vga_color vga_read_fg(uint_32 x, uint_32 y);
vga_color vga_read_bg(uint_32 x, uint_32 y);

void vga_set_cursor(uint_32 x, uint_32 y);
uint_32 vga_cursor_x(void);
uint_32 vga_cursor_y(void);
void vga_enable_cursor(void);
void vga_disable_cursor(void);

void vga_scroll(uint_32 count);

#endif
