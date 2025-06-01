ENTRY(_kernel_start_)

SECTIONS{
    . = 0x1000;
    .text : { *(.text) }
    .bss : { *(.bss) }
    .data : { *(.data) }
    .rodata : { *(.rodata) }

}