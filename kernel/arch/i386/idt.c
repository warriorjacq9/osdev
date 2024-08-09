#include <kernel/idt.h>
#include <stdio.h>
#include <string.h>
#include <kernel/page.h>

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];
__attribute__((aligned(0x10)))
static idtr_t idtr;

char *exceptions[] = 
{
    "Division by Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

void exception_handler(registers_t r)
{
    if (r.int_no < 32)
    {
        if(r.int_no == 14){
            page_fault(r);
        } else {
            printf("%d: %s exception. System halted", r.int_no, exceptions[r.int_no]);
        }
        for(;;);
    }
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags)
{
    idt[vector].isr_low        = (uint16_t)((uint32_t)isr & 0x0000FFFF);
    idt[vector].kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    idt[vector].attributes     = flags;
    idt[vector].isr_high       = (uint16_t)((uint32_t)isr >> 16);
    idt[vector].reserved       = 0;
}

static bool vectors[IDT_MAX_DESCRIPTORS];
extern void *isr_stub_table[];

void idt_init()
{
    idtr.base = (uint32_t) &idt[0];
    idtr.limit = sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    memset(&idt, 0, sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS);

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}