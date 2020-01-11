#include "vga.h"

unsigned int vga_entry_color(vga_color back, vga_color fore)
{
    return fore | back << 4;
}

unsigned int vga_entry(uchar c, unsigned int color)
{
    return (unsigned int) c | (unsigned int) color << 8;
}

atlas_vga_terminal terminal_initialize(unsigned int background, unsigned int foreground, size_t width, size_t height)
{
    atlas_vga_terminal terminal;
    terminal.width = width;
    terminal.height = height;
    terminal.terminal_row = 0;
    terminal.terminal_column = 0;
    terminal.terminal_color = vga_entry_color(background, foreground);
    terminal.terminal_buffer = (unsigned int *) 0xB8000;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            const size_t index = y * width + x;
            terminal.terminal_buffer[index] = vga_entry(' ', terminal.terminal_color);
        }
    }

    return terminal;
}

void terminal_set_color(atlas_vga_terminal *terminal, unsigned int color)
{
    terminal->terminal_color = color;
}

void terminal_putentry(atlas_vga_terminal *terminal, char c, unsigned int color, unsigned int x, unsigned int y)
{
    const unsigned int index = y * terminal->width + x;
    terminal->terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(atlas_vga_terminal *terminal, char c)
{
    terminal_putentry(terminal, c, terminal->terminal_color, terminal->terminal_column, terminal->terminal_row);
    if (++terminal->terminal_column == terminal->width)
    {
        terminal->terminal_column = 0;
        if (++terminal->terminal_row == terminal->height)
        terminal->terminal_row = 0;
    }
}

void terminal_write(atlas_vga_terminal *terminal, atlas_string str, unsigned int size)
{
    for(unsigned int i = 0; i < size; i++)
    {
        terminal_putchar(terminal, str[i]);
    }
}

void terminal_writestring(atlas_vga_terminal *terminal, atlas_string str)
{
    terminal_write(terminal, str, strlen(str));
}

void free_terminal(atlas_vga_terminal *terminal)
{
}