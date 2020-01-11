#!/bin/bash

rm -f img/atlas.img

./mkbcfs/mkbcfs img/atlas.img bcfs 1044480
# ./mkbcfs/mkbcfs img/atlas.img bcfs 10653696

dd status=noxfer conv=notrunc if=src/bootloader/init.bin of=img/atlas.img || exit

