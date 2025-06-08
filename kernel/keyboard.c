#include "keyboard.h"
#include "ps2.h"
#include "terminal.h"
#include "utils.h"

#define KEYBOARD_ACK 0xFA
#define KEYBOARD_RESEND 0xFE

#define GETSET_SCANCODE (0xF0)
#define GET_SCANCODE (0x00)
#define SET_SCANCODE1 (0x01)
#define SET_SCANCODE2 (0x02)
#define SET_SCANCODE3 (0x03)

#define keyboard_success() (inb(KEYBOARD_DATA_PORT) == KEYBOARD_ACK)

#define SCANCODE_BUF_SZ 8

static struct keyboard_state{
    scancode_t scancode_buf[SCANCODE_BUF_SZ];
    int scancode_c;

    keyboard_flags flags;
}keyboard_state;

static void init_keyboard_state();
static void reinterpret_scancode();

static void push_scancode(scancode_t sc);
static scancode_t pop_scancode();
static int is_empty_scancode();

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
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    //printf("%X ", scancode);
    switch (scancode) {
        case ON_RELEASE: push_scancode(scancode); break;
        default: push_scancode(scancode); reinterpret_scancode(); break;
    }
}


static void push_scancode(scancode_t sc){
    if(keyboard_state.scancode_c >= SCANCODE_BUF_SZ)
        PANIC("Scancode buffer is full!");
    keyboard_state.scancode_buf[keyboard_state.scancode_c++] = sc;
}
static scancode_t pop_scancode(){
    if(keyboard_state.scancode_c <= 0)
        PANIC("Scancode buffer is empty!");
    return keyboard_state.scancode_buf[--keyboard_state.scancode_c];
}
static int is_empty_scancode(){
    return keyboard_state.scancode_c == 0;
}
/*
static void push_keycode(keycode_t kc){
    if(keyboard_state.keycode_c >= KEYCODE_BUF_SZ)
        PANIC("Keycode buffer is full!");
    keyboard_state.keycode_buf[keyboard_state.keycode_c++] = kc;
}
static keycode_t pop_keycode(){
    if(keyboard_state.keycode_c <= 0)
        PANIC("Keycode buffer is empty!");
    return keyboard_state.keycode_buf[--keyboard_state.keycode_c];
}
*/

static void init_keyboard_state(){
    memset(&keyboard_state, 0, sizeof(keyboard_state));
}

static void reinterpret_scancode(){
    if(is_empty_scancode())
        return;
    scancode_t scancode = pop_scancode();
    switch (scancode) {
        /*
        case KEY_A:case KEY_B:case KEY_C:case KEY_D:
        case KEY_E:case KEY_F:case KEY_G:case KEY_H:
        case KEY_I:case KEY_J:case KEY_K:case KEY_L:
        case KEY_M:case KEY_N:case KEY_O:case KEY_P:
        case KEY_Q:case KEY_R:case KEY_S:case KEY_T:
        case KEY_U:case KEY_V:case KEY_W:case KEY_X:
        case KEY_Y:case KEY_Z:case KEY_SPACE:
        case KEY_1:case KEY_2:case KEY_3:case KEY_4:case KEY_5:
        case KEY_6:case KEY_7:case KEY_8:case KEY_9:case KEY_0:
        {
            if (is_empty_scancode())
                send_symbol_to_terminal(scancode);
            break;
        }
            */
        case KEY_LSHIFT: case KEY_RSHIFT:{
            if(is_empty_scancode()){
                keyboard_state.flags |= KF_SHIFT;
            }else if(pop_scancode() == ON_RELEASE){
                keyboard_state.flags &= ~KF_SHIFT;
            }
            break;
        }

        case KEY_CAPSLOCK:{
            // if ON_RELEASE not found
            if (is_empty_scancode()) {
                keyboard_state.flags ^= KF_CAPSLOCK;
            }
            break;
        }
        default:{
            if (is_empty_scancode()) // if ON_RELEASE not found
                send_symbol_to_terminal(scancode);
            break;
        }
    }
    //clear buffer
    keyboard_state.scancode_c = 0;
}

char reinterpet_key_to_char(scancode_t sc){
    switch (sc) {
        case KEY_A: return 'a';
        case KEY_B: return 'b';
        case KEY_C: return 'c';
        case KEY_D: return 'd';
        case KEY_E: return 'e';
        case KEY_F: return 'f';
        case KEY_G: return 'g';
        case KEY_H: return 'h';
        case KEY_I: return 'i';
        case KEY_J: return 'j';
        case KEY_K: return 'k';
        case KEY_L: return 'l';
        case KEY_M: return 'm';
        case KEY_N: return 'n';
        case KEY_O: return 'o';
        case KEY_P: return 'p';
        case KEY_Q: return 'q';
        case KEY_R: return 'r';
        case KEY_S: return 's';
        case KEY_T: return 't';
        case KEY_U: return 'u';
        case KEY_V: return 'v';
        case KEY_W: return 'w';
        case KEY_X: return 'x';
        case KEY_Y: return 'y';
        case KEY_Z: return 'z';
        case KEY_1: return '1';
        case KEY_2: return '2';
        case KEY_3: return '3';
        case KEY_4: return '4';
        case KEY_5: return '5';
        case KEY_6: return '6';
        case KEY_7: return '7';
        case KEY_8: return '8';
        case KEY_9: return '9';
        case KEY_0: return '0';
        case KEY_SPACE: return ' ';
        case KEY_COMMA: return ',';
        case KEY_PERIOD: return '.';
        case KEY_ENTER: return '\n';
        default:return '\0';
    }
}

int is_shift_pressed(){
    return (keyboard_state.flags & KF_SHIFT) >> 1;
}
int is_capslock_enabled(){
    return keyboard_state.flags & KF_CAPSLOCK;
}