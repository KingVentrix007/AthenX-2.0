#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
#define NULL 0

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef signed char sint8;
typedef signed short sint16;
typedef signed int sint32;
typedef unsigned long long uint64;
typedef unsigned long phys_bytes;	/* physical addr/length in bytes */
// typedef unsigned __int128 __uint128_t;

typedef uint8 byte;
typedef uint16 word;
typedef uint32 dword;

typedef enum {
    FALSE,
    TRUE
} BOOL;

#endif

