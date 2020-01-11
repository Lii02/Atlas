# ========================================================================
# Beginning of Atlas Kernel Assembly Link Module v1.0
# ========================================================================

.code32								# Code runs in 32 bits
.intel_syntax noprefix				# Use intel syntax instead of AT&T
.text								# Code segment, not represented as data
.org 0x0							# Memory offset is specified during assembling (0x2000)

.global start						# Start is accessible from bootloader

# MEMORY MAPPINGS
# PROCESS               FIRST BYTE		LAST BYTE		SIZE (B)
# ------------------------------------------------------------------------
# DESCRIPTOR TABLES		0x0				0x1FFF 			8192 (approx. 1KB)
# KERNEL				0x2000		 	0xFFFFF			1040383 (approx. 1MB)
# STACK					0x100000	 	0x1FFFFF 		1048575 (approx. 1MB)

# ========================================================================
# Kernel Entry Point

start:							# Called from bootloader; initiates kernel
	xor eax,eax						# Zero EAX
	call kernel_init				# Jump to function in C kernel
	
	jmp $							# In the event of kernel exit, hang CPU

# ========================================================================
# Kernel Inclusions

# START OF C KERNEL BINARY DIRECTIVE

# ========================================================================
# End of Atlas Kernel Assembly Link Module v1.0
# ========================================================================
