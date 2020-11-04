#include <atlas/vga/vga.h>
#include <atlas/ia64/ia64asm.h>
#include <atlas/standard/string.h>
#include <atlas/kmalloc.h>

#define VGA_SET_CURSOR(vga, x, y) vga->cursor_x = x; vga->cursor_y = y; vga_update_cursor();
#define CRET(vga) vga->cursor_x = 0; vga_update_cursor(vga);
#define LNFEED(vga) vga->cursor_y++; if(vga->cursor_y >= vga->height) vga_scroll(1); else vga_update_cursor();
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
	vga_putc(' '); \

static vga_instance* kvga;

void init_vga(size_t width, size_t height)
{
	kvga = (vga_instance*)kmalloc(sizeof(vga_instance));
    kvga->width = width;
    kvga->height = height;
}

void free_vga()
{
	kfree(kvga);
}

void set_vga_colors(vga_color fg, vga_color bg)
{
    kvga->raw_fg = fg;
    kvga->raw_fg = bg;
    kvga->new_color = vga_create_color(fg, bg);
}

void vga_enable_cursor()
{
    kvga->bcursor = true;
    CPUOUTB(0x3D4, 0x0A);
    CPUOUTB(0x3D5, (CPUINB(0x3D5) & 0xC0) | 0x0);
    CPUOUTB(0x3D4, 0x0B);
    CPUOUTB(0x3D5, (CPUINB(0x3D5) & 0xE0) | 0x1);
}

void vga_disable_cursor()
{
    kvga->bcursor = false;
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

void clear_vga()
{
    uint16_t* buff = VGA_BUFFER;
    for(int x = 0; x < kvga->width; x++)
    {
        for(int y = 0; y < kvga->height; y++)
        {
            size_t index = y * kvga->width + x;
            buff[index] = vga_char(' ', kvga->new_color); 
        }
    }
    kvga->cursor_x = 0;
    kvga->cursor_y = 0;
    vga_update_cursor();
}

void vga_update_cursor()
{
    size_t index = kvga->cursor_y * kvga->width + kvga->cursor_x;
    CPUOUTB(0x3D4, 0xE);
    CPUOUTB(0x3D5, (uint8_t)(index >> 8));
    CPUOUTB(0x3D4, 0xF);
    CPUOUTB(0x3D5, (uint8_t)(index));
}

void write_char_vga(char ch, uint32_t x, uint32_t y)
{
    vga_writeptr(vga_char(ch, kvga->new_color), x, y);
}

void vga_writeptr(uint16_t v, uint32_t x, uint32_t y)
{
    uint32_t index = y * kvga->width + x;
    uint16_t* buff = VGA_BUFFER;
    buff[index] = v;
}

void vga_putc(char ch)
{
    switch(ch)
    {
	case '\0': return;
	case '\t':
		for(int i = 0; i < 4; i++)
		{
			vga_putc(' ');
			return;
		}
	case '\r':
		CRET(kvga);
		return;
	case '\n':
		LNFEED(kvga);
		return;
	case '\b':
		BACKSPACE(kvga);
		return;
	default:
		write_char_vga(ch, kvga->cursor_x++, kvga->cursor_y);
		vga_update_cursor();
		return;
	}
}

void vga_puts(char* str)
{
	size_t len = strlen(str);
	int i;
	for(i = 0; i < len; i++) vga_putc(str[i]);
}

void vga_scroll(uint32_t count)
{
	int i, x, y;
	for(i = 0; i < count; i++)
	{
		for(y = 0; y < kvga->height - 1; y++)
		{
			for(x = 0; x < kvga->width; x++) vga_writeptr(vga_read_char(x, y + 1), x, y);	
		}
	}
	for(x = 0; x < kvga->width; x++) vga_writeptr(' ', x, kvga->height - 1);

	if(count > kvga->cursor_y) 
	{
		VGA_SET_CURSOR(kvga, 0, 0);
	}
	else
	{
		VGA_SET_CURSOR(kvga, kvga->cursor_x, kvga->cursor_y - 1)
	}
}

char vga_read_char(uint32_t x, uint32_t y)
{
	uint32_t index = y * kvga->width + x;
	uint16_t* buff = VGA_BUFFER;
	return buff[index] & 0xFF;
}
