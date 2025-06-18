#include "keyboard.h"
#include "io.h"
#include "ps2.h"
#include "utils.h"

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
static keycode_t reinterpet_scan_to_key(scancode_t scancode);

//return value in the 0'st bit
static inline int is_shift_pressed();
static inline int is_capslock_enabled();

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
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    switch (scancode) {
        case ON_RELEASE: push_scancode(scancode); break;
        default: push_scancode(scancode); reinterpret_scancode(); break;
    }
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
    if(is_empty_scancode())
        return;
    scancode_t scancode = pop_scancode();
    switch (scancode) {
        case SCANCODE_LSHIFT: case SCANCODE_RSHIFT:{
            if(is_empty_scancode()){
                keyboard_state.flags |= KF_SHIFT;
            }else if(pop_scancode() == ON_RELEASE){
                keyboard_state.flags &= ~KF_SHIFT;
            }
            break;
        }

        case SCANCODE_CAPSLOCK:{
            // if ON_RELEASE not found
            if (is_empty_scancode()) {
                keyboard_state.flags ^= KF_CAPSLOCK;
            }
            break;
        }
        default:{
            if (is_empty_scancode()){ // if ON_RELEASE not found
                //then it is a single key
                keycode_t kc = reinterpet_scan_to_key(scancode);
                push_keycode(kc);
                io_process_keycode();
            }
            break;
        }
    }
    //clear buffer
    keyboard_state.scancode_c = 0;
}

static keycode_t reinterpet_scan_to_key(scancode_t scancode){
    switch (scancode) {
        case SCANCODE_A: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'a' : 'A';
        case SCANCODE_B: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'b' : 'B';
        case SCANCODE_C: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'c' : 'C';
        case SCANCODE_D: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'd' : 'D';
        case SCANCODE_E: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'e' : 'E';
        case SCANCODE_F: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'f' : 'F';
        case SCANCODE_G: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'g' : 'G';
        case SCANCODE_H: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'h' : 'H';
        case SCANCODE_I: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'i' : 'I';
        case SCANCODE_J: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'j' : 'J';
        case SCANCODE_K: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'k' : 'K';
        case SCANCODE_L: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'l' : 'L';
        case SCANCODE_M: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'm' : 'M';
        case SCANCODE_N: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'n' : 'N';
        case SCANCODE_O: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'o' : 'O';
        case SCANCODE_P: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'p' : 'P';
        case SCANCODE_Q: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'q' : 'Q';
        case SCANCODE_R: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'r' : 'R';
        case SCANCODE_S: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  's' : 'S';
        case SCANCODE_T: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  't' : 'T';
        case SCANCODE_U: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'u' : 'U';
        case SCANCODE_V: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'v' : 'V';
        case SCANCODE_W: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'w' : 'W';
        case SCANCODE_X: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'x' : 'X';
        case SCANCODE_Y: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'y' : 'Y';
        case SCANCODE_Z: return !(is_shift_pressed() ^ is_capslock_enabled()) ?  'z' : 'Z';
        case SCANCODE_1: return !is_shift_pressed() ? '1' : '!';
        case SCANCODE_2: return !is_shift_pressed() ? '2' : '@';
        case SCANCODE_3: return !is_shift_pressed() ? '3' : '#';
        case SCANCODE_4: return !is_shift_pressed() ? '4' : '$';
        case SCANCODE_5: return !is_shift_pressed() ? '5' : '%';
        case SCANCODE_6: return !is_shift_pressed() ? '6' : '^';
        case SCANCODE_7: return !is_shift_pressed() ? '7' : '&';
        case SCANCODE_8: return !is_shift_pressed() ? '8' : '*';
        case SCANCODE_9: return !is_shift_pressed() ? '9' : '(';
        case SCANCODE_0: return !is_shift_pressed() ? '0' : ')';
        case SCANCODE_SPACE: return ' ';
        case SCANCODE_COMMA: return !is_shift_pressed() ? ',' : '<';
        case SCANCODE_PERIOD: return !is_shift_pressed() ? '.' : '>';
        case SCANCODE_ENTER: return '\n';
        default:return '\0';
    }
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