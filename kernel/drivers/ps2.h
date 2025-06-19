#ifndef __PS2_H
#define __PS2_H

#include <stdint.h>

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_REGISTER 0x64
#define PS2_COMMAND_REGISTER 0x64

#define PS2_OUTPUT_BUFFER_STATUS(status) ((status) & (1 << 0))
#define PS2_INPUT_BUFFER_STATUS(status) ((status) & (1<<1))
#define PS2_SYSTEM_FLAG_STATUS(status) ((status) & (1<<2))
#define PS2_CMD_DATA_STATUS(status) ((status) & (1<<3))
#define PS2_TIMEOUT_ERR_STATUS(status) ((status) & (1<<6))
#define PS2_PARITY_ERR_STATUS(status) ((status) & (1<<7))

int init_PS2_controller();
void clear_PS2_data_port();

int has_ps2_input();
uint8_t read_ps2_data();

#endif