#include "isr.h"
#include "timer.h"
#include "interrupts.h"
#include "keyboard.h"
#include "utils.h"
#include <stdint.h>

static uint32_t interrupt_num;

__attribute__((naked))void isr_0(){
    asm volatile(
        "cli\n"
        "mov %0, 0\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_1(){
    asm volatile(
        "cli\n"
        "mov %0, 1\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_2(){
    asm volatile(
        "cli\n"
        "mov %0, 2\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_3(){
    asm volatile(
        "cli\n"
        "mov %0, 3\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_4(){
    asm volatile(
        "cli\n"
        "mov %0, 4\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_5(){
    asm volatile(
        "cli\n"
        "mov %0, 5\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_6(){
    asm volatile(
        "cli\n"
        "mov %0, 6\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_7(){
    asm volatile(
        "cli\n"
        "mov %0, 7\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_8(){
    asm volatile(
        "cli\n"
        "mov %0, 8\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_9(){
    asm volatile(
        "cli\n"
        "mov %0, 9\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_10(){
    asm volatile(
        "cli\n"
        "mov %0, 10\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_11(){
    asm volatile(
        "cli\n"
        "mov %0, 11\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_12(){
    asm volatile(
        "cli\n"
        "mov %0, 12\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_13(){
    asm volatile(
        "cli\n"
        "mov %0, 13\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_14(){
    asm volatile(
        "cli\n"
        "mov %0, 14\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}
__attribute__((naked))void isr_15(){
    asm volatile(
        "cli\n"
        "mov %0, 15\n"
        "jmp common_isr_stub_handler" : "=m"(interrupt_num));
}

void isr_handler_0(){
    timer_on_interrupt();
    send_EOI(IRQ_TIMER);
}
void isr_handler_1(){
    process_scancode();
    send_EOI(IRQ_KEYBOARD);
}
void isr_handler_2(){
    println("isr_handler_2 called");
}
void isr_handler_3(){
    println("isr_handler_3 called");
}
void isr_handler_4(){
    println("isr_handler_4 called");
}
void isr_handler_5(){
    println("isr_handler_5 called");
}
void isr_handler_6(){
    println("isr_handler_6 called");
}
void isr_handler_7(){
    println("isr_handler_7 called");
}
void isr_handler_8(){
    println("isr_handler_8 called");
}
void isr_handler_9(){
    println("isr_handler_9 called");
}
void isr_handler_10(){
    println("isr_handler_10 called");
}
void isr_handler_11(){
    println("isr_handler_11 called");
}
void isr_handler_12(){
    println("isr_handler_12 called");
}
void isr_handler_13(){
    println("isr_handler_13 called");
}
void isr_handler_14(){
    println("isr_handler_14 called");
}
void isr_handler_15(){
    println("isr_handler_15 called");
}

__attribute__((naked))void common_isr_stub_handler(){
asm volatile(
        "pusha\n"
        "push ds\n"
        "push es\n"
        "push fs\n"
        "push gs\n"
        "push esp\n"

        "push %0\n"
        "call common_isr_stub_switcher\n"
        "add esp, 0x4\n"

        "pop esp\n"
        "pop gs\n"
        "pop fs\n"
        "pop es\n"
        "pop ds\n"
        "popa\n"
        "sti\n"

        "iret" : : "m"(interrupt_num));
}

void common_isr_stub_switcher(uint32_t int_num){
    switch(int_num){
        case 0: isr_handler_0(); break;
        case 1: isr_handler_1(); break;
        case 2: isr_handler_2(); break;
        case 3: isr_handler_3(); break;
        case 4: isr_handler_4(); break;
        case 5: isr_handler_5(); break;
        case 6: isr_handler_6(); break;
        case 7: isr_handler_7(); break;
        case 8: isr_handler_8(); break;
        case 9: isr_handler_9(); break;
        case 10: isr_handler_10(); break;
        case 11: isr_handler_11(); break;
        case 12: isr_handler_12(); break;
        case 13: isr_handler_13(); break;
        case 14: isr_handler_14(); break;
        case 15: isr_handler_15(); break;
        default: printf("Unhandled interrupt: %X\n", int_num);
    }

}

void send_EOI(IRQ_t irq){
    if (irq > 7) {
        outb(PIC_SLAVE_CONTROL_PORT, 0x20);
    }
    outb(PIC_MASTER_CONTROL_PORT, 0x20);
}