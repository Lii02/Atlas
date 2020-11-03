#include <atlas/vga/vga.h>
#include <atlas/ia64/ia64asm.h>
#include <atlas/kmalloc.h>
#include <atlas/memory.h>

void kernel_main()
{
    vga_instance vga = init_vga(80, 25);
    set_vga_colors(&vga, VGA_LIGHT_GREY, VGA_BLACK);
}
