# ========================================================================
# Beginning of Atlas Second-Stage 32-Bit PM Application Module v1.0
# ========================================================================

# ========================================================================
# Protectd Mode Application Routines

.func enable_pm					# Function: enable_pm
enable_pm:						# Enables 32-bit protected mode
	mov eax,cr0						# Store cr0 register in EAX
	or al,1							# Set final bit of register (determines PM state)
	mov cr0,eax						# Store result back into cr0
	ret								# Return to call location
.endfunc

.func cl_prefetch				# Function: cl_prefetch
cl_prefetch:					# Clears CPU prefetch queue
	jmp _cl_prefetch_cldat			# Jump past enough instructions (triggers CPU clearing)
	nop								# Pad mem with NO-OP to verify there is enough space to clear trigger clearing
	nop
_cl_prefetch_cldat:				# Jump location to clear prefetch queue
	ret								# Return to call location
.endfunc

# ========================================================================
# End of Atlas Second-Stage 32-Bit PM Application Module v1.0
# ========================================================================