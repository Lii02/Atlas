#include "vga/vga.h"

void kernel_main()
{
    vga_instance vga = init_vga(80, 25);
    set_vga_colors(&vga, VGA_LIGHT_GREY, VGA_BLACK); 
	
	vga_puts("Hello World\n\r", &vga);
}
