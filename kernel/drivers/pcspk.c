#include <drivers/pcspk.h>
#include <drivers/timer.h>
#include <stdio.h>

void play_sound(uint32_t freq) {
    uint32_t div;
    uint8_t tmp;

        //Set the PIT to the desired frequency
    div = 1193180 / freq;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t) (div) );
    outb(0x42, (uint8_t) (div >> 8));

        //And play the sound using the PC speaker
    tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}

//make it shut up
void nosound() {
    uint8_t tmp = inb(0x61) & 0xFC;
    
    outb(0x61, tmp);
}

//Make a beep
void beep() {
    beep_freq(1000);
}

void beep_freq(uint32_t f) {
    play_sound(f);
    timer_wait(1);
    nosound();
    timer_set(1);
}