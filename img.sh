#!/bin/bash

rm -f img/atlas.img

dd if=/dev/zero of=img/atlas.img bs=1M count=100 && sync
mkfs.ext2 img/atlas.img

dd status=noxfer conv=notrunc if=src/bootloader/init.bin of=img/atlas.img || exit

rm -rf tmp-loop
mkdir tmp-loop
mount -o loop img/atlas.img tmp-loop

files=$(shopt -s nullglob dotglob; echo ext2/*)
if (( ${#files} ))
then
	cp ext2/*.* tmp-loop
fi

sleep 0.2

umount tmp-loop || exit
rm -rf tmp-loop

