#!/bin/bash

rm -f img/atlas.img

./mkbcfs/mkbcfs img/atlas.img ./bcfs 1044480

dd status=noxfer conv=notrunc if=bootloader/init.bin of=img/atlas.img || exit
