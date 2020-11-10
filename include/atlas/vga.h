#ifndef ATLAS_VGA_H
#define ATLAS_VGA_H
#include <libc/stdint.h>
#include <libc/stdbool.h>

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

void init_vga(size_t width, size_t height);
void free_vga();
void set_vga_colors(vga_color fg, vga_color bg);
void vga_enable_cursor();
void vga_disable_cursor();
uint8_t vga_create_color(vga_color fg, vga_color bg);
uint16_t vga_char(char ch, uint8_t color);
void reset_vga();
void vga_update_cursor();
void write_char_vga(char ch, uint32_t x, uint32_t y);
void vga_writeptr(uint16_t v, uint32_t x, uint32_t y);
void vga_putc(char ch);
void vga_puts(char* str);
void vga_scroll(uint32_t count);
char vga_read_char(uint32_t x, uint32_t y);

#endif
