#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include "interrupts.h"

#define NULL (0)

typedef uint32_t size_t;

#define swap(a,b,type) do {type temp = a; a = b; b = temp;} while(0)
#define PANIC(...) do { \
    printf(__VA_ARGS__); \
    disable_interrupts(); \
    while(1);   \
} while(0)

char* itoa(int32_t value, char* str, uint32_t base);
size_t strlen(const char* str);
int32_t strcmp(const char* s1, const char* s2);
char tolower(char c);
char* makelower(char* s);

void* memcpy(void* dest, const void* src, size_t count);
void* memset(void* dest, char ch, size_t count );

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t byte);

#define io_wait() outb(0x80,0)

#endif
