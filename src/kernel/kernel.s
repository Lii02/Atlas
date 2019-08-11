.code32
.intel_syntax noprefix
.text

.global start

start:
	lea esp,sys_stack
	call start_kernel

.section .bss
	.lcomm buff, 0x1000

sys_stack:

.section .data
	sec_id: .ascii "DATA SECTION"
