#ifndef __STDLIB__H
#define __STDLIB__H
#include "types.h"
#include "stddef.h"
uint32_t atoi(const char *str);
void* malloc(size_t size);
void free(void* ptr);
int exit(int code);
void abort(void);
char* getenv(const char* name);
void* calloc(size_t nmemb, size_t size);
int abs(int j);
int atexit(void (*function)(void));
#endif