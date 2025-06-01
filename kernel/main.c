#include "console.h"

const uint8_t hello_str[] = "Hello, this is my OS!";

void _main(){
    print_string(hello_str);
    return;
}