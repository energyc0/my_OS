#include <stdarg.h>
#include "io.h"
#include "utils.h"
#include "terminal.h"

static inline void __putchar(char c){
    switch (c) {
        case '\n': proc_newline(); proc_carriage_ret(); return;
        case '\r': proc_carriage_ret(); return;
    }
    drawchar(c);
    advance_cursor();
}

static inline void __print_string(const char* s){
    while (*s) 
        __putchar(*s++);
}

void putchar(char c){
    __putchar(c);
    update_cursor();
}

void print_string(const char* s){
    __print_string(s);
    update_cursor();
}

void println(const char* s){
    __print_string(s);
    putchar('\n');
}

void printf(const char* fmt, ...){
    static char buf[256];
    va_list ap;
    va_start(ap, fmt);
    for (; *fmt; ++fmt) {
        if(*fmt == '%'){
            switch (*(++fmt)) {
                case '\0': __putchar('%'); --fmt; break;
                case 'd': case 'i':{
                    int32_t val = va_arg(ap, int32_t);
                    __print_string(itoa(val, buf, 10));
                    break;
                }
                case 'O': case 'o':{
                    int32_t val = va_arg(ap, int32_t);
                    buf[0] = '0';
                    itoa(val, buf+1, 8);
                    __print_string(buf);
                    break;
                }
                case 'X': case 'x':{
                    int32_t val = va_arg(ap, int32_t);
                    buf[0] = '0';
                    buf[1] = 'x';
                    itoa(val, buf+2, 16);
                    __print_string(*fmt == 'x' ? makelower(buf) : buf);
                    break;
                }
                case 's':{
                    const char* s = va_arg(ap,const char*);
                    __print_string(s);
                    break;
                }
                case 'c': {
                    uint32_t c = va_arg(ap,uint32_t);
                    __putchar(c);
                    break;
                }
                case 'p':{
                    uint32_t p = va_arg(ap,uint32_t);
                    buf[0] = '0';
                    buf[1] = 'x';
                    itoa(p, buf+2, 16);
                    __print_string(buf);
                    break;
                }
                case '%': __putchar('%'); break;
            }
        }else{
            __putchar(*fmt);
        }
    }
    va_end(ap);
    update_cursor();
}