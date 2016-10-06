.code32
# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set VIDINFO,  1 << 2
.set FLAGS,    ALIGN | MEMINFO | VIDINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0xe85250d6       # 'magic number' lets bootloader find the header
.set ARCH,     0x0000000
.set LEN,      18
.set CHECKSUM, -(MAGIC + ARCH + LEN) # checksum of above, to prove we are multiboot
#oh osdev.org, you have failed me again...
multiboot_start:	
.section .multiboot
.align 4
.long MAGIC
.long ARCH
.long LEN
.long CHECKSUM
.short 0 #type
.short FLAGS
.long  8
bootstrap:	 hlt
	/*
multiboot_stop:	
# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
# The kernel entry point
.section .text
#.code64	
.global _start
.type _start, @function
_start:
	# Set the stack pos
	#mov $stack_top, %rsp
    	#mov $stack_top, %rbp
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
*/
