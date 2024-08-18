#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/pic.h>
#include <drivers/timer.h>
#include <drivers/pcspk.h>

void kernel_main(void) {
	terminal_initialize();

	pic_remap(0x20, 0x28);
	pic_disable();
	irq_clear_mask(0); // Timer
	irq_clear_mask(1); // Keyboard
	timer_set(1);
	gdt_init();
	idt_init();
	
	printf("Hello, kernel World!\n");
	while(1) asm ("hlt");
}
