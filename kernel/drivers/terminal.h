#ifndef __TERMINAL_H
#define __TERMINAL_H

#include <stdint.h>
#include "keyboard.h"

typedef enum{
    CA_FORE_BLUE = 0b00000001,
    CA_FORE_GREEN = 0b00000010,
    CA_FORE_RED = 0b00000100,
    CA_FORE_BRIGHT = 0b00001000,
    CA_BACK_BLUE = 0b00010000,
    CA_BACK_GREEN = 0b00100000,
    CA_BACK_RED = 0b01000000,
    CA_BACK_BRIGHT = 0b10000000
} char_attrib_t;

#define TERMINAL_WIDTH (80)
#define TERMINAL_HEIGHT (25)

//draws char on the cursor position
void drawchar(char ch);
//erases char on the cursor position
void erasechar();

//moves cursor in the next line
void proc_newline();
//moves cursor in the start of the line
void proc_carriage_ret();

//moves cursor forward
void advance_cursor();
//moves cursor backward
void backward_cursor();

void clear_screen();
//moves cursor without changing its position on the screen
void move_cursor(int32_t x, int32_t y);
void get_cursor(int32_t* x, int32_t* y);
void set_char_attrib(char_attrib_t attr);
//updates cursor position on the screen
void update_cursor();
void scroll();

#endif