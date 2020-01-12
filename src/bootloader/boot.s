# ========================================================================
# Beginning of Atlas Second-Stage Bootloader v1.0
# ========================================================================

.code16								# Code runs in 16 bits
.intel_syntax noprefix				# Use intel syntax instead of AT&T
.text								# Code segment, not represented as data
.org 0x0							# Memory offset is specified during assembling (0x8000)

# ========================================================================
# Boot Entry Point

boot_call_vector:				# Call vector for loading from bootoader
	jmp boot_main					# 0000h -- Called from bootloader

# ========================================================================
# Boot Main Routines

boot_main:						# OS Main func, called from bootloader
	cli								# Clear interrupts
	mov ax,0						# Load 0 to AX
	mov ss,ax						# Set stack segment pointer
	mov sp,0x7FF0					# Set stack pointers
	sti								# Restore interrupts
	cld								# Clear direction flag
	
	mov ax,0						# Set all segments to current memory offset
	mov ds,ax						# Set data segment offset
	mov es,ax						# Set extra segment offset
	
	call clearsc					# Clear the screen

	lea si,str_strt					# Load start string to SI
	call puts						# Print message to screen

	call read_kernel				# Read kernel from disk
	
	call enable_A20					# Attempt to enable the A20 line
	
	call define_GDT					# Create global descriptor table
	lgdt gdt_r						# Link GDT to the processor
	call define_IDT					# Create interrupt descriptor table
	lidt idt_r						# Link IDT to the processor
	
	cli								# Disable interrupts
	call enable_pm					# Enable protected mode
	call cl_prefetch				# Clear the CPU prefetch queue
	
	mov ax,0x10						# Set data segments to second GDT selector
	mov ds,ax						# DS = 0x10
	mov es,ax						# ES = 0x10
	
	jmp exec_kernel					# Execute the kernel

# ========================================================================
# 32-Bit Boot Subroutines

.func fatal_err					# Function: fatal_err
fatal_err:						# Handle fatal error
	lea si,str_reboot				# Prepare to print string
	call putsln						# Print message to screen
	xor ax,ax						# Zero AX
	int 0x16						# Wait for key press
	xor ax,ax						# Zero AX
	int 0x19						# Reboot the system
.endfunc

.func enable_A20				# Function: enable_A20
enable_A20:						# Try all methods to enable A20 line
	call verify_A20					# Check if already enabled
	jc _enable_A20_success			# If so, skip to end
	call activate_A20_bios			# Attempt to enable A20 through BIOS
	call verify_A20					# Test A20 state
	jc _enable_A20_success			# If set, skip to end
	call activate_A20_keyctrl		# Attempt to enable A20 through keyboard ctrl
	call verify_A20					# Test A20 state
	jc _enable_A20_success			# If set, skip to end
	call activate_A20_FA20			# Attempt to enable A20 through FA20 method
	call verify_A20					# Test A20 state
	jc _enable_A20_success			# If set, skip to end
	
	lea si,str_A20err				# Prepare to print error message
	call putsln						# Print message
	jmp fatal_err					# Handle fatal error
_enable_A20_success:			# Called when A20 was enabled successfully
	ret								# Return to call location
.endfunc

.func exec_kernel				# Function: exec_kernel
exec_kernel:					# Execute kernel
	.byte 0x66						# Opcode
	.byte 0xEA						# Opcode
	.int  0x2000					# Offset
	.word 0x0008					# Selector
	# END OF EXECUTION
.endfunc

# ========================================================================
# Boot Descriptor Location Identifiers

gdt_r:							# GDT location identifier
	.word 24						# Size of GDT (in bytes)
	.int  0x800						# Memory location of GDT
idt_r:							# IDT location identifier
	.word 2048						# Size of IDT (in bytes)
	.int  0x0						# Memory location of IDT

# ========================================================================
# Boot Messages

str_strt:	.asciz "BOOT.BIN successfully executed"
str_reboot:	.asciz "Press any key to reboot..."
str_A20err: .asciz "32-bit addressing is not supported by host processor"

# ========================================================================
# Bootloader External Definitions

.include "modules/io_routine.s"
.include "modules/descriptor.s"
.include "modules/A20_routine.s"
.include "modules/prot_mode.s"
.include "modules/sysdisk.s"

# ========================================================================
# End of Atlas Second-Stage Bootloader v1.0
# ========================================================================