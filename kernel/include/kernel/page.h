#ifndef PAGE_H
#define PAGE_H
#include <stdint.h>
#include <kernel/idt.h>

//void page_init();
void page_fault(registers_t regs);

#endif