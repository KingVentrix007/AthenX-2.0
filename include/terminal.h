#ifndef _TTY_H
#define _TTY_H 1

#include "../include/stdint.h"
#include "../include/stddef.h"
#include "../include/stdarg.h"

/*
 * Some macros to produce output on the screen.
 * Use those as far as possible
 */

/*
 * Simply print something to the screen
 */
#define PRINT(...) do {printf(__VA_ARGS__); } while (0)


// #define DEBUG(...) do {if (1 == 1 ) { printf("DEBUG at %s@%d (%s): ", __FILE__, __LINE__, __FUNCTION__); \
//                      printf(__VA_ARGS__); }} while (0)
// #define DEBUG(...) do {if (__loglevel > 0 ) { printf("DEBUG at %s@%d (%s): ", __FILE__, __LINE__, __FUNCTION__); \
//                      printf(__VA_ARGS__); }} while (0)

/*
 * Similar to PRINT, but output is enhanced by some
 * debugging information and message is preceeded by ERROR
 */
#define ERROR(...) do {printf("ERROR at %s@%d (%s): ", __FILE__, __LINE__, __FUNCTION__); \
                     printf(__VA_ARGS__); } while (0)

/*
 * Message preceeded by a functionally defined module name
 */
#define MSG(...) do {printf ("[%s] ", __module); printf(__VA_ARGS__); } while (0)

/*
 * As above, but after printing the message, CPU is halted
 */


void kprintf(char* template, ...);
#endif