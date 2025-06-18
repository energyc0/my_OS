#ifndef __IO_H
#define __IO_H

#include "keyboard.h"
#include "utils.h"
#include "interrupts.h"

void putchar(int c);
void print_string(const char* s);
void println(const char* s);
void printf(const char* fmt, ...);
int sprintf(char* str, const char* fmt, ...);

int getchar();
size_t getline(char* buf, size_t bufsiz);
void io_process_keycode();

#define PANIC(...) do { \
    printf(__VA_ARGS__); \
    disable_interrupts(); \
    while(1);   \
} while(0)

#endif