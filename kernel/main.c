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
   
    /*
    char str1[]= "To be or not to be";
    char str2[40];
    char str3[40];


    strncpy ( str2, str1, sizeof(str2) );


    strncpy ( str3, str2, 5 );
    str3[5] = '\0'; 
    printf("%s\n",str1);
    printf ("%s\n",str2);
    printf ("%s\n",str3);
    */
    
    /*
    int a = 100;
    int b = 2;
    int n;
    printf("Hello, my friend!%n\n", &n);
    printf("%d\n", n);
    printf("bob: %d %n+ %d = %d\n", a,&n, b, a+b);
    printf("%d\n", n);
    printf("1) %X\n2)%n %x\n3) %o\n4) %O\n", 0xABCDEF,&n, 0x23A, 072221, 01234);
    printf("%d\n", n);
    printf("char: %c\n%n", 'X',&n);
    printf("%d\n", n);
    printf("string: %s\n%n", "my string", &n);
    printf("%d\n", n);
    printf("_bss_start_ = %p\n%n", _bss_start_, &n);
    printf("%d\n", n);
    
    printf("%12.34%  %% %%%%");
    putchar('X');
    */

    char destination[50];
    sprintf(destination, "Hello %s!", "World");
    printf("%s", destination);

    return;
}