#include <stdint.h>
#include <stdio.h>

#include <kernel/page.h>
#include "vga.h"
#include <kernel/tty.h>
#include <kernel/idt.h>
/*
uint32_t *page_dir;
uint32_t *page_table;

extern uint32_t get_cr3();
extern void set_cr3(uint32_t*);
extern uint32_t get_cr0();
extern void set_cr0(uint32_t);

void page_init()
{
	uint32_t addr = 0;
	uint32_t i;
	for(i = 0; i < 1024; i++) {
		page_table[i] = addr | 3; // Attribute is supervisor, read/write, present (Binary 011)
		addr += 4096;
	}

	page_dir[0] = (uint32_t)page_table;
	page_dir[0] |= 3; // Supervisor, R/W, present

	for(i = 1; i < 1024; i++) {
		page_dir[i] = 0 | 2; // Supervisor, R/W, not present
	}
	set_cr3(page_dir);
	set_cr0(get_cr0() | 0x80000000);
	
}
*/
void page_fault(registers_t regs){
	__asm__ volatile("cli");
	
	terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));

	unsigned int err_pos;
	__asm__ volatile ("mov %%cr2, %0" : "=r" (err_pos));

	printf("Page fault occurred at 0x%08X, code 0x%08X\nReasons:\n", err_pos, regs.eip);

	int no_page = regs.err_code & 1;
	int rw = regs.err_code & 2;
	int um = regs.err_code & 4;
	int re = regs.err_code & 8;
	int dc = regs.err_code & 16;

	if(dc)			printf("	(Instruction decode error)\n");
	if(!no_page)	printf(" 	(No page present)\n");
	if(um)			printf(" 	(in user mode)\n");
	if(rw)			printf(" 	(Inadequate write permissions)\n");
	if(re)			printf(" 	(Reserved)\n");
	
	printf("\n");
}