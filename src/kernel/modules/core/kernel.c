/*
 * Atlas Kernel main file
 * 	- Atlas Team 2020
 */

#include "atlas_layer.h"
#include "atlas_std.h"
#include "vga.h"

static atlas_vga_terminal terminal;

void kernel_init()
{
    terminal = terminal_initialize(VGA_COLOR_BLACK, VGA_COLOR_WHITE, 80, 24);
    terminal_writestring(&terminal, "Atlas OS: by Atlas Team");
    terminal_writestring(&terminal, "AAAAAAAAAAAAA");
    int x = 2;
}

void kernel_clean_up()
{
    free_terminal(&terminal);
}