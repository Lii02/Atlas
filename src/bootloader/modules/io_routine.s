# ========================================================================
# Beginning of Atlas Second-Stage IO Routine Module v1.0
# ========================================================================

# ========================================================================
# Boot IO Routines

.func putc						# Function: putc
putc:							# Prints a character to the screen in AL
	pusha							# Preserve all registers on the stack
	mov ah,0x0e						# Set AH to proper BIOS teletype subroutine
	int 0x10						# Run BIOS video interrupt
	popa							# Restore all registers from the stack
	ret								# Return to call location
.endfunc

.func putsln					# Function: putsln
putsln:							# Prints a zero-terminated string to the screen pointed to by SI after advancing to the next line
	call putln						# Call new line function
	call puts						# Call print function
	ret								# Return to call location
.endfunc

.func puts						# Function: puts
puts:							# Prints a zero-terminated string pointed to by by SI to the screen
	pusha							# Preserve all registers on the stack
	mov ah,0x0e						# Set BIOS to teletype subroutine
_puts_rep:						# Loop through and print each character
	lodsb							# Load next character from SI into AL
	cmp al,0						# End of string?
	je _puts_end					# If so, return
	int 0x10						# Run BIOS video interrupt
	jmp _puts_rep					# Print next character
_puts_end:						# Called when the string is terminated
	popa							# Restore all registers from the stack
	ret								# Return to call location
.endfunc

.func putln						# Function: putln
putln:							# Advances cursor to the next line
	pusha							# Preserve all registers on the stack
	mov ax,0x0E0A					# Load line feed into AL, and BIOS teletype subroutine into AH
	int 0x10						# Run BIOS video interrupt
	mov al,0x0D						# Set AL to carriage return
	int 0x10						# Run BIOS video interrupt
	popa							# Restore all registers from the stack
	ret								# Return to call location
.endfunc

.func clearsc					# Function: clearsc
clearsc:						# Clears screen
	pusha							# Push registers to the stack
	mov dx,0						# Set cursor position to top left
	call set_cursor					# Apply cursor position
	
	mov ah,6						# Set proper subfunction for scrolling screen
	mov al,0						# Set scroll mode to clear
	mov bh,7						# Set color attributes for screen
	mov cx,0						# Top left part of screen
	mov dh,24						# Bottom right part of screen
	mov dl,79
	int 0x10						# Run scroll interrupt
	
	popa							# Pop registers from the stack
	ret								# Return to call location
.endfunc

.func set_cursor				# Function: set_cursor
set_cursor:						# Sets cursor position on screen (x=DL, y=DH)
	pusha							# Push registers to the stack
	mov bh,0						# Set page number to 0
	mov ah,2						# Set proper subfunction for location setting
	int 0x10						# Run interrupt
	
	popa							# Pop registers from the stack
	ret								# Return to call location
.endfunc

.func get_cursor				# Function: get_cursor
get_cursor:						# Return cursor's position (x=DL, y=DH)
	pusha							# Push registers to the stack
	mov bh,0						# Set page number to 0
	mov ah,3						# Set proper subfunction for location getting
	int 0x10						# Run interrupt
	
	mov _get_cursor_pos,dx			# Temp store position in variable
	popa							# Pop registers from the stack
	mov dx,_get_cursor_pos			# Store position back into DX
	ret								# Return to call location
.endfunc

_get_cursor_pos: .word 0x0			# Variable to temporarily store cursor location

# ========================================================================
# End of Atlas Second-Stage IO Routine Module v1.0
# ========================================================================