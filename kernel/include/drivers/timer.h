#ifndef TIMER_H
#define TIMER_H
#include <kernel/idt.h>
#include <string.h>

void timer_handler(registers_t r);
void timer_wait(uint32_t ms);
void timer_set(uint32_t freq);

#endif