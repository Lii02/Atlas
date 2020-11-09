#include <atlas/vga/vga.h>
#include <atlas/i386/i386asm.h>
#include <atlas/asm/irq.h>
#include <libc/stdio.h>
#include <atlas/ata.h>
#include <atlas/fs/bcfs.h>
#include <atlas/fs/vfs.h>
#include <libc/string.h>
#include <libc/malloc.h>

#define WIDTH 80
#define HEIGHT 25

void kernel_main()
{
	// initialize the kernel
	isr_install();
	ata_initialize();
	init_bcfs();

	init_vga(WIDTH, HEIGHT);
	init_stdout(WIDTH * HEIGHT, vga_putc);
	set_vga_colors(VGA_LIGHT_GREY, VGA_BLACK);
	clear_vga();
	vga_disable_cursor();
	
	puts("printf test\n");

	printf("%s\n", primary_volume_m.ata_device->identity.model);
	char buff[8];
	itoa(primary_volume_m.sig, buff, 10);
	printf("signature: %s\n", buff);
	itoa(primary_volume_m.inode_count, buff, 10);
	printf("inode count: %s\n", buff);
	itoa(primary_volume_m.major_version, buff, 10);
	printf("major: %s\n", buff);
	itoa(primary_volume_m.minor_version, buff, 10);
	printf("minor: %s\n", buff);
	itoa(primary_volume_m.block_count, buff, 10);
	printf("block count: %s\n", buff);
	itoa(primary_volume_m.block_size, buff, 10);
	printf("block size: %s\n", buff);
	printf("volume name: %s\n", primary_volume_m.volume_name);
	printf("mount dir: %s\n", primary_volume_m.mount_dir);
	itoa(primary_volume_m.root_inode_index, buff, 10);
	printf("root inode index: %s\n", buff);
	itoa(primary_volume_m.inode_size, buff, 10);
	printf("root inode size: %s\n", buff);	
	free_vga();
	free_stdout();
	free_bcfs();
}
