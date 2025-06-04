#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

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

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25

void putchar(char c);
void print_string(const char* s);
void println(const char* s);
void printf(const char* fmt, ...);

void clear_screen();
void move_cursor(int32_t x, int32_t y);
void set_char_attrib(char_attrib_t attr);
void update_cursor();

#endif