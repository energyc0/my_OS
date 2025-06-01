#include "console.h"
#include <stdint.h>

uint16_t cursor_x = 0;
uint16_t cursor_y = 0;

#define VIDEOMEMORY ((volatile uint16_t*)0xb8000)

void putchar(uint8_t c){
    volatile uint16_t* where = VIDEOMEMORY + cursor_x++;
    *where = ((CF_RED | CF_GREEN | CF_BLUE) << 8) + c;
}

void cputchar(uint8_t c, color_flag_t cf){
    volatile uint16_t* where = VIDEOMEMORY + cursor_x++;
    *where = (cf << 8) + c;
}

void print_string(const uint8_t* s){
    while (*s) 
        putchar(*s++);
}