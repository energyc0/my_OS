#include "console.h"
#include "utils.h"
#include <stdint.h>

const char hello_str[] = "Hello, this is my OS!";

void kmain(){
    int32_t values[] = {123456, 9314, -125, 0, 21, 38583, 339, -35380, -9, -12};
    int32_t size = sizeof(values) / sizeof(values[0]);


    char s1[] = "abcdef";
    char s2[] = "abcdefg";
    char s3[] = "Abcdef";
    char s4[] = "aacdef";
    println(hello_str);
    for(int32_t i = 0; i < size; i++){
        printf("%d) %d\n\r", i+1, values[i]);
    }
    clear_screen();
    println("Comparing...");
    set_char_attrib(CA_FORE_GREEN|CA_FORE_BRIGHT);
    printf("strcmp(\"%s\",\"%s\") = %d\n\r", s1,s1, strcmp(s1, s1));
    printf("strcmp(\"%s\",\"%s\") = %d\n\r", s1,s2, strcmp(s1, s2));
    printf("strcmp(\"%s\",\"%s\") = %d\n\r", s1,s3, strcmp(s1, s3));
    printf("strcmp(\"%s\",\"%s\") = %d\n\r", s1,s4, strcmp(s1, s4));
    return;
}