#ifndef LIB_STDIO_H
#define LIB_STDIO_H

// Include necessary standard headers
#include <stdarg.h>
#include "stddef.h"
#include "stdint.h"
// Declare the custom printf_lib function
int printf(const char* format, ...);
char *get_char();
int puts(char *string);
int vsnprintf(char* buffer, size_t size, const char* format, va_list ap) ;
int  test_ret(int in);
int reverse_int(int value);
// Add other declarations for custom functions or types if needed

#endif /* LIB_STDIO_H */
