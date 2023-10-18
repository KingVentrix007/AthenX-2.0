#include "types.h"
#include "../libc/string.h"
#include "../include/syscall.h"
#include <stdarg.h>


int syscall(int syscall_number, int param1, int param2) {
    int result;
    asm volatile (
        "int $42"
        : "=a" (result)
        : "a" (syscall_number), "b" (param1), "c" (param2)
    );
    return result;
}
