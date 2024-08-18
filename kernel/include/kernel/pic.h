#ifndef PIC_H
#define PIC_H

#include <stdint.h>
#include <stdio.h>

#define PIC1		    0x20		/* IO base address for master PIC */
#define PIC2		    0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA       (PIC2+1)
#define PIC_EOI         0x20
#define ICW1_ICW4       0x01
#define ICW1_SINGLE     0x02
#define ICW1_INTERVAL4  0x04
#define ICW1_LEVEL      0x08
#define ICW1_INIT       0x10

#define ICW4_8086       0x01
#define ICW4_AUTO       0x02
#define ICW4_BUF_SLAVE  0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM       0x10

#define PIC_IRR         0x0A
#define PIC_ISR         0x0B

void pic_send_eoi(uint8_t irq);

void pic_remap(uint32_t offset1, uint32_t offset2);
void pic_disable();

void irq_set_mask(uint8_t irq);
void irq_clear_mask(uint8_t irq);

uint16_t pic_get_irr();
uint16_t pic_get_isr();

uint8_t check_interrupts();

#endif