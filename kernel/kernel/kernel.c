#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>

void kernel_main(void) {
	terminal_initialize();

	idt_init();

	create_descriptor(0, 0, 0);
	create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
	create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
	create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
	create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));

	printf("Hello, kernel World!");
}
