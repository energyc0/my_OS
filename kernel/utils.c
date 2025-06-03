#include "utils.h"
#include <stdint.h>

char* itoa(int32_t value, char* str, uint32_t base){
    const char symbols[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t idx = 0;
    if(value < 0){
        value *= -1;
        str[idx++] = '-';
    }
    while (value > 0){
        str[idx++] = symbols[value % base];
        value /= base;
    }
    if(idx == 0)
        str[idx++] = '0';
    str[idx--] = '\0';

    size_t start = str[0] == '-' ? 1 : 0;
    while (start < idx){
        swap(str[start], str[idx], uint8_t);
        ++start; --idx;
    }
    return str;
}

size_t strlen(const char* str){
    size_t len = 0;
    while (str[len])
        ++len;
    return len;
}

int32_t strcmp(const char* s1, const char* s2){
    while (*s1 == *s2 && *s1){
        ++s1;++s2;
    }
    return *s1 - *s2;
}

void* memcpy(void* dest, const void* src, size_t count){
    for(size_t i = 0; i < count; ++i)
        ((char*)dest)[i] = ((char*)src)[i];
    return dest;
}   
void* memset(void* dest, char ch, size_t count){
    for(size_t i = 0; i < count; ++i)
        ((char*)dest)[i] = ch;
    return dest;
}