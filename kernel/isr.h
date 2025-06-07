#ifndef __ISR_H
#define __ISR_H
#include <stdint.h>

void isr_0();
void isr_1();
void isr_2();
void isr_3();
void isr_4();
void isr_5();
void isr_6();
void isr_7();
void isr_8();
void isr_9();
void isr_10();
void isr_11();
void isr_12();
void isr_13();
void isr_14();
void isr_15();

void common_isr_stub_handler();
void common_isr_stub_switcher(uint32_t int_num);

void isr_handler_0();
void isr_handler_1();
void isr_handler_2();
void isr_handler_3();
void isr_handler_4();
void isr_handler_5();
void isr_handler_6();
void isr_handler_7();
void isr_handler_8();
void isr_handler_9();
void isr_handler_10();
void isr_handler_11();
void isr_handler_12();
void isr_handler_13();
void isr_handler_14();
void isr_handler_15();

#endif