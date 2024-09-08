#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);
char *strcat(char *dest, const char *src);
char *strncat(char *s1, const char *s2, size_t n);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dst, const char* src, register size_t n);
char *strchr(const char *s, int c);
char *strrchr(register const char *s, int c);
int strcmp(const char *s1, const char *s2);
#ifdef __cplusplus
}
#endif

#endif
