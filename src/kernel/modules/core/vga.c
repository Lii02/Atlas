#include "vga.h"
#include <atlas/string.h>

static vga_terminal terminal;

static uint8_t encode_color(vga_color fg, vga_color bg)
{
    return fg | (bg << 4);
}

static uint16_t vga_entry(char c, uint8_t color)
{
    return (uint16_t) c | ((uint16_t) color << 8);
}

static void vga_update_cursor(void)
{
	uint32_t pos = terminal.cursor_y * terminal.width + terminal.cursor_x;
	
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) (pos >> 8));
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) pos);
}

static void lnfeed(void)
{
	terminal.cursor_y++;
	if(terminal.cursor_y >= terminal.height) vga_scroll(1);
	else vga_update_cursor();
}

static void cret(void)
{
	terminal.cursor_x = 0;
	vga_update_cursor();
}

static void backspace(vga_color fg, vga_color bg)
{
	if(terminal.cursor_x == 0)
	{
		if(terminal.cursor_y > 0)
		{
			terminal.cursor_x = terminal.width - 1;
			terminal.cursor_y--;
		}
	}
	else terminal.cursor_x--;
	vga_write(' ', terminal.cursor_x, terminal.cursor_y, fg, bg);
}

static void write_char(char c, vga_color fg, vga_color bg)
{
	if(c == '\0') return;
	if(c == '\t')
	{
		int i;
		for(i = 0 ; i < 4 ; ++i) write_char(' ', fg, bg);
		return;
	}
	else if(c == '\n') lnfeed();
	else if(c == '\r') cret();
	else if(c == '\b') backspace(fg, bg);
	else
	{
		vga_write(c, terminal.cursor_x++, terminal.cursor_y, fg, bg);
		if(terminal.cursor_x >= terminal.width)
		{
			lnfeed();
			cret();
		}
	}
}

void init_terminal(size_t width, size_t height)
{
    terminal.width = width;
    terminal.height = height;
    terminal.cursor_x = 0;
    terminal.cursor_y = 0;
}

void cl_terminal(vga_color fg, vga_color bg)
{
	for(int x = 0 ; x < terminal.width ; ++x)
	{
		for(int y = 0 ; y < terminal.height ; ++y) vga_write(' ', x, y, fg, bg);
	}
	vga_set_cursor(0, 0);
}

void iputc(char c, vga_color fg, vga_color bg)
{
	write_char(c, fg, bg);
	vga_update_cursor();
}

void iputs(char* str, vga_color fg, vga_color bg)
{
	int i;
	for(i = 0 ; i < strlen(str) ; ++i)
	{
		write_char(str[i], fg, bg);
	}
	vga_update_cursor();
}

void putc(char c)
{
	iputc(c, VGA_LIGHT_GREY, VGA_BLACK);
}

void puts(char* str)
{
	iputs(str, VGA_LIGHT_GREY, VGA_BLACK);
}

void vga_write(char c, uint32_t x, uint32_t y, vga_color fg, vga_color bg)
{
	vga_write_raw(vga_entry(c, encode_color(fg, bg)), x, y);
}

void vga_write_raw(uint16_t value, uint32_t x, uint32_t y)
{
	uint32_t index = y * terminal.width + x;
	uint16_t* ptr = 0xB8000;
	ptr[index] = value;
}

uint16_t vga_read(uint32_t x, uint32_t y)
{
	uint32_t index = y * terminal.width + x;
	uint16_t* ptr = 0xB8000;
	return ptr[index];
}

char vga_read_char(uint32_t x, uint32_t y)
{
	return (char) (vga_read(x, y) & 0xFF);
}

vga_color vga_read_fg(uint32_t x, uint32_t y)
{
	return (uint8_t) ((vga_read(x, y) >> 8) & 0x0F);
}

vga_color vga_read_bg(uint32_t x, uint32_t y)
{
	return (uint8_t) ((vga_read(x, y) >> 12) & 0x0F);
}

void vga_set_cursor(uint32_t x, uint32_t y)
{
	terminal.cursor_x = x;
	terminal.cursor_y = y;
	vga_update_cursor();
}

uint32_t vga_cursor_x(void)
{
	return terminal.cursor_x;
}

uint32_t vga_cursor_y(void)
{
	return terminal.cursor_y;
}

void vga_enable_cursor(void)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | 0x0); // Start at row 0x0
	
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | 0x1); // End at row 0x1
}

void vga_disable_cursor(void)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void vga_scroll(uint32_t count)
{
	int i, y, x;
	for(i = 0 ; i < count ; ++i)
	{
		for(y = 0 ; y < terminal.height - 1 ; ++y)
		{
			for(x = 0 ; x < terminal.width ; ++x) vga_write_raw(vga_read(x, y + 1), x, y);
		}
	}
	for(x = 0 ; x < terminal.width ; ++x) vga_write(' ', x, terminal.height - 1, vga_read_fg(x, terminal.height - 1), vga_read_bg(x, terminal.height -1));
	
	if(count > terminal.cursor_y) vga_set_cursor(0, 0);
	else vga_set_cursor(terminal.cursor_x, terminal.cursor_y - count);
}