# -------------------------------------------------------------------
# Beginning of Atlas kernel makefile
# -------------------------------------------------------------------

# Command specifications (AS = assembler, RM = remove, CC = complier, LD = linker)

AS := as
RM := rm
CC := gcc
LD := ld
KERNELHDRS := -I../include/
CFLAGS := -DIA64=1 -w -fno-builtin -fno-pie -nostdlib -ffreestanding -nostdinc -m32 $(KERNELHDRS)

# When adding new module C/S files, add them as entries to the bottom of this file.
# Use the following templates.

# For assembly (.s) files:
# FILENAME.o: FILENAME.s
# 	$(AS) --32 -o FILENAME.o FILENAME.s

# For C (.c) files:
# FILENAME.o: FILENAME.c
#	$(CC) $(CFLAGS) -o FILENAME.o -c FILENAME.c

# Header (.h) files contain only declarations and are included by C files and
# therefore don't need to be compiled directly.

# IMPORTANT! Add all new module C/S files to this dependency list and to the compile list
# on the kernel.elf line.

clear:
	$(RM) -f *.o
	$(RM) -f *.elf
	$(RM) -f *.o
	$(RM) -f ia64/*.o
	$(RM) -f vga/*.o
	$(RM) -f fs/*.o
	$(RM) -f asm/*.o

all: ../bcfs/kernel.prg

../bcfs/kernel.prg: kernel.elf
	objcopy -O binary kernel.elf ../bcfs/kernel.prg

kernel.elf: kernel.o kernel_main.o ia64/ia64asm.o vga/vga.o kmalloc.o fs/bcfs_inode.o fs/bcfs_vol.o memory.o asm/irqbackend.o asm/irq.o
	$(CC) $(CFLAGS) -T link.ld -o kernel.elf kernel.o kernel_main.o ia64/ia64asm.o vga/vga.o kmalloc.o fs/bcfs_inode.o fs/bcfs_vol.o memory.o asm/irqbackend.o asm/irq.o -Ttext 0x2000

kernel.o: kernel.s
	$(AS) --32 -o kernel.o kernel.s

kernel_main.o: kernel_main.c
	$(CC) $(CFLAGS) -o kernel_main.o -c kernel_main.c
	
kmalloc.o: kmalloc.c
	$(CC) $(CFLAGS) -o kmalloc.o -c kmalloc.c
	
memory.o: memory.c
	$(CC) $(CFLAGS) -o memory.o -c memory.c

# Intel specific code

ia64/ia64asm.o: ia64/ia64asm.c
	$(CC) $(CFLAGS) -o ia64/ia64asm.o -c ia64/ia64asm.c 

# VGA code

vga/vga.o: vga/vga.c
	$(CC) $(CFLAGS) -o vga/vga.o -c vga/vga.c 

# BCFS filesystem code

fs/bcfs_inode.o: fs/bcfs_inode.c
	$(CC) $(CFLAGS) -o fs/bcfs_inode.o -c fs/bcfs_inode.c

fs/bcfs_vol.o: fs/bcfs_vol.c
	$(CC) $(CFLAGS) -o fs/bcfs_vol.o -c fs/bcfs_vol.c

# ASM code

asm/irqbackend.o: asm/irqbackend.S
	$(AS) --32 -o asm/irqbackend.o asm/irqbackend.S

asm/irq.o: asm/irq.c
	$(CC) $(CFLAGS) -o asm/irq.o -c asm/irq.c
