#include "utils.h"
#include "timer.h"
#include <stdint.h>

extern uint64_t tick_counter;

static inline void halt();

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

int strcmp(const char* s1, const char* s2){
    while (*s1 == *s2 && *s1){
        ++s1;++s2;
    }
    return *s1 - *s2;
}

char* strchr(const char* str, int character){
    for(;*str != '\0' && *str != character; ++str);
    return (char*)((*str == character) ? str : NULL);
}

char* strcat(char* dest, const char* src){
    size_t idx = strlen(dest);
    for (;*src != '\0'; ++src) {
        dest[idx++] = *src;
    }
    dest[idx] = '\0';
    return dest;
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

int tolower(int c){
    if('A' <= c && c <= 'Z')
        c += 32;
    return c;
}

int toupper(int c){
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

int isalpha(int c){
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

int isdigit(int c){
    return '0' <= c && c <= '9';
}

int isspace(int c){
    return c == ' ' || c == '\t' || c == '\n' || c == '\f' || c == '\r' || c == '\v';
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

void sleep(unsigned int ms){
    uint64_t start_time = tick_counter;
    while(tick_counter - start_time < ms)
        halt();
}

static inline void halt(){
    asm volatile("hlt");
}

uint64_t __udivdi3(uint64_t a, uint64_t b){
    uint64_t res = 0;
    while (a >= b) {
        a-=b;
        ++res;
    }
    return res;
}

int atoi(const char* str){
    int res = 0;

    for (; *str != '\0' && isspace(*str); ++str);
    int sign = *str == '-' ? -1 : 1;
    if (*str == '-' || *str == '+')
        ++str;

    for (; *str != '\0' && *str == '0'; ++str);
    for(;*str != '\0' && isdigit(*str); ++str){
        res = res*10 + *str - '0';
    }
    return res*sign;
}

char* strcpy(char* dest, const char* src){
    size_t i = 0;
    for(; src[i] != '\0'; ++i)
        dest[i] = src[i];
    dest[i] = '\0';
    return dest;
}

char* strncpy(char* dest, const char* src, size_t num){
    for(size_t i = 0; i < num; ++i){
        dest[i] = src[i];
        if (src[i] == '\0')
            break;
    }
    
    return dest;
}