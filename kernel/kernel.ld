ENTRY(_kernel_start_)

PHDRS{
    code PT_LOAD;
    data PT_LOAD;
    bss PT_LOAD;
    rodata PT_LOAD;
    heap PT_LOAD;
}

SECTIONS{
    . = 0x1000;
    .text : {
        ../build/kernel/kernel_start.elf
        *(.text)
        *(.text.*)
    } : code
    
    .data : { 
        *(.data)
        *(.data*)
    } : data

    .rodata : {
         *(.rodata .rodata.*) 
    } : rodata

    .bss : {
        *(.bss .bss.*)


    } : bss


    . = ALIGN(16);
    _stack_end_ = .;
    . += 0x10000; /* 64 Kb stack */
    _stack_top_ = .;

    . = 0x1000000;
    _heap_start_ = .;
    . += 0x1000000; /* 16 Mb heap */
    _heap_end_ = .;
        
    /DISCARD/ :{
        *(.eh_frame)
    }
}