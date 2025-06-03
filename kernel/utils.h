#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define NULL (0)

typedef uint32_t size_t;

#define swap(a,b,type) do {type temp = a; a = b; b = temp;} while(0)

char* itoa(int32_t value, char* str, uint32_t base);
size_t strlen(const char* str);
int32_t strcmp(const char* s1, const char* s2);

#endif
