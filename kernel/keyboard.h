#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60

//set scancode set 2
int init_keyboard();
//should be called in ISR
void process_scancode();

#endif