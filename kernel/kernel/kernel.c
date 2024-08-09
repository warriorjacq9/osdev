#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/pic.h>

void kernel_main(void) {
		
	terminal_initialize();

	pic_remap(0x08, 0x70);
	gdt_init();
	idt_init();
	
	printf("Hello, kernel World!");
}
