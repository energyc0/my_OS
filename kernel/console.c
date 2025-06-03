#include "console.h"
#include "io.h"
#include <stdint.h>

uint16_t cursor_x = 0;
uint16_t cursor_y = 0;
uint8_t attrib = CA_FORE_RED | CA_FORE_GREEN | CA_FORE_BLUE;

#define VIDEOMEMORY ((volatile uint16_t*)0xb8000)

void putchar(uint8_t c){
    switch (c) {
        case '\n': cursor_y++; return;
        case '\r': cursor_x = 0; return;
    }
    volatile uint16_t* where = VIDEOMEMORY + cursor_y*CONSOLE_WIDTH + cursor_x++;
    *where = (attrib << 8) + c;
    if(cursor_x >= CONSOLE_WIDTH){
        cursor_x = 0;
        cursor_y++;
    }
}

void print_string(const uint8_t* s){
    while (*s) 
        putchar(*s++);
    update_cursor();
}

void set_char_attrib(char_attrib_t attr){
    attrib = attr;
}

void update_cursor(){
    uint16_t pos = cursor_y * CONSOLE_WIDTH + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8));
    outb(0x3D4, 15);
    outb(0x3D5, (pos));
}