#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stdint.h>
#include <stddef.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...);
int snprintf(char* str, size_t size, const char* format, ...);
int putchar(int);
int puts(const char*);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void insw(uint16_t port, unsigned char* data, uint32_t count);
void outsw(uint16_t port, unsigned char * data, uint32_t count);
void io_wait();

#ifdef __cplusplus
}
#endif

#endif
