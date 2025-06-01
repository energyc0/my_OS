ENTRY(_kernel_start_)

/*SEARCH_DIR(../build/kernel)*/

PHDRS{
    code PT_LOAD;
    data PT_LOAD;
    bss PT_LOAD;
    rodata PT_LOAD;
}

SECTIONS{
    . = 0x1000;
    .text : {
        ../build/kernel/kernel_start.elf
        *(.text)
    } : code
    .data : { *(.data) } : data
    .rodata : { *(.rodata) } : rodata
    .bss : {
         *(.bss)
        . = ALIGN(16);
        _stack_end_ = .;
        . += 16 * 1024; /* 16 Kb stack */
        _stack_top_ = .;
    } : bss
}