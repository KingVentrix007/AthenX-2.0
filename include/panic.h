#ifndef _PANIC_H
#define _PANIC_H 1

#include "../include/terminal.h"
#include "../include/display.h"
#define ERROR(...) do {display_init(1,0,0,0);printf("ERROR at %s@%d (%s): ", __FILE__, __LINE__, __FUNCTION__); \
                     printf(__VA_ARGS__); } while (0)



#endif