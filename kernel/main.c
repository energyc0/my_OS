#include "heap.h"
#include "timer.h"
#include "utils.h"
#include "interrupts.h"
#include "keyboard.h"
#include "ps2.h"

extern char _heap_start_[];
extern char _heap_end_[];

void kmain(){
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

    char* ptr1 = malloc(1024);
    char* ptr2 = malloc(1024);
    char* ptr3 = malloc(512);
    char* ptr4 = malloc(16);
    char* ptr5 = malloc(32);
    heap_debug();

    printf("After free()\n");
    free(ptr1);
    
    free(ptr3);
    heap_debug();
    char* ptr = malloc(512);
    printf("After malloc()\n");
    heap_debug();
    return;
}