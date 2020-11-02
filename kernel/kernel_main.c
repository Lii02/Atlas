#include <atlas/vga/vga.h>
#include <atlas/standard/malloc.h>

void kernel_main()
{
    vga_instance vga = init_vga(80, 25);
    set_vga_colors(&vga, VGA_LIGHT_GREY, VGA_BLACK);
}
