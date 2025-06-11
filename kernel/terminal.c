#include "terminal.h"
#include "keyboard.h"
#include "utils.h"
#include <stdarg.h>
#include <stdint.h>

int32_t cursor_x = 0;
int32_t cursor_y = 0;
char_attrib_t attrib = CA_FORE_RED | CA_FORE_GREEN | CA_FORE_BLUE;

#define VIDEOMEMORY ((volatile uint16_t*)0xb8000)
#define EMPTY_CHAR  ((uint16_t)((attrib << 8) + ' '))

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
    cursor_x = x % TERMINAL_WIDTH;
    cursor_y = y % TERMINAL_HEIGHT;
}

void get_cursor(int32_t* x, int32_t* y){
    *x = cursor_x;
    *y = cursor_y;
}

void clear_screen(){
    const size_t sz = TERMINAL_WIDTH * TERMINAL_HEIGHT;
    for (size_t i = 0; i < sz; ++i)
        VIDEOMEMORY[i] = EMPTY_CHAR;
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
    *where = EMPTY_CHAR;
}

void advance_cursor(){
    if(++cursor_x >= TERMINAL_WIDTH){
        cursor_x = 0;
        proc_newline();
    }
}

void backward_cursor(){
    if(--cursor_x < 0){
        if(--cursor_y < 0){
            cursor_y = 0;
            cursor_x = 0;
        }else{
            cursor_x = TERMINAL_WIDTH-1;
        }
    }
}

void proc_newline(){
    if(++cursor_y >= TERMINAL_HEIGHT){
        --cursor_y;
        scroll();
    }
}
void proc_carriage_ret(){
    cursor_x = 0;
}

void scroll(){
    int32_t y = 0;
    for(; y < TERMINAL_HEIGHT-1; y++)
        memcpy((void*)&VIDEOMEMORY[TERMINAL_WIDTH*y], (const void*)&VIDEOMEMORY[TERMINAL_WIDTH*(y+1)], TERMINAL_WIDTH*sizeof(VIDEOMEMORY[0]));

    for(int32_t x = 0; x < TERMINAL_WIDTH; ++x)
        VIDEOMEMORY[TERMINAL_WIDTH*y + x] = EMPTY_CHAR;
}