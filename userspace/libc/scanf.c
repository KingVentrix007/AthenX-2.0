#include "scanf.h"
#include "../../include/syscall.h"
#include "printf.h"


int scanf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = vscanf(format, args);
    va_end(args);
    return result;
}
int vscanf(const char* format, va_list args) {
    
}