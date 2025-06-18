#include "timer.h"
#include "utils.h"
#include "io.h"
#include <stdint.h>

#define TIMER_FREQUENCY 1000

uint64_t tick_counter = 0;

void timer_set_frequency(int hz){
    int divisor = 1193180 / hz;
    // 00 - counter
    // 11 - read/write mode
    // 011 - mode
    // 0 - 16 bit counter
    outb(TIMER_COMMAND_REGISTER, 0x36);
    outb(TIMER_CHANNEL0_PORT, divisor & 0xFF);
    outb(TIMER_CHANNEL0_PORT, divisor >> 8);
}

void timer_on_interrupt(){
    ++tick_counter;
}

void timer_setup(){
    timer_set_frequency(TIMER_FREQUENCY);
}

uint64_t timer_get_ticks(){
    return tick_counter;
}

char* gettime(){
    static char buf[16];

    uint32_t total_seconds = tick_counter / TIMER_FREQUENCY;
    int hours = total_seconds/60/60;
    int minutes = total_seconds/60 - hours*60;
    int seconds = total_seconds%60;

    sprintf(buf, "%.2d:%.2d:%.2d", hours, minutes, seconds);
    return buf;
}