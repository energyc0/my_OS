#include "terminal.h"
#include "keyboard.h"
#include "utils.h"
#include <stdarg.h>
#include <stdint.h>

int32_t cursor_x = 0;
int32_t cursor_y = 0;
uint8_t attrib = CA_FORE_RED | CA_FORE_GREEN | CA_FORE_BLUE;

#define VIDEOMEMORY ((volatile uint16_t*)0xb8000)

void set_char_attrib(char_attrib_t attr){
    attrib = attr;
}

void update_cursor(){
    uint16_t pos = cursor_y * TERMINAL_WIDTH + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8));
    outb(0x3D4, 15);
    outb(0x3D5, (pos));
}

void move_cursor(int32_t x, int32_t y){
    cursor_x = x;
    cursor_y = y;
}

void get_cursor(int32_t* x, int32_t* y){
    *x = cursor_x;
    *y = cursor_y;
}

void clear_screen(){
    const size_t sz = TERMINAL_WIDTH * TERMINAL_HEIGHT;
    for (size_t i = 0; i < sz; ++i) {
        VIDEOMEMORY[i] = (attrib<<8);
    }
}

void send_symbol_to_terminal(scancode_t scancode){
    char ch = reinterpet_key_to_char(scancode);
    if (ch != '\0') {
        if(isalpha(ch))
            putchar((is_shift_pressed() ^ is_capslock_enabled()) ? toupper(ch) : ch);
        else if(isdigit(ch))
            putchar(")!@#$%^&*("[ch - '0']);
        else
            putchar(ch);
        update_cursor();
    }else{
       // switch (scancode) {
        //    case KEY_BACKSPACE:
        //}
    }
}


void drawchar(char ch){
    volatile uint16_t* where = VIDEOMEMORY + cursor_y*TERMINAL_WIDTH + cursor_x;
    *where = (attrib << 8) + ch;
}
void erasechar(){
    volatile uint16_t* where = VIDEOMEMORY + cursor_y*TERMINAL_WIDTH + cursor_x;
    *where = (attrib << 8) + ' ';
}

void advance_cursor(){
    if(++cursor_x >= TERMINAL_WIDTH){
        cursor_x = 0;
        ++cursor_y;
    }
}

void backward_cursor(){
    if(--cursor_x < 0){
        cursor_x = TERMINAL_WIDTH-1;
        --cursor_y;
    }
}

void proc_newline(){
    ++cursor_y;
}
void proc_carriage_ret(){
    cursor_x = 0;
}