org 0x7c00

init:
  jmp main

main:
  jmp $

times 510 - ($ - $$) db 0
dw 0xAA55
