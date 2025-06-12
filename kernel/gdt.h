#ifndef __GDT_H
#define __GDT_H

#include <stdint.h>

typedef enum DPL_t{
    DPL_KERNEL = 0,
    DPL_MID_HIGH = 1,
    DPL_MID_LOW = 2,
    DPL_USER = 3
}DPL_t;

#define DPL_BITS 0b01100000

typedef enum gdt_access_byte_t : uint8_t{
    AB_ACCESSED_BIT = 0b00000001,
    AB_RW_BIT = 0b00000010,
    AB_DIRECTION_CONFORMING_BIT = 0b00000100,
    AB_EXECUTABLE_BIT = 0b00001000,
    AB_DESCRIPTOR_TYPE_BIT = 0b00010000,
    AB_PRESENT_BIT = 0b10000000
} gdt_access_byte_t;

typedef enum gdt_flags_t : uint8_t{
    F_LONGMODE = 0b0010,
    F_SIZE = 0b0100,
    F_GRANULARITY = 0b1000
}gdt_flags_t;

typedef struct GDT_entry_t{
    uint16_t limit_low_16bits;
    uint16_t base_low_16bits;
    uint8_t base_high_low_8bits;
    gdt_access_byte_t access_byte;
    uint8_t flags_and_limit;
    uint8_t base_high_high_8bits;
}__attribute__((packed)) GDT_entry_t;

typedef struct GDT_table_t{
    GDT_entry_t null_descriptor;
    GDT_entry_t code_descriptor;
    GDT_entry_t data_descriptor;
}__attribute__((packed)) GDT_table_t;

#define CODE_SEGMENT offsetof(struct GDT_table_t, code_descriptor)
#define DATA_SEGMENT offsetof(struct GDT_table_t, data_descriptor)

void GDT_init();

#endif