#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);
void* malloc(size_t size);

typedef struct
{
	uint8_t *addr;
	uint32_t bytes;
} bitmap_type;

#ifdef __cplusplus
}
#endif

#endif
