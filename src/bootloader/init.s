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
	lea ax,disk_buffer		# Load address of superblock label into AX
	mov sector_adr,ax		# Store superblock address into destination offset
	call read_disk			# Read the disk using the read packet
	
	mov ax,[offset disk_buffer+56]	# Read the 56th byte of superblock (verification ID)
	cmp ax,0xEF53			# Does this byte match ext2 verification ID?
	jne fatal_disk_err		# If not, handle fatal disk error
	
	lea bx,[offset disk_buffer+32]	# Load address of blocks_per_block_group into BX
	call load_i32			# Load that value into EAX
	mov bx,offset block_qty		# Set BX to block_qty variable location
	call store_i32			# Store value that was in EAX
	
	lea bx,[offset disk_buffer+40]	# Load address of inodes_per_block_group into BX
	call load_i32			# Load the value into EAX
	mov bx,offset inode_qty		# Set BX to inode_qty varible location
	call store_i32			# Store value that was in EAX
	
	lea bx,[offset disk_buffer+24]	# Load address of log2(block_size)-10 into BX
	call load_i32			# Load that value into EAX
	shl eax,10			# Shift left by 1024 to get actual block size
	mov bx,offset block_size	# Set BX to block_size variable location
	call store_i32			# Store block size that was in EAX

# ========================================================================
# Primary filesystem routines

.func read_inode		# Function: read_inode
read_inode:			# Reads an inode into memory with index ECX
	# Calculate block group containing an inode by:
	# block_group = (inode_index - 1) / inodes_per_group
	
	mov eax,ecx		# Load inode index into EAX
	sub eax,1		# Subtract 1 from EAX
	mov ebx,inode_qty	# Load inodes per group into EBX
	mov edx,0		# Make sure that there is no segment during division (EDX:EAX)
	div ebx			# Divide EAX by EBX and store result in EAX
	
	

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

.func store_i32			# Function: store_i32 (uses little-endian addressing)
store_i32:			# Stores a 32-bit integer in EAX to memory location 7C00:BX
	push ax				# Preserve the value of AX (Cannot push 32-bit registers)
	mov [bx],ax			# Load low end of EAX into memory location
	shr eax,2			# Shift EAX to the right (get high end into AX)
	mov [bx+2],ax			# Load high end into memory location + 2
	shl eax,2			# Restore the high end of EAX
	pop ax				# Restore the low end of EAX
	ret				# Return to call location
.endfunc

.func load_i32			# Function: load_i32 (uses little-endian addressing)
load_i32:			# Loads a 32-bit integer into EAX from memory location 7C00:BX
	xor eax,eax			# Set AX equal to 0
	mov ax,[bx+2]			# Load the high end of integer into AX
	shl eax,2			# Shift AX into high end of EAX
	mov ax,[bx]			# Load low end of integer ino AX
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

block_qty:	.word 0x0		# Number of blocks per block group
		.word 0x0		# Value is a 4 byte (32 bit) integer
inode_qty:	.word 0x0		# Number of inodes per block group
		.word 0x0		# Value is a 4 byte (32 bit) integer
block_size:	.word 0x0		# Size (in bytes) of each block
		.word 0x0		# Value is a 4 byte (32 bit) integer

# ========================================================================
# Device read packet

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

disk_buffer:				# Label at which all disk reads are loaded into memory

# ========================================================================
# End of Atlas Initial Bootloader v1.0
# ========================================================================
