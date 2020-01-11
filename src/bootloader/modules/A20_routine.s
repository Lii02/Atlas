# ========================================================================
# Beginning of Atlas Second-Stage A20 Connectivity Module v1.0
# ========================================================================

# ========================================================================
# A20 Verification Subroutine

# If A20 is enabled, then locations at 0000:0500 and ffff:0510 are different.
# Otherwise, they refer to the same location in memory.
# This macro tests if they refer to the same place in memory and sets CF accordingly.

.func verify_A20				# Function: verify_A20
verify_A20:						# Test A20 Line connectivity (sets CF if enabled)
	pusha							# Push registers to the stack
	pushf							# Push flag register to the stack
	
	cli								# Disable interrupts
	mov ax,0						# Load 0 into AX
	mov es,ax						# Set segment offset to 0
	mov di,0x500					# Set specific offset to 500
	
	mov al,es:[di]					# Load character at 0000:0500 into AL for storage
	mov byte ptr es:[di],0x00		# Temporarily store 0 in that location
	
	mov ax,0xffff					# Load ffff into AX
	mov es,ax						# Set segment offset to ffff
	mov di,0x510					# Set specific offset to 510
	
	mov ah,es:[di]					# Load character at ffff:0510 into AH for storage
	mov byte ptr es:[di],0xff		# Temporarily store 255 in that location
	
	mov ax,0						# Load 0 into AX
	mov es,ax						# Set segment offset to 0
	mov di,0x500					# Set specific offset to 500
	mov bl,es:[di]					# Load current byte at 0000:0500 into BL
	mov es:[di],al					# Return original value into that address
	
	mov ax,0xffff					# Load ffff into AX
	mov es,ax						# Set segment offset to ffff
	mov di,0x510					# Set specific offset to 510
	mov bh,es:[di]					# Load current byte at ffff:0510 into BH
	mov es:[di],ah					# Return original value into that address
	
	cmp bl,bh						# Are those bytes equal?
	jne _verify_A20_neq				# If not, then they are different mem locations
	
	popf							# Otherwise, they are same. Pop flag register
	popa							# Pop all registers from the stack
	clc								# Clear carry, A20 is disabled
	ret								# Return to call location
_verify_A20_neq:				# Jump point when bytes are different
	popf							# Pop flag register
	popa							# Pop all registers from the stack
	stc								# Set carry, A20 is enabled
	ret								# Return to call location
.endfunc

# ========================================================================
# A20 Activation Routines

.func activate_A20_bios			# Function: activate_A20_bios
activate_A20_bios:		     	# Attempt to enable A20 through the BIOS
	pusha							# Push registers to the stack
	mov ax,0x2401					# Set subfunction for interrupt
	int 0x15						# Enable A20 line
	popa							# Pop registers from the stack
	ret								# Return to call location
.endfunc

.func activate_A20_keyctrl		# Function: activate_A20_keyctrl
activate_A20_keyctrl:			# Attempt to enable A20 through keyboard controller
	pushad							# Push registers to the stack
	cli								# Disable interrupts
	
	call _activate_A20_keyctrl_cmd	# When controller is ready for command
	mov al,0xad						# Send command 0xad (disable keyboard)
	out 0x64,al						# Output AL
	
	call _activate_A20_keyctrl_cmd	# When controller is ready for command
	mov al,0xd0						# Send command 0xd0 (read from input)
	out 0x64,al						# Output AL
	
	call _activate_A20_keyctrl_dat	# When controller has data ready
	in al,0x60						# Read input from keyboard
	push eax						# Save it in the stack
	
	call _activate_A20_keyctrl_cmd	# When controller is ready for command
	mov al,0xd1						# Set command 0xd1 (write to output)
	out 0x64,al						# Output AL
	
	call _activate_A20_keyctrl_cmd	# When controller is ready for command
	pop eax							# Write input back
	or al,2							# Set bit 2 in AL
	out 0x60,al						# Output AL
	
	call _activate_A20_keyctrl_cmd	# When controller is ready for command
	mov al,0xae						# Write command 0xae (enable keyboard)
	out 0x64,al						# Output AL
	
	call _activate_A20_keyctrl_cmd	# Wait until controller is ready for command
	
	sti								# Enable interrupts
	popad							# Pop registers from the stack
	ret								# Return to call location
_activate_A20_keyctrl_cmd:		# Wait for keyboard command
	in al,0x64						# Wait at 0x64
	test al,2
	jnz _activate_A20_keyctrl_cmd	# Result is zero? If not, continue waiting
	ret								# Return to call location
_activate_A20_keyctrl_dat:		# Wait for keyboard data
	in al,0x64						# Wait at 0x64
	test al,1
	jz _activate_A20_keyctrl_dat	# Result is zero? If so, continue waiting
	ret								# Return to call location
.endfunc

.func activate_A20_FA20			# Function: activate_A20_FA20
activate_A20_FA20:				# Attempt to enable A20 through "Fast-A20"
	pusha							# Push registers to the stack
	in al,0x92						# Input command 0x92 into AL
	or al,2							# Enable bit 2 in AL
	out 0x92,al						# Output command back
	popa							# Pop registers from the stack
	ret								# Return to call location
.endfunc

# ========================================================================
# End of Atlas Second-Stage A20 Connectivity Module v1.0
# ========================================================================