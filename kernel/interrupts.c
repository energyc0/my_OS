#include "interrupts.h"
#include "utils.h"
#include "isr.h"
#include <stdint.h>

#define CODE_SEG_OFFSET 0x8

__attribute__((aligned(0x10)))static IDT_entry_t idt_entries[256];
static IDT_ptr_t idt_ptr;

static void init_IDT_entry(IDT_entry_t* entry, uint32_t offset, uint16_t seg_sel, gate_type_t gate_t, DPL_t dpl);
static void LIDT();
static void set_int_handler(IRQ_t irq, void (*handler)());

static void init_IDT_entry(IDT_entry_t* entry, uint32_t offset, uint16_t seg_sel, gate_type_t gate_t, DPL_t dpl){
    entry->type_attributes = 0x80 | (dpl << 5) | gate_t ;
    entry->reserved = 0;
    entry->segment_selector = seg_sel;
    entry->offset_low = offset & 0xFFFF;
    entry->offset_high = (offset >> 16) & 0xFFFF;
}

static void LIDT(){
    asm volatile("lidt %0" : :  "m"(idt_ptr));
}   

static void set_int_handler(IRQ_t irq, void (*handler)()){
    init_IDT_entry(&idt_entries[0x20 + irq], (uint32_t)handler, CODE_SEG_OFFSET, GATE_TYPE_INT32, DPL_KERNEL);
}

void init_IDT(){
    idt_ptr.offset = (uint32_t)idt_entries;
    idt_ptr.size = sizeof(idt_entries) - 1;

    set_int_handler(IRQ_TIMER, isr_0);
    set_int_handler(IRQ_KEYBOARD, isr_1);
    set_int_handler(IRQ_CASCADE, isr_2);
    set_int_handler(IRQ_COM2, isr_3);
    set_int_handler(IRQ_COM1, isr_4);
    set_int_handler(IRQ_LPT2, isr_5);
    set_int_handler(IRQ_FLOPPY, isr_6);
    set_int_handler(IRQ_SPURIOUS, isr_7);
    set_int_handler(IRQ_CMOS_CLOCK, isr_8);
    set_int_handler(IRQ_PERIPH0, isr_9);
    set_int_handler(IRQ_PERIPH1, isr_10);
    set_int_handler(IRQ_PERIPH2, isr_11);
    set_int_handler(IRQ_PS2_MOUSE, isr_12);
    set_int_handler(IRQ_FPU, isr_13);
    set_int_handler(IRQ_ATA_HDD1, isr_14);
    set_int_handler(IRQ_ATA_HDD2, isr_15);

    LIDT();
}

void configure_PIC(){
    //start to reconfigure PIC
    outb(PIC_MASTER_CONTROL_PORT, 0x11);
    io_wait();
    outb(PIC_SLAVE_CONTROL_PORT, 0x11);
    io_wait();

    //set interrupt vector
    outb(PIC_MASTER_DATA_PORT, VECTOR_INTERRUPT_OFFSET);
    io_wait();
    outb(PIC_SLAVE_DATA_PORT, VECTOR_INTERRUPT_OFFSET + 8);
    io_wait();

    //set cascade mode
    outb(PIC_MASTER_DATA_PORT, 0x04);
    io_wait();
    outb(PIC_SLAVE_DATA_PORT, 0x02);
    io_wait();

    //set i8086 mode
    outb(PIC_MASTER_DATA_PORT, 0x01);
    io_wait();
    outb(PIC_SLAVE_DATA_PORT, 0x01);
    io_wait();
}

void set_mask_IRQ(IRQ_t irq){
    uint16_t port;
    uint8_t data;
    if (irq > 7) {
        port = PIC_SLAVE_DATA_PORT;
        data = irq - 8;
    }else{
        port = PIC_MASTER_DATA_PORT;
        data = irq;
    }
    data = inb(port) | (1 << data);
    outb(port, data);
}

void disable_mask_IRQ(IRQ_t irq){
    uint16_t port;
    uint8_t data;
    if (irq > 7) {
        port = PIC_SLAVE_DATA_PORT;
        data = irq - 8;
    }else{
        port = PIC_MASTER_DATA_PORT;
        data = irq;
    }
    data = inb(port) & ~(1 << data);
    outb(port, data);
}

void mask_all_irq(){
    outb(PIC_MASTER_DATA_PORT, 0xff);
    outb(PIC_SLAVE_DATA_PORT, 0xff);
}

