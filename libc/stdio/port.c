#include <stdio.h>
#include <stdint.h>

inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %b0, %w1" :: "a"(val), "Nd"(port) : "memory");
}

inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0"
                    : "=a" (ret)
                    : "Nd" (port)
                    : "memory");
    return ret;
}

inline void io_wait()
{
    outb(0x80, 0);
}

inline void insw(uint16_t port, unsigned char* data, uint32_t count)
{
    __asm__ volatile ("rep insw" : "+D" (data), "+c" (count) : "d" (port) : "memory");
}

inline void outsw(uint16_t port, unsigned char * data, uint32_t count) {
	asm volatile ("rep outsw" : "+S" (data), "+c" (count) : "d" (port));
}