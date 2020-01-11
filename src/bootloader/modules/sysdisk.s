# ========================================================================
# Beginning of Atlas Second-Stage System Disk Routine Module v1.0
# ========================================================================

# ========================================================================
# Primary Disk Routine

.func read_kernel				# Function: read_kernel
read_kernel:					# Reads the kernel into memory
	pusha							# Preserve all registers on the stack
	push es							# Preserve ES on the stack
	
	mov bootdev,dl					# Store boot device
	call read_voldesc				# Calculate volume descriptor key values
	
	lea bx,root_index				# Load memory location of root directory inode index into BX
	call load_i32					# Load root directory inode index into EAX
	mov edi,eax						# Store in EDI
	lea bx,disk_buffer				# Set memory location to be disk buffer
	call read_inode					# Read root directory into disk buffer
	
	xor esi,esi						# Clear the value of SI
	lea si,filename_kern			# Load location of boot.sys filename into SI
	call find_file					# Locate the file and store inode index in EDI
	
	mov ax,0x200					# Set offset for kernel (linear address 0x2000)
	mov es,ax						# Load offset for kernel into ES (ES:BX)
	mov bx,0						# Load target memory offset (0 -> start of segment) into BX
	call read_inode					# Read SSBL into memory at disk_buffer
	
	pop es							# Restore ES from the stack
	popa							# Restore all registers from the stack
	ret								# Return to call location
.endfunc

# ========================================================================
# BCFS Filesystem Routines
# Formula to get sector of block: sector_lba = block_size / 512 * block_ptr + 2

.func read_voldesc				# Function: read_voldesc
read_voldesc:					# Reads important information from the volume descriptor
	pusha							# Preserve all registers on the stack
	
	lea ax,disk_buffer				# Load address of disk buffer into AX
	mov sector_adr,ax				# Store memory address into destination offset
	call read_disk					# Read the disk using the read packet
	
	lea si,disk_buffer				# Load offset of volume descriptor into CX
	mov ax,disk_buffer				# Load first 2 bytes of volume descriptor (BCFS signature)
	cmp ax,0xDC75					# Is it a valid BCFS signature?
	jne _read_voldesc_bad_fmt		# If not, handle bad disk format
	
	mov ax,[si+36]					# Load filesystem state into AX
	cmp ax,0x1						# Does the filesystem have errors?
	jne _read_voldesc_skip			# If not, skip filesystem repair
	call repair_fserr				# Repair filesystem error
_read_voldesc_skip:				# Jump point to skip filesystem repair
	mov ax,[si+10]					# Load block size from volume descriptor into AX
	mov block_size,ax				# Store block size from AX into variable
	mov bx,512						# Each sector is 512 bytes
	xor dx,dx						# Make sure that DX doesn't interfere with division
	div bx							# Divide block size by sector size to get sectors per block
	mov sector_qty,ax				# Load sectors ber block into sector read count
	
	mov ax,[si+120]					# Load inode size from volume descriptor into AX
	mov inode_size,ax				# Store inode size from AX into variable
	
	lea bx,disk_buffer				# Load address of volume descriptor into BX
	add bx,122						# Add offset for root directory inode index
	call load_i32					# Load index into memory
	lea bx,root_index				# Load address of root index variable into BX
	call store_i32					# Store root index into variable
	
	popa							# Restore all registers from the stack
	ret								# Return to call location
_read_voldesc_bad_fmt:			# Jump point for bad disk format
	lea si,fmt_err					# Load message into string pointer
	call putsln						# Print message
	jmp reboot						# Reboot the machine
.endfunc

.func read_inode				# Function: read_inode
read_inode:						# Reads the contents of an inode (index EDI) into memory at ES:BX
	push bx							# Preserve the memory offset
	push es							# Preserve value of segment
	mov ax,0						# Store 0 in AX
	mov es,ax						# Make sure that extra segment is 0
	
	mov sector_seg,es				# Load segment into packet
	lea bx,disk_buffer				# Load address of disk buffer into BX
	mov sector_adr,bx				# Store address of disk buffer into packet as read address
	
	mov ax,block_size				# Load block size into AX
	mov bx,512						# Load sector size into BX
	xor dx,dx						# Make sure DX doesn't interfere with division
	div bx							# blocks_per_sector = block_size / sector_size
	mov bx,1						# Load block_ptr of inode table into BX
	mul bx							# sector_lba = blocks_per_sector * block_ptr
	add ax,2						# Load boot sector (2 lba sectors) into AX
	call zero_lba					# Reset value of LBA
	mov sector_lba,ax				# Add boot sector offset to sector_lba
	call read_disk					# Read the disk into memory
	
	mov ecx,edi						# Load inode index into counter register
	inc ecx							# Increase ECX (inodes left to read = index + 1)
	lea bx,disk_buffer				# Inode at the beginning of the disk buffer
_read_inode_search:				# Jump point to loop and search for the inode
	dec ecx							# Decrease number of inodes left to read
	jz _read_inode_cont				# None left to read? If so, start reading block chain
	
	mov ax,[bx]						# Load link type into AX
	cmp ax,2						# Is it an inode table link?
	jne _read_inode_nolink			# If not, skip the linking process
	add bx,28						# Set BX to memory offset of block pointer
	call load_i32					# Load block pointer into EAX
	mov esi,eax						# Store block pointer into ESI
	
	xor eax,eax						# Make sure that the upper end of EAX is 0
	mov ax,block_size				# Load block size into AX
	mov bx,512						# Load sector size into BX
	xor dx,dx						# Make sure DX doesn't interfere with division
	div bx							# blocks_per_sector = block_size / sector_size
	
	mov ebx,esi						# Load block_ptr of next inode table into EBX
	mul ebx							# sector_lba = blocks_per_sector * block_ptr
	add eax,2						# Load boot sector (2 lba sectors) into AX
	call zero_lba					# Reset value of LBA
	lea bx,sector_lba				# Load address of starting sector in packet
	call store_i32					# Write LBA sector address to memory
	call read_disk					# Read the disk into memory
	
	mov bx,disk_buffer				# Inode at the beginning of the disk buffer
	jmp _read_inode_loop			# Skip the sector inc process (not necessary for link)
_read_inode_nolink:				# Jump point to skip the linking process
	add bx,inode_size				# Increase offset to the next inode
_read_inode_loop:				# Jump point after the linking process
	jmp _read_inode_search			# Scan next inode
_read_inode_cont:				# Jump point to stop reading block chain
	add bx,28						# Set BX to memory offset of inode block pointer
	call load_i32					# Load block pointer into EAX
	
	pop es							# Restore segment from stack
	mov sector_seg,es				# Store segment in variable
	pop bx							# Restore memory offset into BX
	call load_blkchain				# Load block chain into memory
	ret								# Return to call location
.endfunc

.func load_blkchain				# Function: load_blkchain
load_blkchain:					# Loads block chain at block-ptr EAX into memory at offset BX
	push bx							# Preserve offset
	mov ecx,eax						# Store block ptr in ECX
	
	xor eax,eax						# Make sure that the upper end of EAX is 0
	mov ax,block_size				# Load block size into AX
	mov bx,512						# Load sector size into BX
	xor dx,dx						# Make sure DX doesn't interfere with division
	div bx							# blocks_per_sector = block_size / sector_size
	
	mov ebx,ecx						# Load block_ptr of next inode table into EBX
	mul ebx							# sector_lba = blocks_per_sector * block_ptr
	add eax,2						# Load boot sector (2 lba sectors) into AX
	call zero_lba					# Reset value of LBA
	lea bx,sector_lba				# Load address of starting sector in packet
	call store_i32					# Write LBA sector address to memory
	
	pop bx							# Restore memory offset
	mov sector_adr,bx				# Set the address in packet to BX
	call read_disk					# Read the disk into memory
	mov ax,block_size				# Load size of each block into AX
	add bx,ax						# Move BX to address after loaded block
	sub bx,4						# Subtract 4 from BX to point to next block pointer
	call load_seg_i32				# Load next block pointer into EAX
	
	cmp eax,0						# Is this the last block?
	jne load_blkchain				# If not, load the next block
	ret								# Otherwise, return to call location
.endfunc

.func find_file					# Function: find_file
find_file:						# Finds a file with name at location SI and returns index EDI
	lea bx,disk_buffer				# Load location of folder data into BX
_find_file_loop:				# Compare each file in the directory
	xor ecx,ecx						# Ensure ECX is zero
	mov cx,[bx+4]					# Load name length of file into CX
	cmp cx,0						# Zero length string? (No files left)
	je _find_file_notfound			# If so, boot.sys does not exist or is misplaced
	xor edi,edi						# Clear EDI (to prevent interference by upper 16 bits)
	mov di,bx						# Load file entry location into DI
	add di,6						# Offset DI to point to filename
	
	call string_eq					# Is the name equal to the input string?
	jc _find_file_found				# If so, the file has been found
	
	mov cx,[bx+4]					# Otherwise, load filename length into CX
	add bx,6						# Offset BX to point to filename
	add bx,cx						# Offset BX by filename length to point to next entry
	jmp _find_file_loop				# Try the next entry
_find_file_found:				# Called when the file has been found
	call load_i32					# Load the inode index of the file into EAX
	mov edi,eax						# Store inode index into EDI
	ret								# Return to call location
_find_file_notfound:			# Called when the file does not exist or is misplaced
	lea si,search_err				# Load message into string pointer
	call putsln						# Print message to screen
	jmp reboot						# Reboot the machine
.endfunc

# ========================================================================
# Bootloader Disk Routines

.func read_disk					# Function: read_disk
read_disk:						# Performs a BIOS disk read using packet specified at read_pckt
	pusha							# Preserve all registers on the stack
	lea si,read_pckt				# Load read packet into pointer for BIOS
	mov dl,bootdev					# Load device 0 into DL (or whichever device is at 0x80)
	mov ah,0x42						# Set BIOS subinterrupt routine for extensive disk reading
	stc								# Some BIOSes do not set carry flag on error
	int 0x13						# Execute BIOS disk service interrupt routine
	jnc _read_disk_cont				# Is carry flag set (set on error)? If not, continue
	call reset_disk					# Attempt to reset the hard disk
	mov ah,0x42						# Reload subinterrupt routine (0x13 can change it)
	int 0x13						# Try again after disk reset
	jc fatal_disk_err				# Fail again? If so, abort bootloader
_read_disk_cont:				# Jump point to skip disk reset
	popa							# Restore all registers from the stack
	ret								# Return to call location
.endfunc

.func reset_disk				# Function: reset_disk
reset_disk:						# Reset the hard disk to repair errors
	pusha							# Preserve all registers on the stack
	mov ah,0x0						# Load BIOS subinterrupt routine for resetting disk system
	mov dl,bootdev					# Load boot device number into DL
	stc								# Some BIOSes do not set carry flag on error
	int 0x13						# Execute BIOS disk service interrupt routine
	jnc _reset_disk_cont			# Is carry flag set (set on error)? If not, continue
	mov ah,0x0						# Reload subinterrupt routine (0x13 can change it)
	int 0x13						# Try again
	jc fatal_disk_err				# If there is an error then abort bootloader
_reset_disk_cont:				# Jump point to skip 2nd reset process
	popa							# Restore all registers from the stack
	ret								# Return to call location
.endfunc

.func repair_fserr				# Function: repair_fserr
repair_fserr:					# Repairs filesystem errors
	push si							# Preserve the value of SI
	lea si,disk_buffer				# Load offset of volume descriptor in CX
	mov ax,[si+38]					# Load error compensation method into AX
	cmp ax,0x2						# Should cause boot failure?
	je fatal_disk_err				# If so, abort bootloader
	pop si							# Restore the value of SI
	ret								# Otherwise continue normally (could mount read-only in kernel)
.endfunc

fatal_disk_err:					# Prints a fatal disk error message and reboots
	lea si,disk_err					# Load message into string pointer
	call putsln						# Print message to screen
	jmp reboot						# Reboot the machine

reboot:							# Reboots the (virtual) machine
	mov ax,0						# Load subinterrupt routine to read char
	int 0x16						# Execute keyboard service interrupt routine
	mov ax,0						# Load reboot subinterrupt routine
	int 0x19						# Execute power control service interrupt routine

# ========================================================================
# Logical Boot Routines

.func string_eq					# Function: string_eq
string_eq:						# Determines whether two strings (length CX) are equal (SI & DI) and equality is stored in CF
	pusha							# Preserve all registers on the stack
_string_eq_loop:				# Loop through to test each character for equality
	mov al,[si]						# Load next character of first string into AX
	mov bl,[di]						# Load next character of second string into BX
	inc si							# Increase SI to next character
	inc di							# Increase DI to next character
	cmp al,bl						# Are the characters equal?
	jne _string_eq_false			# If not, resolve as false
	loop _string_eq_loop			# Loop to next character if it exists
	popa							# Restore all registers from the stack
	stc								# At this point, all characters have been checked and string is equal
	ret								# Return to call location with carry flag set
_string_eq_false:				# Called when two characters are not equal
	popa							# Restore all registers from the stack
	clc								# Clear carry flag
	ret								# Return to call location
.endfunc

.func zero_lba					# Function: zero_lba
zero_lba:						# Resets the value of the disk read LBA
	push bx							# Preserve BX
	lea bx,sector_lba				# Load offset of LBA variable into BX
	mov word ptr [bx],0				# Set first octet of LBA to 0
	mov word ptr [bx+2],0			# Set second octet of LBA to 0
	mov word ptr [bx+4],0			# Set third octet of LBA to 0
	mov word ptr [bx+6],0			# Set fourth octet of LBA to 0
	pop bx							# Restore BX
	ret								# Return to call location
.endfunc

.func store_i32					# Function: store_i32 (uses little-endian addressing)
store_i32:						# Stores a 32-bit integer in EAX to memory location 7C00:BX
	push ax							# Preserve the value of AX (Cannot push 2-bit registers)
	mov [bx],ax						# Load low end of EAX into memory location
	shr eax,16						# Shift EAX to the right (get high end into AX)
	mov [bx+2],ax					# Load high end into memory location + 2
	shl eax,16						# Restore the high end of EAX
	pop ax							# Restore the low end of EAX
	ret								# Return to call location
.endfunc

.func load_i32					# Function: load_i32 (uses little-endian addressing)
load_i32:						# Loads a 32-bit integer into EAX from memory location 7C00:BX
	xor eax,eax						# Set AX equal to 0
	mov ax,[bx+2]					# Load the high end of integer into AX
	shl eax,16						# Shift AX into high end of EAX
	mov ax,[bx]						# Load low end of integer ino AX
	ret								# Return to call location
.endfunc

.func store_seg_i32				# Function: store_i32 (uses little-endian addressing)
store_seg_i32:					# Stores a 32-bit integer in EAX to memory location ES:BX
	push ax							# Preserve the value of AX (Cannot push 2-bit registers)
	mov es:[bx],ax					# Load low end of EAX into memory location
	shr eax,16						# Shift EAX to the right (get high end into AX)
	mov es:[bx+2],ax				# Load high end into memory location + 2
	shl eax,16						# Restore the high end of EAX
	pop ax							# Restore the low end of EAX
	ret								# Return to call location
.endfunc

.func load_seg_i32				# Function: load_i32 (uses little-endian addressing)
load_seg_i32:					# Loads a 32-bit integer into EAX from memory location ES:BX
	xor eax,eax						# Set AX equal to 0
	mov ax,es:[bx+2]				# Load the high end of integer into AX
	shl eax,16						# Shift AX into high end of EAX
	mov ax,es:[bx]					# Load low end of integer ino AX
	ret								# Return to call location
.endfunc

# ========================================================================
# Boot device data

bootdev:	.byte 0x0				# Boot device (carried from initial bootloader)

block_size:	.word 0x0				# Size of each block in the filesystem
inode_size:	.word 0x0				# Size of each inode in the inode table
root_index:	.word 0x0				# Index of the root directory inode
			.word 0x0

# ========================================================================
# Device read packet

read_pckt:	.byte 0x10				# Data packet for BIOS to read from disk (size of pckt is 16)
			.byte 0x0				# Unused (always 0)
sector_qty:	.word 0x2				# Read 2 sectors (1024 bytes (2 sectors) of volume-desc)
sector_adr:	.word 0x0				# Destination memory offset
sector_seg:	.word 0x0				# Destination memory segment (first segment 0x0)
sector_lba:	.word 0x2				# First sector to read from (specified in LBA)
			.word 0x0				# Volume descriptor exists after 1024 byte boot sector
			.word 0x0				# Boot sector occupies LBA 0 & 1
			.word 0x0

# ========================================================================
# Boot Disk Messages

boot_repair:	.asciz "atlas: Repairing invalid boot device"
disk_err:		.asciz "atlas: Fatal disk read error"
search_err:		.asciz "atlas: Cannot find KERNEL.SYS"
fmt_err:		.asciz "atlas: Disk improperly formatted"

filename_kern:	.asciz "kernel.sys"

# ========================================================================
# Memory Partitioning

disk_buffer:						# Memory buffer as a disk reading destination

# ========================================================================
# End of Atlas Second-Stage System Disk Routine Module v1.0
# ========================================================================