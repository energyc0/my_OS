#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60

typedef enum scancode_t{
    SCANCODE_ON_RELEASE = 0xF0,
    SCANCODE_A = 0x1C,
    SCANCODE_B = 0x32,
    SCANCODE_C = 0x21,
    SCANCODE_D = 0x23,
    SCANCODE_E = 0x24,
    SCANCODE_F = 0x2B,
    SCANCODE_G = 0x34,
    SCANCODE_H = 0x33,
    SCANCODE_I = 0x43,
    SCANCODE_J = 0x3B,
    SCANCODE_K = 0x42,
    SCANCODE_L = 0x4B,
    SCANCODE_M = 0x3A,
    SCANCODE_N = 0x31,
    SCANCODE_O = 0x44,
    SCANCODE_P = 0x4D,
    SCANCODE_Q = 0x15,
    SCANCODE_R = 0x2D,
    SCANCODE_S = 0x1B,
    SCANCODE_T = 0x2C,
    SCANCODE_U = 0x3C,
    SCANCODE_V = 0x2A,
    SCANCODE_W = 0x1D,
    SCANCODE_X = 0x22,
    SCANCODE_Y = 0x35,
    SCANCODE_Z = 0x1A,
    SCANCODE_SPACE = 0x29,
    SCANCODE_LSHIFT = 0x12,
    SCANCODE_RSHIFT = 0x59,
    SCANCODE_CAPSLOCK = 0x58,
    SCANCODE_1 = 0x16,
    SCANCODE_2 = 0x1E,
    SCANCODE_3 = 0x26,
    SCANCODE_4 = 0x25,
    SCANCODE_5 = 0x2E,
    SCANCODE_6 = 0x36,
    SCANCODE_7 = 0x3D,
    SCANCODE_8 = 0x3E,
    SCANCODE_9 = 0x46,
    SCANCODE_0 = 0x45,
    SCANCODE_COMMA = 0x41,
    SCANCODE_PERIOD = 0x49,
    SCANCODE_ENTER = 0x5A,
    SCANCODE_BACKSPACE = 0x66,
    SCANCODE_MISC = 0xE0
}scancode_t;

typedef enum charkey_t : int8_t{
    KEY_a = 'a',
    KEY_b,
    KEY_c,
    KEY_d,
    KEY_e,
    KEY_f,
    KEY_g,
    KEY_h,
    KEY_i,
    KEY_j,
    KEY_k,
    KEY_l,
    KEY_m,
    KEY_n,
    KEY_o,
    KEY_p,
    KEY_q,
    KEY_r,
    KEY_s,
    KEY_t,
    KEY_u,
    KEY_v,
    KEY_w,
    KEY_x,
    KEY_y,
    KEY_z,
    KEY_A = 'A',
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_0 = '0',
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_EXCL = '!',
    KEY_AT = '@',
    KEY_HASH = '#',
    KEY_DOLLAR = '$',
    KEY_PERCENT = '%',
    KEY_CARET = '^',
    KEY_AMPERSAND = '&',
    KEY_ASTERISK = '*',
    KEY_LBRACE = '(',
    KEY_RBRACE = ')',
    KEY_BACKSPACE = -127,
    KEY_DEL
}charkey_t;

typedef enum keyboard_flag_bit : uint8_t{
    KF_CAPSLOCK = (1<<0),
    KF_SHIFT = (1<<1),
    KF_MISC = (1<<6),
    KF_ON_RELEASE = (1<<7)
}keyboard_flag_bit;

typedef uint8_t keyboard_flags;

/*
       flags     charkey
    0000 0000 | 0000 0000
*/
typedef uint16_t keycode_t;

#define KC_IS_ON_RELEASE(kc) ((KF_ON_RELEASE << 8) & (kc))
#define KC_IS_CAPSLOCK(kc) ((KF_CAPSLOCK << 8) & (kc))
#define KC_IS_SHIFT(kc) ((KF_SHIFT << 8) & (kc))
#define KC_IS_MISC(kc) ((KF_MISC << 8) & kc)
#define KC_KEY(kc) (0xFF & (kc))

//set scancode set 2
int init_keyboard();
//should be called in ISR
void process_scancode();
//getchar() counterpart
keycode_t getkeycode();


#endif