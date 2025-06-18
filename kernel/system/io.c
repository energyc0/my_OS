#include <stdarg.h>
#include "io.h"
#include "keyboard.h"
#include "utils.h"
#include "terminal.h"

//buffer used to store characters to read input from user
#define INBUFSIZ (1024)
static struct inbuf{
    volatile char buf[INBUFSIZ];
    volatile size_t count;
} inbuf;

static inline void inbuf_push(int c);
static inline int inbuf_pop();
static inline int inbuf_empty();

#define FMT_BUF_SIZE (8192)
#define PARSE_FMT_SUCCESS (0)
#define PARSE_FMT_FAIL (1)
static inline int __proc_fmt(char** buf_out, char specifier, int width, int precision, va_list* ap, size_t* chars_written);
//parses format and moves the fmt pointer
static inline void __parse_fmt(const char** fmt, int* width, int* precision);

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
    size_t idx = 0;
    for (; *fmt; ++fmt) {
        if(*fmt == '%'){
            const char* start_fmt = ++fmt;
            const size_t start_idx = idx;
            int width, precision;
            char* buf;
            __parse_fmt(&fmt, &width, &precision);
            switch (__proc_fmt(&buf, *fmt, width, precision, &ap, &idx)) {
                case PARSE_FMT_FAIL: 
                    if(*fmt == '\0') --fmt; 
                    strncpy(buf,start_fmt, fmt - start_fmt + 1);
                    buf[fmt - start_fmt + 1] = '\0';
                case PARSE_FMT_SUCCESS:
                    strncpy(str+start_idx, buf, idx - start_idx); 
                default: 
                    break;
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
    size_t chars_written = 0;

    va_list ap;
    va_start(ap, fmt);

    for (; *fmt; ++fmt) {
        if(*fmt == '%'){
            const char* start_fmt = ++fmt;
            int width, precision;
            char* buf;
            __parse_fmt(&fmt, &width, &precision);
            switch (__proc_fmt(&buf, *fmt, width, precision, &ap, &chars_written)) {
                case PARSE_FMT_FAIL: 
                    if(*fmt == '\0') --fmt; 
                    strncpy(buf,start_fmt, fmt - start_fmt + 1);
                    buf[fmt - start_fmt + 1] = '\0';
                case PARSE_FMT_SUCCESS: 
                    __print_string(buf);
                default: 
                    break;
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

static inline void __parse_fmt(const char** fmt, int* width, int* precision){
    *width = 0;
    *precision = 0;
    if(isdigit(**fmt))
        *width = atoi(*fmt);
    for (; isdigit(**fmt); ++(*fmt));
    if(**fmt == '.')
        *precision = atoi(++(*fmt));
    for (; isdigit(**fmt); ++(*fmt));
}

static inline int __proc_fmt(char** buf_out, char specifier, int width, int precision, va_list* ap, size_t* chars_written){
    static char buf[FMT_BUF_SIZE];
    *buf_out = buf;
    buf[0] = '\0';
    switch (specifier) {
        case 'd': case 'i':{
            int val = va_arg(*ap, int);
            itoa(val, buf, 10);
            break;
        }
        case 'O': case 'o':{
            unsigned val = va_arg(*ap, unsigned);
            itoa(val, buf, 8);
            break;
        }
        case 'X': case 'x':{
            unsigned val = va_arg(*ap, unsigned);
            itoa(val, buf, 16);
            if(specifier == 'x')
                makelower(buf);
            break;
        }
        case 's':{
            const char* s = va_arg(*ap,const char*);
            strcpy(buf,s);
            break;
        }
        case 'c': {
            int c = va_arg(*ap,int);
            buf[0] = c;
            buf[1] = '\0';
            break;
        }
        case 'p':{
            uint32_t p = va_arg(*ap,uint32_t);
            itoa(p, buf, 16);
            break;
        }
        case 'n':{
            size_t* p = va_arg(*ap,size_t*);
            *p = *chars_written;
            return PARSE_FMT_SUCCESS;
        }
        default: return PARSE_FMT_FAIL;
    }
    *chars_written += strlen(buf);
    return PARSE_FMT_SUCCESS;
}