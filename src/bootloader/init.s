.code16
.intel_syntax noprefix
.text
.org 0x0

.global main

main:
	jmp short boot
	nop

boot:
	lea si,msg
	call print
	jmp $

.func print
print:
	pusha
	mov ah,0x0e
_print_loop:
	lodsb
	cmp al,0
	je _print_end
	int 0x10
	jmp _print_loop
_print_end:
	popa
	ret
.endfunc

msg: .asciz "Hello world!"

.fill (510-(.-main)), 1, 0
bootSignature: .int 0xAA55
