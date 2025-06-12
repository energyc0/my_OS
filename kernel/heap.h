#ifndef __HEAP_H
#define __HEAP_H

#include "utils.h"

void heap_init();

void* malloc(size_t size);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t new_size);
void free(void* ptr);

void heap_debug();

#endif