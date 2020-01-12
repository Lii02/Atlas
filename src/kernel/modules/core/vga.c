#include "vga.h"

static vga_terminal terminal;

static uchar encode_color(vga_color fg, vga_color bg)
{
    return fg | bg << 4;
}

static uint_16 vga_entry(char c, uchar color)
{
    return (uint_16) c | (uint_16) color << 8;
}

void init_terminal(size_t width, size_t height)
{
    terminal.width = width;
    terminal.height = height;
    terminal.cursor_x = 0;
    terminal.cursor_y = 0;
}

void iputc(char c, vga_color fg, vga_color bg)
{
	vga_write(c, terminal.cursor_x++, terminal.cursor_y, fg, bg);
	if(terminal.cursor_x >= terminal.width)
	{
		terminal.cursor_y++;
		terminal.cursor_x = 0;
	}
}

void iputs(string str, size_t length, vga_color fg, vga_color bg)
{
	int i;
	for(i = 0 ; i < length ; ++i) iputc(str[i], fg, bg);
}

void putc(char c)
{
	iputc(c, VGA_LIGHT_GREY, VGA_BLACK);
}

void puts(string str, size_t length)
{
	iputs(str, length, VGA_LIGHT_GREY, VGA_BLACK);
}

void vga_write(char c, int x, int y, vga_color fg, vga_color bg)
{
	uint_32 index = y * terminal.width + x;
	uint_16* ptr = 0xB8000;
	ptr[index] = vga_entry(c, encode_color(fg, bg));
}
