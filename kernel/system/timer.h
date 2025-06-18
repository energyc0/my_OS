#ifndef __TIMER_H
#define __TIMER_H

#define TIMER_CHANNEL0_PORT 0x40
#define TIMER_CHANNEL1_PORT 0x41
#define TIMER_CHANNEL2_PORT 0x42
#define TIMER_COMMAND_REGISTER 0x43

#include <stdint.h>

void timer_setup();
void timer_on_interrupt();
void timer_set_frequency(int hz);

uint64_t timer_get_ticks();
//return pointer to a static buffer containing string "hours:minutes:second"
char* gettime();
#endif