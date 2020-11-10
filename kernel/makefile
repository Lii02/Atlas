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
CFLAGS := -DI386=1 -w -fno-builtin -fno-pie -ffreestanding -nostdinc -m32 $(KERNELHDRS)

OBJS = \
kernel.o \
kernel_main.o \
i386/i386asm.o \
vga.o \
fs/bcfs_inode.o \
fs/bcfs_vol.o \
asm/irqbackend.o \
asm/irq.o \
keyboard.o \
ata.o \
fs/bcfs.o \
fs/vfs.o \

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

clean:
	$(RM) -f *.o
	$(RM) -f *.elf
	$(RM) -f i386/*.o
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
	
keyboard.o: keyboard.c
	$(CC) $(CFLAGS) -o keyboard.o -c keyboard.c
	
ata.o: ata.c
	$(CC) $(CFLAGS) -o ata.o -c ata.c

# Intel specific code

i386/i386asm.o: i386/i386asm.c
	$(CC) $(CFLAGS) -o i386/i386asm.o -c i386/i386asm.c 

# VGA code

vga.o: vga.c
	$(CC) $(CFLAGS) -o vga.o -c vga.c 

# BCFS filesystem code

fs/bcfs_inode.o: fs/bcfs_inode.c
	$(CC) $(CFLAGS) -o fs/bcfs_inode.o -c fs/bcfs_inode.c

fs/bcfs_vol.o: fs/bcfs_vol.c
	$(CC) $(CFLAGS) -o fs/bcfs_vol.o -c fs/bcfs_vol.c
	
fs/bcfs.o: fs/bcfs.c
	$(CC) $(CFLAGS) -o fs/bcfs.o -c fs/bcfs.c
	
fs/vfs.o: fs/vfs.c
	$(CC) $(CFLAGS) -o fs/vfs.o -c fs/vfs.c

# ASM code

asm/irqbackend.o: asm/irqbackend.S
	$(AS) --32 -o asm/irqbackend.o asm/irqbackend.S

asm/irq.o: asm/irq.c
	$(CC) $(CFLAGS) -o asm/irq.o -c asm/irq.c