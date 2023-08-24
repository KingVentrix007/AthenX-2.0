#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG(...) do {if (__loglevel > 0 ) { kprintf("DEBUG at %s@%d (%s): ", __FILE__, __LINE__, __FUNCTION__); \
                     kprintf(__VA_ARGS__); }} while (0)


int screen_of_death();

#endif