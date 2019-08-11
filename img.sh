#!/bin/bash

rm -f img/atlas.img

dd if=/dev/zero of=img/atlas.img bs=1M count=100 && sync
mkfs.ext4 img/atlas.img

dd status=noxfer conv=notrunc if=src/bootloader/init.bin of=img/atlas.img || exit

rm -rf tmp-loop
mkdir tmp-loop
mount -o loop img/atlas.img tmp-loop

files=$(shopt -s nullglob dotglob; echo ext4/*)
if (( ${#files} ))
then
	cp ext4/*.* tmp-loop
fi

sleep 0.2

umount tmp-loop || exit
rm -rf tmp-loop

