#ifndef VGA_H
#define VGA_H
#include "../intstd/stdint.h"
#include "../intstd/stdbool.h"

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
	vga_color raw_fg;
	vga_color raw_bg;
    bool bcursor;
    uint8_t new_color;
} vga_instance;

#define VGA_BUFFER 0xB8000

vga_instance init_vga(size_t width, size_t height);
void set_vga_colors(vga_instance* vga, vga_color fg, vga_color bg);
void vga_enable_cursor(vga_instance* vga);
void vga_disable_cursor(vga_instance* vga);
uint8_t create_color(vga_color fg, vga_color bg);
uint16_t vga_char(char ch, uint8_t color);
void vga_write_char(vga_instance* vga, char ch);

#endif