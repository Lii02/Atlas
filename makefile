# -------------------------------------------------------------------
# Beginning of Atlas image makefile
# -------------------------------------------------------------------

# Command specifications (AS = assembler, RM = remove, CC = complier, LD = linker)

AS := as
RM := rm
CC := gcc
LD := ld
CFLAGS := -Wall -fno-builtin -fno-pie -nostdlib -ffreestanding -nostdinc -m32

# IMPORTANT! Any other makefiles that need to be run to create binaries in the ext4
# folder should be added as a dependency to the 'all' entry before 'img/atlas.img'
# Additionally, you must create an entry for that makefile following this template.

# ext4/FILENAME.EXTENSION:
# 	$(MAKE) -C src/FOLDER_CONTAINING_MAKEFILE/ ../../ext4/FILENAME.EXTENSION

#                  ||
#                  ||
# Insert here      \/
all: ext4/kernel.sys img/atlas.img

img/atlas.img: src/bootloader/init.bin
	bash img.sh

src/bootloader/init.bin:
	$(MAKE) -C src/bootloader/ init.bin

ext4/kernel.sys:
	$(MAKE) -C src/kernel/ ../../ext4/kernel.sys

clear:
	$(RM) -f ext4/*.*
	$(RM) -f img/*.img
	$(MAKE) -C src/bootloader/ clear
	$(MAKE) -C src/kernel/ clear
