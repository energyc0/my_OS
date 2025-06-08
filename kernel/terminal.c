#include "terminal.h"
#include "utils.h"
#include <stdarg.h>

int32_t cursor_x = 0;
int32_t cursor_y = 0;
uint8_t attrib = CA_FORE_RED | CA_FORE_GREEN | CA_FORE_BLUE;

#define VIDEOMEMORY ((volatile uint16_t*)0xb8000)

void putchar(char c){
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

void print_string(const char* s){
    while (*s) 
        putchar(*s++);
    update_cursor();
}

void println(const char* s){
    print_string(s);
    putchar('\n');
    putchar('\r');
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

void printf(const char* fmt, ...){
    static char buf[256];
    va_list ap;
    va_start(ap, fmt);
    for (; *fmt; ++fmt) {
        if(*fmt == '%'){
            switch (*(++fmt)) {
                case '\0': putchar('%'); --fmt; break;
                case 'd': case 'i':{
                    int32_t val = va_arg(ap, int32_t);
                    print_string(itoa(val, buf, 10));
                    break;
                }
                case 'O': case 'o':{
                    int32_t val = va_arg(ap, int32_t);
                    buf[0] = '0';
                    itoa(val, buf+1, 8);
                    print_string(buf);
                    break;
                }
                case 'X': case 'x':{
                    int32_t val = va_arg(ap, int32_t);
                    buf[0] = '0';
                    buf[1] = 'x';
                    itoa(val, buf+2, 16);
                    print_string(*fmt == 'x' ? makelower(buf) : buf);
                    break;
                }
                case 's':{
                    const char* s = va_arg(ap,const char*);
                    print_string(s);
                    break;
                }
                case 'c': {
                    uint32_t c = va_arg(ap,uint32_t);
                    putchar(c);
                    break;
                }
                case '%': putchar('%'); break;
            }
        }else{
            putchar(*fmt);
        }
    }
    va_end(ap);
}

void move_cursor(int32_t x, int32_t y){
    cursor_x = x;
    cursor_y = y;
}

void clear_screen(){
    const size_t sz = CONSOLE_WIDTH * CONSOLE_HEIGHT;
    for (size_t i = 0; i < sz; ++i) {
        VIDEOMEMORY[i] = (attrib<<8);
    }
}