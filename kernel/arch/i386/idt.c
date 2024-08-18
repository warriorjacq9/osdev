#include <kernel/idt.h>
#include <stdio.h>
#include <string.h>
#include <kernel/page.h>
#include <drivers/keyboard.h>
#include <kernel/pic.h>
#include <drivers/timer.h>

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];
__attribute__((aligned(0x10)))
static idtr_t idtr;

isr_t handlers[15];

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
            printf("%d: %s exception at 0x%08X. System halted", r.int_no, exceptions[r.int_no], r.eip);
        }
        for(;;);
    } else if (r.int_no < 48)
    {
        pic_send_eoi(r.int_no - 32);
        if (handlers[r.int_no - 32] != 0){
            isr_t handler = handlers[r.int_no - 32];
            handler(r);
        }
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
extern void *irq_stub_table[];

void register_handler(uint8_t vector, isr_t handler)
{
    handlers[vector - 32] = handler;
    idt_set_descriptor(vector, irq_stub_table[vector - 32], 0x8E);
}

void idt_init()
{
    idtr.base = (uint32_t) &idt[0];
    idtr.limit = sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    memset(&idt, 0, sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS);

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    register_handler(33, keyboard_handler);
    register_handler(32, timer_handler);

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}