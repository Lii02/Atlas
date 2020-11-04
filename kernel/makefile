# -------------------------------------------------------------------
# Beginning of Atlas kernel makefile
# -------------------------------------------------------------------

# Command specifications (AS = assembler, RM = remove, CC = complier, LD = linker)

AS := as
RM := rm
CC := gcc
LD := ld
KERNELHDRS := -I../include/
KERNELLIB := -L../libc/
CFLAGS := -DIA64=1 -w -fno-builtin -fno-pie -ffreestanding -nostdinc -m32 $(KERNELHDRS)

OBJS = \
kernel.o \
kernel_main.o \
i386/i386asm.o \
vga/vga.o \
fs/bcfs_inode.o \
fs/bcfs_vol.o \
asm/irqbackend.o \
asm/irq.o

LIBS = -nostdlib
LINK_LIST = \
	$(LIBS) \
	$(OBJS) \

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
	$(RM) -f i386/*.o
	$(RM) -f vga/*.o
	$(RM) -f fs/*.o
	$(RM) -f asm/*.o

all: ../bcfs/kernel.sys

../bcfs/kernel.sys: kernel.elf
	objcopy -O binary kernel.elf ../bcfs/kernel.sys

kernel.elf: $(OBJS)
	$(LD) -m elf_i386 -T link.ld -o kernel.elf -Ttext 0x2000 $(LINK_LIST) ../libc/libc.a

kernel.o: kernel.s
	$(AS) --32 -o kernel.o kernel.s

kernel_main.o: kernel_main.c
	$(CC) $(CFLAGS) -o kernel_main.o -c kernel_main.c

# Intel specific code

i386/i386asm.o: i386/i386asm.c
	$(CC) $(CFLAGS) -o i386/i386asm.o -c i386/i386asm.c 

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