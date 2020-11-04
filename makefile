# -------------------------------------------------------------------
# Beginning of Atlas image makefile
# -------------------------------------------------------------------

# Command specifications (AS = assembler, RM = remove, CC = complier, LD = linker)

AS := as
RM := rm
CC := gcc
LD := ld
CFLAGS := -Wall -fno-builtin -fno-pie -nostdlib -ffreestanding -nostdinc -m32

# IMPORTANT! Any other makefiles that need to be run to create binaries in the bcfs
# folder should be added as a dependency to the 'all' entry before 'img/atlas.img'
# Additionally, you must create an entry for that makefile following this template.

# bcfs/FILENAME.EXTENSION:
# 	$(MAKE) -C FOLDER_CONTAINING_MAKEFILE/ ../../bcfs/FILENAME.EXTENSION

#                  ||
#                  ||
# Insert here      \/
all: bcfs/libc.al bcfs/kernel.sys bcfs/boot.bin img/atlas.img

img/atlas.img: bootloader/init.bin
	bash img.sh

bootloader/init.bin:
	$(MAKE) -C bootloader/ init.bin

bcfs/boot.bin:
	$(MAKE) -C bootloader/ boot.bin

bcfs/libc.al:
	$(MAKE) -C libc/ ../bcfs/libc.al

bcfs/kernel.sys:
	$(MAKE) -C kernel/ ../bcfs/kernel.sys

clearsrc:
	$(MAKE) -C bootloader/ clear
	$(MAKE) -C kernel/ clear
	$(MAKE) -C libc/ clear

clear:
	$(RM) -f bcfs/*.sys
	$(RM) -f bcfs/*.bin
	$(RM) -f bcfs/*.al
	$(RM) -f img/*.img
	$(RM) -f libc/*.al
	$(MAKE) -C bootloader/ clear
	$(MAKE) -C kernel/ clear
	$(MAKE) -C libc/ clear