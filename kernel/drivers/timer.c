#include <drivers/timer.h>
#include <kernel/idt.h>
#include <kernel/tty.h>
#include <stdio.h>

char *itoa(uint32_t value, char *buffer) {
    char temp[10];
    char *p = temp;
    do {
        *p++ = (char)(value % 10) + '0';
        value /= 10;
    } while (value > 0);

    do {
        *buffer++ = *--p;
    } while (p != temp);

    return buffer;
}

uint32_t tick;
void timer_handler(registers_t r)
{
    char buf[256];
    if (tick < 4294967296){tick++;}else tick = 0;
    itoa(tick, buf);
    terminal_writestringat(buf, 0, VGA_HEIGHT - 1);
}

void timer_wait(uint32_t ms)
{
    uint32_t a = tick;
    while(tick - a != ms) asm("nop");
}

void timer_set(uint32_t freq)
{
    uint32_t div = 1193180 / freq;
    uint8_t low = (uint8_t)(div & 0xFF);
    uint8_t high = (uint8_t)((div >> 8) & 0xFF);

    outb(0x43, 0x36);
    outb(0x40, low);
    outb(0x40, high);
}