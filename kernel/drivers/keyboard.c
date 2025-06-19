#include "keyboard.h"
#include "io.h"
#include "ps2.h"
#include "utils.h"
#include <stdint.h>

#define KEYBOARD_ACK (0xFA)
#define KEYBOARD_RESEND (0xFE)

#define GETSET_SCANCODE (0xF0)
#define GET_SCANCODE (0x00)
#define SET_SCANCODE1 (0x01)
#define SET_SCANCODE2 (0x02)
#define SET_SCANCODE3 (0x03)

#define keyboard_success() (inb(KEYBOARD_DATA_PORT) == KEYBOARD_ACK)

#define SCANCODE_BUF_SZ (8)
#define KEYCODE_BUF_SZ (1024)

static struct keyboard_state{
    scancode_t scancode_buf[SCANCODE_BUF_SZ];
    volatile size_t scancode_c;

    keycode_t keycode_buf[KEYCODE_BUF_SZ];
    volatile size_t keycode_c;

    keyboard_flags flags;
}keyboard_state;

static void init_keyboard_state();
static void reinterpret_scancode();
static keycode_t reinterpet_scan_to_key();

//return value in the 0'st bit
static inline int is_shift_pressed();
static inline int is_capslock_enabled();
//check keycode_buf if there any ON_RELEASE
static inline int is_key_release();
static inline void process_scancode_to_key(keycode_t* kc);


static inline void push_scancode(scancode_t sc);
static inline scancode_t pop_scancode();
static inline int is_empty_scancode();

static inline void push_keycode(keycode_t kc);
static inline keycode_t pop_keycode();
static inline int is_empty_keycode();

int init_keyboard(){
    //read input, clear buffer
    clear_PS2_data_port();

    //set scancode set 2
    int32_t tries = 0;
    for(; tries < 3; tries++){
        outb(KEYBOARD_DATA_PORT, GETSET_SCANCODE);
        io_wait();
        if (!keyboard_success())
            continue;
        io_wait();
        outb(KEYBOARD_DATA_PORT, SET_SCANCODE2);
        io_wait();
        if (keyboard_success())
            break;
        io_wait();
    }
    if (tries == 3) {
        return 0;
    }
    init_keyboard_state();
    return 1;
}

void process_scancode(){
    //push all scancodes into the stack
    while(PS2_OUTPUT_BUFFER_STATUS(inb(PS2_STATUS_REGISTER)))
        push_scancode(inb(KEYBOARD_DATA_PORT));
    //reinterpret scancode sequence
    reinterpret_scancode();
    keyboard_state.scancode_c = 0;
}


static inline void push_scancode(scancode_t sc){
    if(keyboard_state.scancode_c >= SCANCODE_BUF_SZ)
        PANIC("Scancode buffer is full!");
    keyboard_state.scancode_buf[keyboard_state.scancode_c++] = sc;
}
static inline scancode_t pop_scancode(){
    if(keyboard_state.scancode_c <= 0)
        PANIC("Scancode buffer is empty!");
    return keyboard_state.scancode_buf[--keyboard_state.scancode_c];
}
static inline int is_empty_scancode(){
    return keyboard_state.scancode_c == 0;
}

static inline void push_keycode(keycode_t kc){
    if(keyboard_state.keycode_c >= SCANCODE_BUF_SZ)
        PANIC("Keycode buffer is full!");
    keyboard_state.keycode_buf[keyboard_state.keycode_c++] = kc;
}
static inline keycode_t pop_keycode(){
    if(keyboard_state.keycode_c <= 0)
        PANIC("Keycode buffer is empty!");
    return keyboard_state.keycode_buf[--keyboard_state.keycode_c];
}
static inline int is_empty_keycode(){
    return keyboard_state.keycode_c == 0;
}


static void init_keyboard_state(){
    memset(&keyboard_state, 0, sizeof(keyboard_state));
}

static void reinterpret_scancode(){
    push_keycode(reinterpet_scan_to_key());
    io_process_keycode();
}

static keycode_t reinterpet_scan_to_key(){
    keycode_t kc = (keyboard_state.flags << 8);
    process_scancode_to_key(&kc);
    switch (KC_KEY(kc)) {
        case SCANCODE_CAPSLOCK:{ 
            if(!KC_IS_ON_RELEASE(kc))
                keyboard_state.flags ^= KF_CAPSLOCK;
            break;
        }
        case SCANCODE_RSHIFT: case SCANCODE_LSHIFT:{
            if (KC_IS_ON_RELEASE(kc)) {
                keyboard_state.flags &= (~KF_SHIFT);
            }else{
                keyboard_state.flags |= KF_SHIFT;
            }
            break;
        }
    }
    //printf("=0x%X=\n", kc);
    return kc;
}

static inline int is_shift_pressed(){
    return (keyboard_state.flags & KF_SHIFT) >> 1;
}
static inline int is_capslock_enabled(){
    return keyboard_state.flags & KF_CAPSLOCK;
}

keycode_t getkeycode(){
    while (is_empty_keycode());
    return pop_keycode();
}

static inline int is_key_release(){
    for(size_t i = 0; i < keyboard_state.scancode_c; ++i)
        if(keyboard_state.scancode_buf[i] == SCANCODE_ON_RELEASE)
            return 1;
    return 0;
}

static inline void process_scancode_to_key(keycode_t* kc){
    //stupid processing of scancodes
    //will not work for the whole set of keys
    while(!is_empty_scancode()){
        scancode_t sc = pop_scancode();
        switch (sc) {
            case SCANCODE_MISC:
                *kc |= (KF_MISC << 8);
                break;
            case SCANCODE_ON_RELEASE:
                *kc |= (KF_ON_RELEASE << 8);
                break;
            default:
                *kc = (*kc & 0xFF00) + sc;
                break;
        }
    }
}