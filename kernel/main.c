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
    move_cursor(0, 24);
    //printf("%X", *(uint16_t*)0xb8000);
    //move_cursor(0, 1);
    //putchar('A');
    //move_cursor(0, 0);
   ///printf("%X", *(uint16_t*)0xb8000);
   /*
    for(int x = 0; x < 10; x++){
        //get_cursor(&x, &y);
        printf("x = %d, %X\n", x, ((uint16_t*)0xb8000)[x]);
        //printf("x = %d", x);
    }
        */
    /*putchar('A');
    sleep(1000);
    backward_cursor();
    update_cursor();
    sleep(1000);
    erasechar();
    */
    /*
    int counter = 1;
    move_cursor(0, 23);
    while (1) {
        sleep(1000);
        printf("%d\n", counter++);
    }*/

    return;
}