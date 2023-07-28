#ifndef _TTY_H
#define _TTY_H 1

#include "stdint.h"
#include "stddef.h"
#include "stdarg.h"

/*
 * Some macros to produce output on the screen.
 * Use those as far as possible
 */

/*
 * Simply print something to the screen
 */
#define PRINT(...) do {kprintf(__VA_ARGS__); } while (0)

/*
 * Similar to PRINT, but output is enhanced by some
 * debugging information
 */
#define DEBUG(...) do {if (__loglevel > 0 ) { kprintf("DEBUG at %s@%d (%s): ", __FILE__, __LINE__, __FUNCTION__); \
                     kprintf(__VA_ARGS__); }} while (0)

/*
 * Similar to PRINT, but output is enhanced by some
 * debugging information and message is preceeded by ERROR
 */
#define ERROR(...) do {kprintf("ERROR at %s@%d (%s): ", __FILE__, __LINE__, __FUNCTION__); \
                     kprintf(__VA_ARGS__); } while (0)

/*
 * Message preceeded by a functionally defined module name
 */
#define MSG(...) do {kprintf ("[%s] ", __module); kprintf(__VA_ARGS__); } while (0)

/*
 * As above, but after printing the message, CPU is halted
 */


void kprintf(char* template, ...);
#endif