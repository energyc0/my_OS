#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60

typedef enum scancode_t{
    ON_RELEASE = 0xF0,
    KEY_A = 0x1C,
    KEY_B = 0x32,
    KEY_C = 0x21,
    KEY_D = 0x23,
    KEY_E = 0x24,
    KEY_F = 0x2B,
    KEY_G = 0x34,
    KEY_H = 0x33,
    KEY_I = 0x43,
    KEY_J = 0x3B,
    KEY_K = 0x42,
    KEY_L = 0x4B,
    KEY_M = 0x3A,
    KEY_N = 0x31,
    KEY_O = 0x44,
    KEY_P = 0x4D,
    KEY_Q = 0x15,
    KEY_R = 0x2D,
    KEY_S = 0x1B,
    KEY_T = 0x2C,
    KEY_U = 0x3C,
    KEY_V = 0x2A,
    KEY_W = 0x1D,
    KEY_X = 0x22,
    KEY_Y = 0x35,
    KEY_Z = 0x1A,
    KEY_SPACE = 0x29,
    KEY_LSHIFT = 0x12,
    KEY_RSHIFT = 0x59,
    KEY_CAPSLOCK = 0x58,
    KEY_1 = 0x16,
    KEY_2 = 0x1E,
    KEY_3 = 0x26,
    KEY_4 = 0x25,
    KEY_5 = 0x2E,
    KEY_6 = 0x36,
    KEY_7 = 0x3D,
    KEY_8 = 0x3E,
    KEY_9 = 0x46,
    KEY_0 = 0x45,
    KEY_COMMA = 0x41,
    KEY_PERIOD = 0x49,
    KEY_ENTER = 0x5A,
    KEY_BACKSPACE = 0x0E
}scancode_t;

typedef enum keyboard_flag_bit{
    KF_CAPSLOCK = 0x1,
    KF_SHIFT = 0x2
}keyboard_flag_bit;

typedef uint8_t keyboard_flags;

//set scancode set 2
int init_keyboard();
//should be called in ISR
void process_scancode();

//return value in the 0'st bit
int is_shift_pressed();
int is_capslock_enabled();

char reinterpet_key_to_char(scancode_t scancode);

#endif