#ifndef __HEAP_H
#define __HEAP_H

#include "utils.h"

void heap_init();

void* malloc(size_t size);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t new_size);
void free(void* ptr);

void heap_debug();

void heap_basic_test();
void heap_multiple_test();
void heap_calloc_test();
void heap_realloc_test();
void heap_edge_cases_test();

#endif