#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

size_t get_row();
size_t get_col();
void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_scroll(void);
void terminal_setcolor(uint8_t color);
void terminal_writestringat(const char* data, size_t x, size_t y);
void terminal_backspace();

#endif
