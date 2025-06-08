#include "ps2.h"
#include "utils.h"

#define PS2_CONTROLLER_TEST_SUCCESS 0x55

#define ENABLE_PS2_FIRST_PORT 0xAE
#define READ_CONTROLLER_CONFIGURATION_BYTE 0x20
#define WRITE_CONTROLLER_CONFIGURATION_BYTE 0x60
#define PS2_SELFTEST 0xAA

typedef enum ps2_status_bit_t{
    PS2_OUTPUT_BUFFER_BIT = 0x1,
    PS2_INPUT_BUFFER_BIT = 0x2,
    PS2_SYSTEM_FLAG = 0x4,
    PS2_COMMAND_DATA = 0x8
}ps2_status_bit_t;

static int test_PS2_controller();

int init_PS2_controller(){
    clear_PS2_data_port();
    //init PS/2 port
    outb(PS2_COMMAND_REGISTER, ENABLE_PS2_FIRST_PORT);
    io_wait();
    outb(PS2_COMMAND_REGISTER, READ_CONTROLLER_CONFIGURATION_BYTE);
    io_wait();
    /*3,4 and 7 bit are zero,
    disable PS/2 translation(for scancode set 2),
    enable first PS/2 clock port
    enable interrupts for PS/2*/
    uint8_t status = (inb(PS2_DATA_PORT) & (~0xD8)) | 0x1;

    outb(PS2_COMMAND_REGISTER, WRITE_CONTROLLER_CONFIGURATION_BYTE);
    io_wait();
    outb(PS2_DATA_PORT, status); //enable interrupts
    io_wait();
    
    return test_PS2_controller();
}

static int test_PS2_controller(){
    for(int32_t tries = 0; tries < 3; tries++){
        outb(PS2_COMMAND_REGISTER, PS2_SELFTEST);
        io_wait();
        if(inb(PS2_DATA_PORT) == PS2_CONTROLLER_TEST_SUCCESS)
            return 1;
        io_wait();
    }
    return 0;
}

void clear_PS2_data_port(){
    while (has_ps2_input())
        read_ps2_data();
}

int has_ps2_input(){
    return inb(PS2_STATUS_REGISTER) & PS2_OUTPUT_BUFFER_BIT;
}
uint8_t read_ps2_data(){
    return inb(PS2_DATA_PORT);
}