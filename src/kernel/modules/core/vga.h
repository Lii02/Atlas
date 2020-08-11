#include "asm.h"

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
	uint32_t cursor_x;
	uint32_t cursor_y;
	vga_color fg;
	vga_color bg;
} vga_terminal;

void init_terminal(size_t width, size_t height);
void cl_terminal(vga_color fg, vga_color bg);

void iputc(char c, vga_color fg, vga_color bg);
void iputs(char* str, vga_color fg, vga_color bg);

void putc(char c);
void puts(char* str);

void vga_write(char c, uint32_t x, uint32_t y, vga_color fg, vga_color bg);
void vga_write_raw(uint16_t value, uint32_t x, uint32_t y);
uint16_t vga_read(uint32_t x, uint32_t y);
char vga_read_char(uint32_t x, uint32_t y);
vga_color vga_read_fg(uint32_t x, uint32_t y);
vga_color vga_read_bg(uint32_t x, uint32_t y);

void vga_set_cursor(uint32_t x, uint32_t y);
uint32_t vga_cursor_x(void);
uint32_t vga_cursor_y(void);
void vga_enable_cursor(void);
void vga_disable_cursor(void);

void vga_scroll(uint32_t count);

vga_terminal* get_global_vga_terminal();

#endif
