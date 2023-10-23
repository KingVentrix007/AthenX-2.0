#ifndef __STDLIB__H
#define __STDLIB__H
#include "../include/types.h"
#include "stddef.h"
uint32_t atoi(const char *str);
void* malloc(size_t size);
void free(void* ptr);

#endif