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

char tolower(char c){
    if('A' <= c && c <= 'Z')
        c += 32;
    return c;
}

char toupper(char c){
    if('a' <= c && c <= 'z')
        c -= 32;
    return c;
}

char* makelower(char* s){
    char*ptr = s;
    while(*ptr){
        *ptr = tolower(*ptr);
        ++ptr;
    }
    return s;
}

int isalpha(char c){
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

int isdigit(char c){
    return '0' <= c && c <= '9';
}

uint8_t inb(uint16_t port){
    uint8_t ret;
    asm volatile ("inb %0, %1" 
        : "=a"(ret) 
        : "Nd"(port) : "memory");
    return ret;
}
void outb(uint16_t port, uint8_t byte){
    asm volatile ("outb %0, %1" 
        : : "Nd"(port), "a"(byte) : "memory");
}