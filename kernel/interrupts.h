#ifndef __INTERRUPTS_H
#define  __INTERRUPTS_H

#include <stdint.h>

typedef enum IRQ_t{
    IRQ_TIMER = 0x0,
    IRQ_KEYBOARD = 0x1,
    IRQ_CASCADE = 0x2,
    IRQ_COM2 = 0x3,
    IRQ_COM1 = 0x4,
    IRQ_LPT2= 0x5,
    IRQ_FLOPPY = 0x6,
    IRQ_SPURIOUS = 0x7,
    IRQ_CMOS_CLOCK = 0x8,
    IRQ_PERIPH0 = 0x9,
    IRQ_PERIPH1 = 0xA,
    IRQ_PERIPH2 = 0xB,
    IRQ_PS2_MOUSE = 0xC,
    IRQ_FPU = 0xD,
    IRQ_ATA_HDD1 = 0xE,
    IRQ_ATA_HDD2 = 0xF
}IRQ_t;

typedef enum gate_type_t{
    GATE_TYPE_TASK = 0x5,
    GATE_TYPE_INT16 = 0x6,
    GATE_TYPE_INT32 = 0xE,
    GATE_TYPE_TRAP16 = 0x7,
    GATE_TYPE_TRAP32 = 0xF,
}gate_type_t;

typedef enum DPL_t{
    DPL_KERNEL = 0,
    DPL_MID_HIGH = 1,
    DPL_MID_LOW = 2,
    DPL_USER = 3
}DPL_t;

typedef struct IDT_entry_t{
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t reserved;
    uint8_t type_attributes;
    uint16_t offset_high;
}__attribute__((packed))IDT_entry_t;

typedef struct IDT_ptr{
    uint16_t size;
    uint32_t offset;
}__attribute__((packed))IDT_ptr_t;

#define PIC_MASTER_CONTROL_PORT 0x20
#define PIC_MASTER_DATA_PORT 0x21
#define PIC_SLAVE_CONTROL_PORT 0xA0
#define PIC_SLAVE_DATA_PORT 0xA1

#define VECTOR_INTERRUPT_OFFSET 0x20

void init_IDT();
void configure_PIC();

void set_mask_IRQ(IRQ_t irq);
void disable_mask_IRQ(IRQ_t irq);
void mask_all_irq();
void send_EOI(IRQ_t irq);

void set_interrupts();
void disable_interrupts();

#endif