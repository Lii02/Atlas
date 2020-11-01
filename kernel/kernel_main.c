#include "vga/vga.h"

void kernel_main()
{
	vga_instance vga = init_vga(80, 25);
	set_vga_colors(&vga, VGA_BLUE, VGA_BLACK);
}