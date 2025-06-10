#include "terminal.h"
#include "timer.h"
#include "utils.h"
#include "interrupts.h"
#include "keyboard.h"
#include "ps2.h"

void kmain(){
    if(!init_PS2_controller())
        PANIC("Failed to initialize PS/2 controller!");
    if(!init_keyboard())
        PANIC("Failed to initialize keyboard!");

    timer_setup();

    init_IDT();
    configure_PIC();

    mask_all_irq();
    set_interrupts();
    disable_mask_IRQ(IRQ_TIMER);
    disable_mask_IRQ(IRQ_KEYBOARD);
    
    int counter = 1;
    while (1) {
        sleep(100);
        printf("%d ", counter++);
    }

    return;
}