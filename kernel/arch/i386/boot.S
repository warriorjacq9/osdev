# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .bss, "aw", @nobits
	.align 4096
.global boot_page_directory
boot_page_directory:
	.skip 4096
.global boot_page_table1
boot_page_table1:
	.skip 4096

# The kernel entry point.
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
movl $(boot_page_table1 - 0xC0000000), %edi
	movl $0, %esi
	movl $1023, %ecx
1:
	cmpl $_kernel_start, %esi
	jl 2f
	cmpl $(_kernel_end - 0xC0000000), %esi
	jge 3f

	movl %esi, %edx
	orl	$0x003, %edx
	movl %edx, (%edi)
2:
	addl $4096, %esi
	addl $4, %edi
	loop 1b
3:
	movl $(0x000B8000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4

	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

	movl $(boot_page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3

	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	lea 4f, %ecx
	jmp *%ecx

.section .text

4:
	movl $0, boot_page_directory + 0

	movl %cr3, %ecx
	movl %ecx, %cr3

	mov $stack_top, %esp

	# Call the global constructors.
	#call _init

	# Transfer control to the main kernel.
	cli
	call kernel_main


	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b

.global gdt_flush     
.extern gp          
gdt_flush:
    lgdt [gp]        
    mov $0x10, %ax      
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
	jmp $0x08, $flush2
flush2:
    ret

.global get_cr0
get_cr0:
	mov %cr0, %eax
	ret

.global set_cr0
set_cr0:
	push %ebp
	mov %esp, %ebp
	mov 8(%ebp), %eax
	mov %eax, %cr0
	pop %ebp
	ret

.global get_cr3
get_cr3:
	mov %cr3, %eax
	ret

.global set_cr3
set_cr3:
	push %ebp
	mov %esp, %ebp
	mov 8(%ebp), %eax
	mov %eax, %cr3
	pop %ebp
	ret