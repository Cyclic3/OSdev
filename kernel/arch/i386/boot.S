# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set VIDINFO,  1 << 2
.set FLAGS,    ALIGN | MEMINFO | VIDINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0
.long 0
.long 0
.long 0
.long 0
.long 1 # 0 = graphics mode; 1 = text mode
.long 0, 0, 0 # Width, height, depth
# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	# Set the stack pos
	movl $stack_top, %esp
    movl $stack_top, %ebp
	# Initialize the core kernel before running the global constructors.
	call kernel_early

	# Call the global constructors.
	call _init
    
	# Transfer control to the main kernel.
	call kernel_main

    #call if kernel_main decides to stop
    call kernel_ret

    cli
.Lhang:
	hlt
	jmp .Lhang
.size _start, . - _start
