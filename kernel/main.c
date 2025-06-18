#include "cline.h"
#include "heap.h"
#include "io.h"
#include "timer.h"
#include "keyboard.h"
#include "ps2.h"
#include "gdt.h"

extern char _bss_start_[];
extern char _bss_end_[];

void kmain(){
    memset(_bss_start_, 0, _bss_end_ - _bss_start_);
    GDT_init();
    heap_init();

    if(!init_PS2_controller())
        PANIC("Failed to initialize PS/2 controller!");
    if(!init_keyboard())
        PANIC("Failed to initialize keyboard!");
    timer_setup();
    init_IDT();
    configure_PIC();

    mask_all_IRQ();
    disable_mask_IRQ(IRQ_TIMER);
    disable_mask_IRQ(IRQ_KEYBOARD); 
    set_interrupts();
    
    //launch_command_line();
    int n;
    const char s[] = "Hola!";
    printf("%11.6d\n", 11);
    printf("%d\n", 11);
    return;
}