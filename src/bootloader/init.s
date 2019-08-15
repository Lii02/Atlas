# ========================================================================
# Beginning of Atlas Initial Bootloader v1.0
# ========================================================================

.code16					# Code runs in 16 bits
.intel_syntax noprefix			# Use intel syntax instead of AT&T
.text					# Code segment, not represented as data
.org 0x0				# Memory offset is specified during assembling (0x7C00)

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
	
	lea bx,disk_buffer		# Load address of superblock into BX
	add bx,32			# Add offset for blocks_per_block group
	call load_i32			# Load that value into EAX
	mov bx,offset block_qty		# Set BX to block_qty variable location
	call store_i32			# Store value that was in EAX
	
	lea bx,disk_buffer		# Load address of superblock into BX
	add bx,40			# Add offset for inodes_per_block_group
	call load_i32			# Load the value into EAX
	mov bx,offset inode_qty		# Set BX to inode_qty varible location
	call store_i32			# Store value that was in EAX
	
	lea bx,disk_buffer		# Load address of superblock into BX
	add bx,24			# Add offset for log2(block_size)-10
	call load_i32			# Load that value into EAX
	shl eax,10			# Shift left by 1024 to get actual block size
	mov bx,offset block_size	# Set BX to block_size variable location
	call store_i32			# Store block size that was in EAX
	
	lea bx,disk_buffer		# Load address of superblock into BX
	add bx,76			# Add offset for major version
	call load_i32			# Load value into EAX
	cmp eax,0			# Is this a version 0 ext2 filesystem?
	je skip_inode_cfg		# If so, skip inode configuration stage
	
	mov ax,[offset disk_buffer+88]	# Load custom inode size into AX
	mov inode_size,ax		# Store custom inode size into variable location
	
	lea bx,disk_buffer		# Load address of superblock into BX
	add bx,100			# Add offset for supported features
	call load_i32			# Put supported features list into EAX
	and eax,0x02			# Is 64-bit file size supported?
	mov ext_size,al			# Store result in variable
skip_inode_cfg:			# Jump point to skip custom inode configuration stage
	jmp $				# Still working on bootloader

# ========================================================================
# Primary filesystem routines

.func read_inode		# Function: read_inode
read_inode:			# Reads an inode into memory with index ECX
	# Calculate block group containing an inode by:
	# block_group = (inode_index - 1) / inodes_per_group
	
	mov eax,ecx			# Load inode index into EAX
	sub eax,1			# Subtract 1 from EAX
	mov ebx,inode_qty		# Load inodes per group into EBX
	mov edx,0			# Make sure that there is no segment during division (EDX:EAX)
	div ebx				# Divide EAX by EBX and store result in EAX
	mov esi,eax			# Temporarily preserve EAX in ESI
	
	call read_descriptor		# Read the descriptor of the given block group
	mov eax,32			# Load 32 into EAX (32 bytes per block group descriptor)
	mov ebx,esi			# Load block group ID into EBX
	mul ebx				# Multiply 32 by block group ID into EAX
	add eax,offset disk_buffer	# Add offset of disk buffer to EAX
	add eax,8			# Starting block address of inode table at byte 8 of descriptor
	mov ebx,[eax]			# Load value into EBX
	mov eax,ebx			# Store value into EAX
	
	call read_bitmap		# Read the inode bitmap into memory
	mov eax,ecx			# Load inode number into EAX
	dec eax				# Inode address starts at 1 (subtract to start at 0)
	mov ebx,inode_size		# Load inode size into EBX
	mul ebx				# Mutliply inode size by inode index (into EAX)
	add eax,offset disk_buffer	# Add disk buffer base (EAX now has address to inode)
	mov esi,eax			# Store address in ESI (pointer register)
	
	mov bl,ext_size			# Load extended size support flags into BL
	cmp bl,0			# Is extended filesize supported?
	je _read_inode_i386		# If not, skip to 32 bit portion of size loading
	
	lea bx,[esi+108]		# Load address of high 32 bits of filesize into BX
	call load_i32			# Load integer into EAX
	mov edx,eax			# Store high 32 bits into EDX
	jmp _read_inode_L0		# We don't want to zero out the high 32 bits
_read_inode_i386:		# Jump point to zero out high 32 bits of filesize
	mov edx,0			# Make sure that EDX is zero for division
_read_inode_L0:			# Jump point to calculate low 32 bits of filesize
	lea bx,[esi+4]			# Load address of low 32 bits of filesize into BX
	call load_i32			# Load integer into EAX (total filesize is now EDX:EAX)
	
	mov edi,eax			# Preserve low 32 bits of filesize in EDI (EAX will be used)
	mov bx,offset block_size	# Load address of block_size into BX
	call load_i32			# Load block size into EAX
	mov ebx,eax			# Load block size into EBX
	mov eax,edi			# Restore low 32 bits of filesize from EDI into EAX
	div ebx				# Divide filesize by block size to get total block count
	
	cmp edx,0			# Is there no remainder?
	je _read_inode_skip		# If so, skip the rounding up process
	
	inc eax				# If there is a partial block, we must include that as well
_read_inode_skip:		# Jump point to skip the rounding up process if it is not necessary
	
.endfunc

.func read_descriptor		# Function: read_descriptor
read_descriptor:		# Reads the block group descriptor into memory until block group EAX
	inc eax				# Make sure to read through block group entry, not up to it
	mov sector_lba,word ptr 0x4	# Table begins at the sector #4
	mov ebx,32			# Each table entry is 32 bytes
	mul ebx				# Multiply by sector count to get number of bytes to read
	
	xor edx,edx			# Make sure that EDX is 0 so we don't have a segment
	mov ebx,512			# Each sector is 512 bytes
	div ebx				# Divide byte count by sector length to get sector count
	cmp edx,0			# Is there no remainder?
	je _read_descriptor_skip	# If so, skip rounding up process
	
	inc eax				# Round up EAX
_read_descriptor_skip:		# Jump point to skip the rounding up process
	mov sector_qty,ax		# Load number of sectors to read into packet
	call read_disk			# Read disk
	ret				# Return to call location
.endfunc

.func read_bitmap		# Function: read_bitmap
read_bitmap:			# Reads the inode bitmap given a block address (EAX) up to inode (ECX)
	mov esi,eax			# Temporarily store block address in ESI
	lea bx,block_size		# Load the address of block_size variable into BX
	call load_i32			# Load block_size into EAX
	mov ebx,esi			# Load the block address into EBX
	mul ebx				# Multiply block address by block size to get byte address
	
	xor edx,edx			# Make sure EDX is 0 (so that it doesn't interfere with div)
	mov ebx,512			# Store 512 in EBX (each sector is 512 bytes)
	div ebx				# Divide byte address by sector count to get sector number
	lea bx,sector_lba		# Put address of sector_lba into BX
	call store_i32			# Store sector number in packet variable for starting sector
	
	mov eax,ecx			# Put inode index into EAX
	xor ebx,ebx			# Zero out high end of EBX
	mov bx,inode_size		# Put the inode size into BX
	mul ebx				# Multiply inode size by inode index to get offset from table
	
	xor edx,edx			# Make sure EDX is 0 (so that it doesn't interfere with div)
	mov ebx,512			# Store 512 in EBX (each sector is 512 bytes)
	div ebx				# Divide table offset by 512 to get sector offset count
	cmp edx,0			# Is there no remainder?
	je _read_bitmap_skip		# If so, skip rounding up process
	
	inc eax				# Round up (to ensure that the required table index is read)
_read_bitmap_skip:		# Jump point to skip the rounding up process
	mov sector_qty,ax		# Load number of sectors to read into the packet
	call read_disk			# Read the disk
	ret				# Return to call location
.endfunc

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
inode_size:	.word 128		# Size of each inode (default 128)
ext_size:	.byte 0x0		# Is 64-bit file size supported?

# ========================================================================
# Device read packet

read_pckt:	.byte 0x10		# Data packet for BIOS to read superblock (size of pckt is 16)
		.byte 0x0		# Unused (always 0)
sector_qty:	.word 0x2		# Read 2 sectors (1024 bytes of superblock)
sector_adr:	.word 0x0		# Destination memory offset
		.word 0x0		# Destination memory segment (set to 0x7C00 by ORG)
sector_lba:	.word 0x2		# First sector to read from (specified in LBA)
		.word 0x0		# Superblock exists after 1024 byte boot sector
		.word 0x0		# Boot sector occupies LBA 0 & 1
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
