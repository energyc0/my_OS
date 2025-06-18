#include <cstdarg>
#include <stdarg.h>
#include "io.h"
#include "keyboard.h"
#include "utils.h"
#include "terminal.h"

//buffer used to store characters to read input from user
#define INBUFSIZ 1024
static struct inbuf{
    volatile char buf[INBUFSIZ];
    volatile size_t count;
} inbuf;

static inline void inbuf_push(int c);
static inline int inbuf_pop();
static inline int inbuf_empty();

static inline size_t __parse_fmt(char* buf, char specifier, size_t width, size_t precision, va_list* ap);

static inline void __putchar(int c){
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

void putchar(int c){
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

int sprintf(char* str, const char* fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    int idx = 0;
    for (; *fmt; ++fmt) {
        if(*fmt == '%'){
            switch (*(++fmt)) {
                case '\0': str[idx] = '%'; --fmt;break;
                case 'd': case 'i':{
                    int val = va_arg(ap, int);
                    itoa(val, str + idx, 10);
                    for(;str[idx] != '\0'; ++idx);
                    break;
                }
                case 'O': case 'o':{
                    unsigned val = va_arg(ap, unsigned);
                    str[idx++] = '0';
                    itoa(val, str+idx, 8);
                    for(;str[idx] != '\0'; ++idx);
                    break;
                }
                case 'X': case 'x':{
                    unsigned val = va_arg(ap, unsigned);
                    str[idx++] = '0';
                    str[idx++] = 'x';
                    itoa(val, str+idx, 16);
                    if(*fmt == 'x')
                        makelower(str+idx);
                    for(;str[idx] != '\0'; ++idx);
                    break;
                }
                case 's':{
                    const char* s = va_arg(ap,const char*);
                    str[idx] = '\0';
                    strcat(str+idx, s);
                    idx+=strlen(s);
                    break;
                }
                case 'c': {
                    int c = va_arg(ap,int);
                    str[idx++] = c;
                    break;
                }
                case 'p':{
                    uint32_t p = va_arg(ap,uint32_t);
                    str[idx++] = '0';
                    str[idx++] = 'x';
                    itoa(p, str+idx, 16);
                    for(;str[idx] != '\0'; ++idx);
                    break;
                }
                case 'n':{
                    size_t* p = va_arg(ap,size_t*);
                    *p = idx;
                    break;
                }
                case '%': str[idx++] = '%'; break;
            }
        }else{
            str[idx++] = *fmt;
        }
    }
    va_end(ap);
    str[idx] = '\0';
    return idx;
}

void printf(const char* fmt, ...){
    static char buf[256];
    size_t chars_written = 0;
    size_t width = 0;
    size_t precision = 0;
    va_list ap;
    va_start(ap, fmt);

    for (; *fmt; ++fmt) {
        if(*fmt == '%'){
            switch (*(++fmt)) {
                case '\0': __putchar('%'); --fmt; break;
                case 'd': case 'i':{
                    int val = va_arg(ap, int);
                    __print_string(itoa(val, buf, 10));
                    chars_written+=strlen(buf);
                    break;
                }
                case 'O': case 'o':{
                    unsigned val = va_arg(ap, unsigned);
                    buf[0] = '0';
                    itoa(val, buf+1, 8);
                    __print_string(buf);
                    chars_written+=strlen(buf)+1;
                    break;
                }
                case 'X': case 'x':{
                    unsigned val = va_arg(ap, unsigned);
                    buf[0] = '0';
                    buf[1] = 'x';
                    itoa(val, buf+2, 16);
                    __print_string(*fmt == 'x' ? makelower(buf) : buf);
                    chars_written+=strlen(buf)+2;
                    break;
                }
                case 's':{
                    const char* s = va_arg(ap,const char*);
                    __print_string(s);
                    chars_written+=strlen(s);
                    break;
                }
                case 'c': {
                    int c = va_arg(ap,int);
                    __putchar(c);
                    ++chars_written;
                    break;
                }
                case 'p':{
                    uint32_t p = va_arg(ap,uint32_t);
                    buf[0] = '0';
                    buf[1] = 'x';
                    itoa(p, buf+2, 16);
                    __print_string(buf);
                    chars_written+=strlen(buf)+2;
                    break;
                }
                case 'n':{
                    size_t* p = va_arg(ap,size_t*);
                    *p = chars_written;
                    break;
                }
                case '%': __putchar('%'); ++chars_written; break;
            }
        }else{
            __putchar(*fmt);
            ++chars_written;
        }
    }
    va_end(ap);
    update_cursor();
}

static inline void inbuf_push(int c){
    if (inbuf.count < INBUFSIZ)
        inbuf.buf[inbuf.count++] = c;
}
static inline int inbuf_pop(){
    return inbuf.buf[--inbuf.count];
}

static inline int inbuf_empty(){
    return inbuf.count == 0;
}

int getchar(){
    while (inbuf_empty());
    return inbuf_pop();
}

size_t getline(char* buf, size_t bufsiz){
    size_t count = 0;
    while (count < bufsiz) {
        char ch = getchar();
        if((buf[count++] = ch) == '\n')
            break;
    }
    return count;
}

void io_process_keycode(){
    keycode_t kc = getkeycode();
    //if ascii character and not '\0'
    if(0 < kc && kc <= 127){
        inbuf_push(kc);
        putchar(kc);
    }
}

static inline size_t __parse_fmt(char* buf, char specifier, size_t width, size_t precision, va_list* ap){

}