#ifndef PCSPK_H
#define PCSPK_H

#include <stdint.h>
#include <drivers/timer.h>

void play_sound(uint32_t freq);
void nosound();
void beep();
void beep_freq();

#endif