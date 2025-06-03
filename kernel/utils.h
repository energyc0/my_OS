#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define NULL (0)

typedef uint32_t size_t;

#define swap(a,b,type) do {type temp = a; a = b; b = temp;} while(0)

uint8_t* itoa(int32_t value, uint8_t* str, uint8_t base);
size_t strlen(const uint8_t* str);
int32_t strcmp(const uint8_t* s1, const uint8_t* s2);

#endif
