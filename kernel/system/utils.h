#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include "interrupts.h"

#define NULL (0)

typedef uint32_t size_t;

#define swap(a,b,type) do {type temp = a; a = b; b = temp;} while(0)

//str must be a buffer containing enough space for the value
char* itoa(int32_t value, char* str, uint32_t base);
size_t strlen(const char* str);
int strcmp(const char* s1, const char* s2);
char* strchr(const char* str, int character);
char* strcat(char* dest, const char* src);

int toupper(int c);
int tolower(int c);
int isalpha(int c);
int isdigit(int c);
int isspace(int c);
char* makelower(char* s);

void* memcpy(void* dest, const void* src, size_t count);
void* memset(void* dest, char ch, size_t count );

void sleep(unsigned int ms);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t byte);

uint64_t __udivdi3(uint64_t a, uint64_t b);

#define io_wait() outb(0x80,0)

#endif
