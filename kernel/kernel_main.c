#include <atlas/vga/vga.h>
#include <atlas/ia64/ia64asm.h>
#include <atlas/kmalloc.h>
#include <atlas/memory.h>
#include <atlas/asm/irq.h>

void keyboard_interrupt(cpuregisters_t r)
{
	char key = CPUINB(0x60);
	vga_putc(key);
}

void kernel_main()
{
	// initialize the kernel	
	isr_install();
	initialize_interrupt(IRQ1, keyboard_interrupt);

    init_vga(80, 25);
    set_vga_colors(VGA_LIGHT_GREY, VGA_BLACK);
	clear_vga();

	free_vga();
}
