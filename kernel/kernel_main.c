#include <atlas/vga.h>
#include <atlas/i386/i386asm.h>
#include <atlas/asm/irq.h>
#include <libc/stdio.h>
#include <atlas/ata.h>
#include <atlas/fs/bcfs.h>
#include <libc/string.h>
#include <libc/malloc.h>
#include <atlas/fs/bcfs_inode.h>
#include <atlas/fs/bcfs_table.h>

#define WIDTH 80
#define HEIGHT 25

void kernel_main()
{
	// initialize the kernel
	isr_install();	
	ata_initialize();
	init_bcfs();

	init_vga(WIDTH, HEIGHT);
	set_vga_colors(VGA_LIGHT_GREY, VGA_BLACK);
	clear_vga();
	vga_disable_cursor();

	vga_puts("atlas: Initializing memory paging...\n");
	init_memory_paging();
	vga_puts("atlas: Finished initializing paging!\n");

	/*
	bcfs_table* table0 = bcfs_init_table(0);
	bcfs_fill_inode_table(&primary_volume_m, &table0, 4, 0);	
	*/
	
	vga_puts("puts test\n\n");
	
	/*
	for(int i = 0; i < primary_volume_m.inode_count; i++)
	{
		char buff[8];
		int index = i;
		bcfs_inode_t* inode = table0->inodes[index];
	
		itoa(index, buff, 10);
		puts("inode index: ");
		puts(buff);
		putchar('\n');

		itoa(inode->link_type, buff, 10);
		puts("inode link: ");
		puts(buff);
		putchar('\n');
	}

	bcfs_free_table(table0);
	*/

	free_vga();
	free_bcfs();
}
