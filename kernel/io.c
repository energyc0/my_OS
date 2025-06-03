#include "io.h"

uint8_t inb(uint16_t port){
    uint8_t ret;
    asm volatile ("inb %0, %1" 
        : "=a"(ret) 
        : "Nd"(port) : "memory");
    return ret;
}
void outb(uint16_t port, uint8_t byte){
    asm volatile ("outb %0, %1" 
        : : "Nd"(port), "a"(byte) : "memory");
}