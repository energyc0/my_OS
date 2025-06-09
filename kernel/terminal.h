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

#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 25


void drawchar(char ch);
void erasechar();

void proc_newline();
void proc_carriage_ret();

void advance_cursor();
void backward_cursor();

void clear_screen();
void move_cursor(int32_t x, int32_t y);
void get_cursor(int32_t* x, int32_t* y);
void set_char_attrib(char_attrib_t attr);
void update_cursor();

void send_symbol_to_terminal(scancode_t scancode);

#endif