# ========================================================================
# Beginning of Atlas Initial Bootloader v1.0
# ========================================================================

.code16					# Code runs in 16 bits
.intel_syntax noprefix			# Use intel syntax instead of AT&T
.text					# Code segment, not represented as data
.org 0x0				# Memory offset is specified during assembling

.global main				# Main is accessible from BIOS

# ========================================================================
# Bootloader entry and initialization

main:				# Bootloader entry point
	jmp short boot			# Jumps to boot label (begins boot process)
	nop				# No operation, seperates main from boot label

boot:				# Primary bootloader boot funcion
	and dl,0x80			# Is the boot device valid?
	jnz skip_boot_repair		# If so, skip boot repair
	
	lea si,boot_repair		# Load repair message into string pointer
	call puts			# Print message to screen
	mov dl,0x80			# Set DL to default boot device
	
skip_boot_repair:		# Location to skip boot repair
	mov bootdev,dl			# Load boot device into memory
	lea ax,superblock		# Load address of superblock label into AX
	mov sector_adr,ax		# Store superblock address into destination offset
	call read_disk			# Read the disk using the read packet
	
	mov ax,[offset superblock+56]	# Read the 56th byte of superblock (verification ID)
	cmp ax,0xEF53			# Does this byte match ext2 verification ID?
	jne fatal_disk_err		# If not, handle fatal disk error
	
	jmp $				# Still working on bootloader

# ========================================================================
# Initial bootloader BIOS subroutines

.func puts			# Function: puts
puts:				# Prints string pointed to by SI (0 terminated)
	pusha				# Preserve all registers on the stack
	mov ah,0x0e			# Set BIOS subinterrupt routine to VGA print
_puts_loop:			# Loop to print each character one by one
	lodsb				# Load character at SI into AL; inc SI
	cmp al,0			# Is the string ended (0 terminated)
	je _puts_end			# If so, jump to _puts_end
	int 0x10			# Run BIOS video service interrupt routine
	jmp _puts_loop			# Loop back to print next character
_puts_end:			# Ends printing procedure
	popa				# Restore all registers from the stack
	ret				# Return to call location
.endfunc

.func read_disk			# Function: read_disk
read_disk:			# Performs a BIOS disk read using packet specified at read_pckt
	pusha				# Preserve all registers on the stack
	lea si,read_pckt		# Load read packet into pointer for BIOS
	mov dl,bootdev			# Load device 0 into DL (or whichever device is at 0x80)
	mov ah,0x42			# Set BIOS subinterrupt routine for extensive disk reading
	int 0x13			# Execute BIOS disk service interrupt routine
	jc fatal_disk_err		# Is carry flag set (set on error)? If so, handle disk error
	popa				# Restore all registers from the stack
	ret				# Return to call location
.endfunc

fatal_disk_err:			# Prints a fatal disk error message and reboots
	lea si,disk_err			# Load message into string pointer
	call puts			# Print message to screen
	jmp reboot			# Reboot the machine

reboot:				# Reboots the (virtual) machine
	mov ax,0			# Load subinterrupt routine to read char
	int 0x16			# Execute keyboard service interrupt routine
	mov ax,0			# Load reboot subinterrupt routine
	int 0x19			# Execute power control service interrupt routine

# ========================================================================
# Boot device data

bootdev:	.byte 0x0		# Stores drive number (BIOS stores it in DL)

read_pckt:	.byte 0x10		# Data packet for BIOS to read superblock (size of pckt is 16)
		.byte 0x0		# Unused (always 0)
sector_qty:	.word 0x2		# Read 2 sectors (1024 bytes of superblock)
sector_adr:	.word 0x0		# Destination memory offset
		.word 0x0		# Destination memory segment (set to 0x7C00 by ORG)
sector_lba:	.word 0x2		# First sector to read from (specified in LBA)
		.word 0x0		# Superblock exists after 1024 byte boot sector
		.word 0x0		# Boot sector occupies LBA 1 & 2
		.word 0x0

# ========================================================================
# Boot messages

boot_repair:	.asciz "atlas: Repairing invalid boot device"
search_err:	.asciz "atlas: Cannot find BOOT.SYS"
disk_err:	.asciz "atlas: Fatal disk read error"

# ========================================================================
# Boot sector partitioning

.fill (510-(.-main)), 1, 0		# Pad unused bootloader space with 0s (up to 510 bytes)
bootSignature: .int 0xAA55		# Set last to bytes of bootloader to 55AA (boot signature)

superblock:				# Label at which the superblock is loaded into memory

# ========================================================================
# End of Atlas Initial Bootloader v1.0
# ========================================================================
