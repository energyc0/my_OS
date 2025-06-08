#include "keyboard.h"
#include "ps2.h"
#include "terminal.h"
#include "utils.h"
#include <stdint.h>

#define KEYBOARD_ACK 0xFA
#define KEYBOARD_RESEND 0xFE

#define GETSET_SCANCODE (0xF0)
#define GET_SCANCODE (0x00)
#define SET_SCANCODE1 (0x01)
#define SET_SCANCODE2 (0x02)
#define SET_SCANCODE3 (0x03)

#define keyboard_success() (inb(KEYBOARD_DATA_PORT) == KEYBOARD_ACK)

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
    return tries != 3;
}

void process_scancode(){
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    printf("%X ", scancode);
}