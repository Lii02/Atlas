.code32
.intel_syntax noprefix
.text

.global set_as_vga
.func set_as_vga
set_as_vga:
	mov ah, 0
	mov al, 0
	int 0x10
    ret
.endfunc