# ========================================================================
# Beginning of Nyx-Atlas Second-Stage G/I Descriptor Module v1.0
# ========================================================================

# ========================================================================
# Boot Memory Descriptor Definition Routines
# Each global descriptor is 8 bytes

.func define_GDT				# Function: define_GDT
define_GDT:						# Define Global-Descriptor-Table
	mov ax,0						# Load AX to 0
	mov es,ax						# Store 0 in ES as memory location
	mov di,0x800					# Set memory offset to 800h (0-79F is IDT)
	
	# NULL DESCRIPTOR
	mov cx,4						# Set rep counter to 4
	rep stosw						# Load empty words into the value of DI
	
	# CODE DESCRIPTOR
	mov word ptr es:[di],0xffff		# Limit of code descriptor is ffff (full 4 gigs) (lower 2 bytes of limit)
	mov word ptr es:[di+2],0x0		# Base of code descriptor is 0 (beginning of memory) (lower 2 bytes of base)
	mov byte ptr es:[di+4],0x0		# Other segment of base (middle byte of base)
	mov byte ptr es:[di+5],0x9a		# Access byte = 0x9a (explained online)
	mov byte ptr es:[di+6],0xcf		# Flag partal byte + rest of limit (explained online) (top 4 bits of limits)
	# Granularity bit set in flags -> multiplies all 20 bits of limit by 4096 (resulting in FFFFF = 4 gigs)
	mov byte ptr es:[di+7],0x0		# Final segment of base (upper byte of base)
	add di,8						# Add total amount of filled bytes to DI for next entry
	
	# DATA DESCRIPTOR
	mov word ptr es:[di],0xffff		# Limit of code descriptor is ffff (full 4 gigs)
	mov word ptr es:[di+2],0x0		# Base of code descriptor is 0 (beginning of memory)
	mov byte ptr es:[di+4],0x0		# Other segment of base
	mov byte ptr es:[di+5],0x92		# Access byte for data = 0x92 (explained online)
	mov byte ptr es:[di+6],0xcf		# Flag partal byte + rest of limit (explained online)
	mov byte ptr es:[di+7],0x0		# Final segment of base
	
	ret								# Return to call location
.endfunc

.func define_IDT				# Function: define_IDT
define_IDT:						# Define Interrupt-Descriptor-Table
	mov ax,0						# Load AX to 0
	mov es,ax						# Store 0 in ES as memory location
	mov di,0x0						# Set memory offset to 0h (800h is GDT)
	
	# NULL INTERRUPT TABLE
	mov cx,0x800					# Set rep counter to 2048
	rep stosb						# Load zeros into table
	
	ret								# Return to call location
.endfunc

# ========================================================================
# End of Atlas Second-Stage G/I Descriptor Module v1.0
# ========================================================================