#ifndef __HEAP_H
#define __HEAP_H

#include "utils.h"

#define PRINT_POINTER(ptr) printf(#ptr" = %p\n", ptr)

void heap_init();
void* malloc(size_t size);
void free(void* ptr);

void heap_debug();

#endif