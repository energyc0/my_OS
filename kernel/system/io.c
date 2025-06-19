#include <stdarg.h>
#include <stdint.h>
#include "io.h"
#include "interrupts.h"
#include "keyboard.h"
#include "utils.h"
#include "terminal.h"

#define STACK_SIZE (BUFSIZ)
typedef struct stack_t{
    volatile char buf[STACK_SIZE];
    volatile size_t count;
} stack_t;

static inline void st_push(stack_t* st, int c);
static inline int st_pop(stack_t* st);
static inline int st_empty(const stack_t* st);
//copy src buf to dst;
//static inline void st_flush(stack_t* dst, const stack_t* src);

#define FMT_BUF_SIZE (8192)
#define PARSE_FMT_SUCCESS (0)
#define PARSE_FMT_FAIL (1)
//draw char on the screen and advance the cursor
static inline void __putchar(int c);
//moves cursor backward and erases the previous char
static inline void __deletechar();
//draw string on the screen via __putchar()
static inline void __print_string(const char* s);
static inline int __proc_fmt(char** buf_out, char specifier, int width, int precision, char padding, va_list* ap, size_t* chars_written);
//parses format and moves the fmt pointer
static inline void __parse_fmt(const char** fmt, int* width, int* precision, char* padding);
//if len < width, then add 'padding' in the beginning (width - len) times
static inline void __correct_width(char* buf, int width, size_t len, char padding);

//buffer used to store keycodes related to terminal until '\n' character
static stack_t keycode_buf;
//buffer used to store user input in characters
static stack_t inbuf;

//return code of operation to perform
static inline int __is_symbolic(scancode_t sc);
static inline void __flush_keycodes();

static inline void __putchar(int c){
    switch (c) {
        case '\n': proc_newline(); proc_carriage_ret(); return;
        case '\r': proc_carriage_ret(); return;
    }
    drawchar(c);
    advance_cursor();
}

static inline void __deletechar(){
    backward_cursor();
    erasechar();
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
            char padding;
            __parse_fmt(&fmt, &width, &precision, &padding);
            switch (__proc_fmt(&buf, *fmt, width, precision,padding, &ap, &idx)) {
                case PARSE_FMT_FAIL: 
                    if(*fmt == '\0') --fmt; 
                    strncpy(buf,start_fmt, fmt - start_fmt + 1);
                    buf[fmt - start_fmt + 1] = '\0';
                    __attribute__ ((fallthrough));
                case PARSE_FMT_SUCCESS:
                    strncpy(str+start_idx, buf, idx - start_idx);
                    __attribute__ ((fallthrough)); 
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
            char padding;
            __parse_fmt(&fmt, &width, &precision, &padding);
            switch (__proc_fmt(&buf, *fmt, width, precision,padding, &ap, &chars_written)) {
                case PARSE_FMT_FAIL: 
                    if(*fmt == '\0') --fmt; 
                    strncpy(buf,start_fmt, fmt - start_fmt + 1);
                    buf[fmt - start_fmt + 1] = '\0';
                    __attribute__ ((fallthrough));
                case PARSE_FMT_SUCCESS: 
                    __print_string(buf);
                    __attribute__ ((fallthrough));
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

static inline void st_push(stack_t* st, int c){
    if (st->count < STACK_SIZE)
        st->buf[st->count++] = c;
}
static inline int st_pop(stack_t* st){
    return st->buf[--(st->count)];
}

static inline int st_empty(const stack_t* st){
    return st->count == 0;
}

int getchar(){
    while (st_empty(&inbuf));
    return st_pop(&inbuf);
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

static inline int __is_symbolic(scancode_t sc){
    switch (sc) {
        case SCANCODE_A:case SCANCODE_B:case SCANCODE_C:case SCANCODE_D:
        case SCANCODE_E:case SCANCODE_F:case SCANCODE_G:case SCANCODE_H:
        case SCANCODE_I:case SCANCODE_J:case SCANCODE_K:case SCANCODE_L:
        case SCANCODE_M:case SCANCODE_N:case SCANCODE_O:case SCANCODE_P:
        case SCANCODE_Q:case SCANCODE_R:case SCANCODE_S:case SCANCODE_T:
        case SCANCODE_U:case SCANCODE_V:case SCANCODE_W:case SCANCODE_X:
        case SCANCODE_Y:case SCANCODE_Z:case SCANCODE_1:case SCANCODE_2:
        case SCANCODE_3:case SCANCODE_4:case SCANCODE_5:case SCANCODE_6:
        case SCANCODE_7:case SCANCODE_8:case SCANCODE_9:case SCANCODE_0:
        case SCANCODE_SPACE:case SCANCODE_COMMA:case SCANCODE_PERIOD:case SCANCODE_ENTER: return 1;
        default: return 0;
    }
}

//interrupts disabled
void io_process_keycode(){
    keycode_t kc = getkeycode();
    if(KC_IS_ON_RELEASE(kc))
        return;

    if(KC_KEY(kc) == SCANCODE_BACKSPACE){
        if(!st_empty(&keycode_buf)){
            st_pop(&keycode_buf);
            __deletechar();
            update_cursor();
        }
    }else if(__is_symbolic(KC_KEY(kc))){
        int ch;
        switch (KC_KEY(kc)) {
            case SCANCODE_A: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'A' : 'a' ; break;
            case SCANCODE_B: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'B' : 'b'; break;
            case SCANCODE_C: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'C' : 'c' ; break;
            case SCANCODE_D: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'D' : 'd'; break;
            case SCANCODE_E: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'E' : 'e'; break;
            case SCANCODE_F: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'F' : 'f'; break;
            case SCANCODE_G: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'G' : 'g'; break;
            case SCANCODE_H: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'H' : 'h'; break;
            case SCANCODE_I: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'I' : 'i'; break;
            case SCANCODE_J: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'J' : 'j'; break;
            case SCANCODE_K: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'K' : 'k'; break;
            case SCANCODE_L: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'L' : 'l'; break;
            case SCANCODE_M: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'M' : 'm'; break;
            case SCANCODE_N: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'N' : 'n'; break;
            case SCANCODE_O: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'O' : 'o'; break;
            case SCANCODE_P: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'P' : 'p'; break;
            case SCANCODE_Q: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'Q' : 'q'; break;
            case SCANCODE_R: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'R' : 'r'; break;
            case SCANCODE_S: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'S' : 's'; break;
            case SCANCODE_T: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'T' : 't'; break;
            case SCANCODE_U: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'U' : 'u'; break;
            case SCANCODE_V: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'V' : 'v'; break;
            case SCANCODE_W: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'W' : 'w'; break;
            case SCANCODE_X: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'X' : 'x'; break;
            case SCANCODE_Y: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'Y' : 'y'; break;
            case SCANCODE_Z: ch = (KC_IS_SHIFT(kc) ^ KC_IS_CAPSLOCK(kc)) ?  'Z' : 'z'; break;
            case SCANCODE_1: ch = !KC_IS_SHIFT(kc) ? '1' : '!'; break;
            case SCANCODE_2: ch = !KC_IS_SHIFT(kc) ? '2' : '@'; break;
            case SCANCODE_3: ch = !KC_IS_SHIFT(kc) ? '3' : '#'; break;
            case SCANCODE_4: ch = !KC_IS_SHIFT(kc) ? '4' : '$'; break;
            case SCANCODE_5: ch = !KC_IS_SHIFT(kc) ? '5' : '%'; break;
            case SCANCODE_6: ch = !KC_IS_SHIFT(kc) ? '6' : '^'; break;
            case SCANCODE_7: ch = !KC_IS_SHIFT(kc) ? '7' : '&'; break;
            case SCANCODE_8: ch = !KC_IS_SHIFT(kc) ? '8' : '*'; break;
            case SCANCODE_9: ch = !KC_IS_SHIFT(kc) ? '9' : '('; break;
            case SCANCODE_0: ch = !KC_IS_SHIFT(kc) ? '0' : ')'; break;
            case SCANCODE_SPACE: ch = ' '; break;
            case SCANCODE_COMMA: ch = !KC_IS_SHIFT(kc) ? ',' : '<'; break;
            case SCANCODE_PERIOD: ch = !KC_IS_SHIFT(kc) ? '.' : '>'; break;
            case SCANCODE_ENTER: ch = '\n';break;
            default: ch = '\0'; break;
        }
        if(ch != '\0'){
            st_push(&keycode_buf, ch);
            putchar(ch);
            if(ch == '\n')
                __flush_keycodes();
        }
    }
}

//interrupts disabled
static inline void __flush_keycodes(){
    for(; !st_empty(&keycode_buf); )
        st_push(&inbuf,st_pop(&keycode_buf));
}

static inline void __parse_fmt(const char** fmt, int* width, int* precision, char* padding){
    *width = -1;
    *precision = -1;
    *padding = (**fmt == '0') ? '0' : ' ';
    if(isdigit(**fmt))
        *width = atoi(*fmt);
    for (; isdigit(**fmt); ++(*fmt));
    if(**fmt == '.')
        *precision = atoi(++(*fmt));
    for (; isdigit(**fmt); ++(*fmt));
}

static inline int __proc_fmt(char** buf_out, char specifier, int width, int precision,char padding, va_list* ap, size_t* chars_written){
    static char buf[FMT_BUF_SIZE];
    *buf_out = buf;
    buf[0] = '\0';
    int len = -1;
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
    
    if(strchr("diOoXx", specifier)){
        len = strlen(buf);
        if(len < precision){
            __correct_width(buf, precision, len, '0');
            len = precision;
        }
    }else if(specifier == 's' && precision >= 0){
        len = strlen(buf);
        if(len > precision){
            len = precision;
            buf[len] = '\0';
        }
    }

    if(len == -1)
        len = strlen(buf);
    __correct_width(buf, width, len, padding);
    *chars_written += len;
    return PARSE_FMT_SUCCESS;
}

static inline void __correct_width(char* buf, int width, size_t len, char padding){
    if(width > 0 && len < (size_t)width){
        char temp[len+1];
        strcpy(temp, buf);
        memset(buf, padding, width-len);
        strcpy(buf + width - len, temp);
    }
}