#include "gdt.h"
#include <stdint.h>
#include <stddef.h>

static GDT_table_t GDT_table;

static struct GDT_descriptor{
    uint16_t size;
    uint32_t offset;
}__attribute__((packed))GDT_descriptor;

static void init_GDT_entry(GDT_entry_t* p, uint32_t base, uint32_t limit, gdt_flags_t flags, gdt_access_byte_t access_byte, DPL_t dpl);

void GDT_init(){
    GDT_descriptor.offset = (uint32_t)&GDT_table;
    GDT_descriptor.size = sizeof(GDT_table_t)-1;

    init_GDT_entry(&GDT_table.null_descriptor, 0, 0, 0, 0, 0);

    init_GDT_entry(&GDT_table.code_descriptor, 0, 0xFFFFFF,
    F_SIZE | F_GRANULARITY,
    AB_PRESENT_BIT|AB_DESCRIPTOR_TYPE_BIT|AB_EXECUTABLE_BIT|AB_DIRECTION_CONFORMING_BIT|AB_RW_BIT,
    DPL_KERNEL);

    init_GDT_entry(&GDT_table.data_descriptor, 0, 0xFFFFFF,
    F_SIZE|F_GRANULARITY,
    AB_PRESENT_BIT|AB_DESCRIPTOR_TYPE_BIT|AB_RW_BIT,
    DPL_KERNEL);

    asm volatile(
    "lgdt %0\n"
    "jmp %1:start_protected_mode\n"
    "start_protected_mode:\n"
    //"mov ax, %2\n"
    "mov ds, %2\n"
    "mov es, %2\n"
    "mov ss, %2\n"
    "mov fs, %2\n"
    "mov gs, %2\n"
    : : "m"(GDT_descriptor), "n"(CODE_SEGMENT), "r"(DATA_SEGMENT));
}

static void init_GDT_entry(GDT_entry_t* p, uint32_t base, uint32_t limit, gdt_flags_t flags, gdt_access_byte_t access_byte, DPL_t dpl){
    p->base_low_16bits = base & 0xFFFF;
    p->limit_low_16bits = limit & 0xFFFF;
    p->base_high_low_8bits = (base >> 16) & 0xFF;
    p->access_byte = access_byte | ((dpl & 0b11) << 5);
    p->flags_and_limit = (flags << 4) | ((limit >> 16) & 0xF);
    p->base_high_high_8bits = base >> 24;
}