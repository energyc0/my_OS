#include "heap.h"
#include "timer.h"
#include "utils.h"
#include "interrupts.h"
#include "keyboard.h"
#include "ps2.h"
#include "gdt.h"

extern char _stack_top_[];
extern char _stack_end_[];
void kmain(){
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
    
    return;
}