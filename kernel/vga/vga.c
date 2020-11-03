#include <atlas/vga/vga.h>
#include <atlas/ia64/ia64asm.h>
#include <atlas/standard/string.h>

extern void set_as_vga();

#define VGA_SET_CURSOR(vga, x, y) vga->cursor_x = x; vga->cursor_y = y; vga_update_cursor(vga);
#define CRET(vga) vga->cursor_x = 0; vga_update_cursor(vga);
#define LNFEED(vga) vga->cursor_y++; if(vga->cursor_y >= vga->height) vga_scroll(1, vga); else vga_update_cursor(vga);
#define BACKSPACE(vga) \
	if(vga->cursor_x == 0) \
	{ \
		if(vga->cursor_y > 0) \
		{ \
			vga->cursor_x = vga->width - 1; \
			vga->cursor_y++; \
		} \
	} \
	else \
		vga->cursor_x--; \
	vga_putc(' ', vga); \

vga_instance init_vga(size_t width, size_t height)
{
    vga_instance vga;
    vga.width = width;
    vga.height = height;
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
    CPUOUTB(0x3D4, 0x0A);
    CPUOUTB(0x3D5, (ia64inb(0x3D5) & 0xC0) | 0x0);
    CPUOUTB(0x3D4, 0x0B);
    CPUOUTB(0x3D5, (ia64inb(0x3D5) & 0xE0) | 0x1);
}

void vga_disable_cursor(vga_instance* vga)
{
    vga->bcursor = false;
    CPUOUTB(0x3D4, 0x0A);
	CPUOUTB(0x3D5, 0x20);
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
    CPUOUTB(0x3D4, 0xE);
    CPUOUTB(0x3D5, (uint8_t)(index >> 8));
    CPUOUTB(0x3D4, 0xF);
    CPUOUTB(0x3D5, (uint8_t)(index));
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
	case '\0': return;
	case '\t':
		for(int i = 0; i < 4; i++)
		{
			vga_putc(' ', vga);
			return;
		}
	case '\r':
		CRET(vga);
		return;
	case '\n':
		LNFEED(vga);
		return;
	case '\b':
		BACKSPACE(vga);
		return;
	default:
		write_char_vga(ch, vga->cursor_x++, vga->cursor_y, vga);
		vga_update_cursor(vga);
		return;
	}
}

void vga_puts(char* str, vga_instance* vga)
{
	size_t len = strlen(str);
	int i;
	for(i = 0; i < len; i++) vga_putc(str[i], vga);
}

void vga_scroll(uint32_t count, vga_instance* vga)
{
	int i, x, y;
	for(i = 0; i < count; i++)
	{
		for(y = 0; y < vga->height - 1; y++)
		{
			for(x = 0; x < vga->width; x++) vga_writeptr(vga, vga_read_char(x, y + 1, vga), x, y);	
		}
	}
	for(x = 0; x < vga->width; x++) vga_writeptr(vga, ' ', x, vga->height - 1);

	if(count > vga->cursor_y) 
	{
		VGA_SET_CURSOR(vga, 0, 0);
	}
	else
	{
		VGA_SET_CURSOR(vga, vga->cursor_x, vga->cursor_y - 1)
	}
}

char vga_read_char(uint32_t x, uint32_t y, vga_instance* vga)
{
	uint32_t index = y * vga->width + x;
	uint16_t* buff = VGA_BUFFER;
	return buff[index] & 0xFF;
}
