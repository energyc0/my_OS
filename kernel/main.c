#include "console.h"
#include "utils.h"
#include <stdint.h>

const uint8_t hello_str[] = "Hello, this is my OS!";

void kmain(){
    uint8_t buf[128];
    int32_t values[] = {123456, 9314, -125, 0, 21, 38583, 339, -35380, -9, -12};
    int32_t size = sizeof(values) / sizeof(values[0]);


    uint8_t s1[] = "abcdef";
    //uint8_t s2[] = "abcdefg";
    uint8_t s3[] = "Abcdef";
    uint8_t s4[] = "aacdef";
    uint8_t s5[] = "abcdea";
    println(hello_str);
    for(int32_t i = 0; i < size; i++){
        println(itoa(values[i], buf, 10));
    }

    println((const uint8_t*)"Comparing...");
    set_char_attrib(CA_FORE_GREEN|CA_FORE_BRIGHT);
    println(itoa(strcmp(s1, s1), buf, 10));
    println(itoa(strcmp(s1, s3), buf, 10));
    println(itoa(strcmp(s1, s4), buf, 10));
    println(itoa(strcmp(s1, s5), buf, 10));
    return;
}