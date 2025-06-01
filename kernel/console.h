#include <stdint.h>

typedef enum{
    CF_BLUE = 0b00000001,
    CF_GREEN = 0b00000010,
    CF_RED = 0b00000100,
    CF_BRIGHT = 0b00001000,
} color_flag_t;

void putchar(uint8_t c);
void cputchar(uint8_t c, color_flag_t cf);
void print_string(const uint8_t* s);