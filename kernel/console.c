#include "console.h"
#include <stdint.h>

uint16_t cursor_x = 0;
uint16_t cursor_y = 0;

void putchar(uint8_t c){
    uint8_t color = 0b00000111;
    volatile uint16_t* where = (volatile uint16_t*)0xb8000 + cursor_x++;
    *where = (color << 8) + c;
}