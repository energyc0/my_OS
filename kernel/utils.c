#include "utils.h"
#include <stdint.h>

uint8_t* itoa(int32_t value, uint8_t* str, uint8_t base){
    const uint8_t symbols[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
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

size_t strlen(const uint8_t* str){
    size_t len = 0;
    while (str[len])
        ++len;
    return len;
}

int32_t strcmp(const uint8_t* s1, const uint8_t* s2){
    while (*s1 == *s2 && *s1){
        ++s1;++s2;
    }
    return *s1 - *s2;
}