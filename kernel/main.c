#include "console.h"

const uint8_t hello_str[] = "Hello, this is my OS!\n\r";

void kmain(){
    for(int i = 0; i < 10; i++)
        print_string(hello_str);
    return;
}