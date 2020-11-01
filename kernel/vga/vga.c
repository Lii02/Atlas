#include "vga.h"
#include "../i32/i32asm.h"

extern void set_as_vga();

vga_instance init_vga(size_t width, size_t height)
{
    vga_instance vga;
    vga.width = width;
    vga.height = height;
    vga_enable_cursor(&vga);
    clear_vga(&vga);
    //set_as_vga();
    return vga;
}

void set_vga_colors(vga_instance* vga, vga_color fg, vga_color bg)
{
    vga->raw_fg = fg;
    vga->raw_fg = bg;
    vga->new_color = vga_create_color(fg, bg);
    clear_vga(vga);
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

uint8_t vga_create_color(vga_color fg, vga_color bg)
{
    return (fg | (bg << 4));
}

uint16_t vga_char(char ch, uint8_t color)
{
    return (uint16_t) ch | ((uint16_t) color << 8);
}

void clear_vga(vga_instance* vga)
{
    uint16_t* buff = VGA_BUFFER;
    for(int x = 0; x < vga->width; x++)
    {
        for(int y = 0; y < vga->height; y++)
        {
            size_t index = y * vga->width + x;
            buff[index] = vga_char(' ', vga->new_color);  
        }
    }
    vga->cursor_x = 0;
    vga->cursor_y = 0;
    vga_update_cursor(vga);
}

void vga_update_cursor(vga_instance* vga)
{
    size_t index = vga->cursor_y * vga->width + vga->cursor_x;
    i32outb(0x3D4, 0xE);
    i32outb(0x3D5, (uint8_t)(index >> 8));
    i32outb(0x3D4, 0xF);
    i32outb(0x3D5, (uint8_t)(index));
}

void write_char_vga(char ch, uint32_t x, uint32_t y, vga_instance* vga)
{
    vga_writeptr(vga, vga_char(ch, vga->new_color), x, y);
}

void vga_writeptr(vga_instance* vga, uint16_t v, uint32_t x, uint32_t y)
{
    uint32_t index = y * vga->width + x;
    uint16_t* buff = VGA_BUFFER;
    buff[index] = v;
}

void vga_putc(char ch, vga_instance* vga)
{
    switch(ch)
    {
	case '\0': return; break;
	default:
		   if(vga->cursor_x >= vga->width)
		   {
		   }
	break;
    }
}
