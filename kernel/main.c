#include "heap.h"
#include "timer.h"
#include "utils.h"
#include "interrupts.h"
#include "keyboard.h"
#include "ps2.h"
#include "gdt.h"

void print_array(int* arr, size_t len){
    printf("Array at %p: ", arr);
    for(size_t i = 0; i < len; i++)
        printf("%d ", arr[i]);
    putchar('\n');
}

void kmain(){
    GDT_init();
    heap_init();
/*
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

    const int sz = 10;
    int* ptr1 = malloc(sz*sizeof(int));
    
    for(int i = 0; i < sz; i++)
        ptr1[i] = i;
    heap_debug();
    print_array(ptr1, sz);

    realloc(ptr1, (sz+2)*sizeof(int));
    printf("After realloc()\n");
    heap_debug();
    print_array(ptr1, sz+2);
    */
    printf("Hello!");
    return;
}