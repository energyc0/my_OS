#ifndef __PS2_H
#define __PS2_H

#include <stdint.h>

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_REGISTER 0x64
#define PS2_COMMAND_REGISTER 0x64

int init_PS2_controller();
void clear_PS2_data_port();

int has_ps2_input();
uint8_t read_ps2_data();

#endif