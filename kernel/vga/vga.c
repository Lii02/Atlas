#include "vga.h"
#include "../i32/i32asm.h"

vga_instance init_vga(size_t width, size_t height)
{
    vga_instance vga;
    vga.width = width;
    vga.height = height;
    vga.cursor_x = 0;
    vga.cursor_y = 0;
    vga.bcursor = true;
    return vga;
}

void set_vga_colors(vga_instance* vga, vga_color fg, vga_color bg)
{
    vga->raw_fg = fg;
    vga->raw_fg = bg;
    vga->new_color = create_color(fg, bg);
    
    uint16_t* buff = VGA_BUFFER;
    for (size_t y = 0; y < vga->height; y++) {
		for (size_t x = 0; x < vga->width; x++) {
			size_t index = y * vga->width + x;
			buff[index] = vga_char(' ', vga->new_color);
		}
	}
}

void vga_enable_cursor(vga_instance* vga)
{
    vga->bcursor = true;
    i32outb(0x3D4, 0x0A);
	i32outb(0x3D5, (i32inb(0x3D5) & 0xC0) | 0x0);
	
	i32outb(0x3D4, 0x0B);
	i32outb(0x3D5, (i32inb(0x3D5) & 0xE0) | 0x1);
}

void vga_disable_cursor(vga_instance* vga)
{
    vga->bcursor = false;
    i32outb(0x3D4, 0x0A);
	i32outb(0x3D5, 0x20);
}

uint8_t create_color(vga_color fg, vga_color bg)
{
    return (fg | (bg << 4));
}

uint16_t vga_char(char ch, uint8_t color)
{
    return (uint16_t) ch | ((uint16_t) color << 8);
}