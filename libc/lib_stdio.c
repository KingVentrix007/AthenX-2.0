
#include "../include/syscall.h" // Include your kernel headers
#include "lib_stdio.h"
// Simplified function to print a string using kernel's printf
#include <stdarg.h>
#include <stddef.h>
#include "string.h"

extern volatile uint32_t *syscall_ret;

uint32_t reverse_uint32_user(uint32_t value) {
    return ((value >> 24) & 0xFF) | ((value >> 8) & 0xFF00) | ((value << 8) & 0xFF0000) | ((value << 24) & 0xFF000000);
}

int reverse_int(int value) {
    int reversed = 0;
    while (value != 0) {
        int digit = value % 10;
        reversed = reversed * 10 + digit;
        value /= 10;
    }
    return reversed;
}
char *get_char()
{
    char *chr;
    
    int res = system_call(0x82,0,0);
    printf("res->%d",(res));
    char str[2]; // Create a string of length 2 (1 for the character and 1 for the null terminator).
    str[0] = (char)res;
    str[1] = '\0';
    if(res == 'C' || res == 'c')
    {
        printf("\nWE GOT C\n");
    }
    //printf("%s",chr);
    return res;
}
int pop_value() {
    int result;
    asm volatile (
        "pop %[result]"   // Pop the value from the stack and store it in 'result'
        : [result] "=r" (result)
    );
    return result;
}
int  test_ret(int in)
{
    int pls_work;
    
    int  r =  pop_value(); // if(r == 10)
    // {
    //     r = 78;
    // }
    printf("ret->%d\n",r);
    return pls_work;
}

int printf(const char* format, ...) {
    char buffer[1000];
    memset(buffer, 0, sizeof(buffer));
    va_list args;
    va_start(args, format);

    // Call your custom my_vsnprintf function to format the text and store it in the buffer.
    int chars_written = vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    // Check for buffer overflow.
    if (chars_written < 0) {
        // Handle the error (e.g., return an error code).
        return -1;
    }

    // Print the formatted text to the standard output (you can replace this with your own output mechanism).
   
    system_call(0x81,buffer,0);
    // for (int i = 0; i < chars_written; i++) {
    //     system_call(0x81,buffer);
    // }
    return chars_written;
}

int addToBuffer(char* buffer, const char* format, ...) {
    va_list args; // Define a variable argument list.
    va_start(args, format); // Initialize the argument list.

    // Use vsnprintf to add formatted text to the buffer.
    // It returns the number of characters added or -1 on error.
    int result = 0;//vsnprintf(buffer, /* buffer size */ 100, format, args);

    va_end(args); // Clean up the argument list.

    return result;
}

int vsnprintf(char* buffer, size_t size, const char* format, va_list ap) {
    int chars_written = 0; // Initialize the character count.

    // Loop through the format string.
    while (*format != '\0') {
        if (*format == '%' && *(format + 1) == 's') {
            // Handle the '%s' format specifier (string).
            const char* str = va_arg(ap, const char*);

            // Copy the string to the buffer.
            while (*str != '\0' && chars_written < (int)size - 1) {
                buffer[chars_written++] = *str++;
            }
            
            format += 2; // Move format pointer past '%s'.
        } else if (*format == '%' && *(format + 1) == 'd') {
            // Handle the '%d' format specifier (integer).
            int num = va_arg(ap, int);
            char num_str[12]; // Assuming a maximum of 12 digits for an int.
            int num_chars = 0;

            // Handle negative numbers.
            if (num < 0) {
                buffer[chars_written++] = '-';
                num = -num;
            } else if (num == 0) {
                num_str[num_chars++] = '0'; // Handle special case for 0.
            }

            // Convert the number to a string in reverse order.
            while (num > 0) {
                int digit = num % 10;
                num_str[num_chars++] = '0' + digit;
                num /= 10;
            }

            // Copy the integer string to the buffer.
            while (num_chars > 0 && chars_written < (int)size - 1) {
                buffer[chars_written++] = num_str[--num_chars];
            }
            
            format += 2; // Move format pointer past '%d'.
        } else {
            // Copy regular characters to the buffer.
            if (chars_written < (int)size - 1) {
                buffer[chars_written++] = *format;
            } else {
                // Buffer full, terminate the loop.
                break;
            }
            format++;
        }
    }

    // Null-terminate the buffer.
    if (chars_written < (int)size) {
        buffer[chars_written] = '\0';
    } else {
        // Ensure the buffer is null-terminated in case of overflow.
        buffer[size - 1] = '\0';
    }

    return chars_written;
}

int puts(char *string)
{
    system_call(0x81,string,0);
}


void prepare_args(uint32_t* args, const char* format, ...) {
    va_list arg_list;
    va_start(arg_list, format);

    int i = 0;
    while (*format) {
        if (*format == 'd') {
            args[i] = va_arg(arg_list, int);
            i++;
        } else if (*format == 's') {
            char* str_arg = va_arg(arg_list, char*);
            args[i] = (uint32_t)str_arg;
            i++;
        } else if (*format == 'S') {
            struct MyStruct* struct_arg = va_arg(arg_list, struct MyStruct*);
            // Convert the struct pointer to uintptr_t to pass it as an argument
            args[i] = (uint32_t)(uintptr_t)struct_arg;
            i++;
        } else {
            // Handle other data types as needed
        }
        format++;
    }

    va_end(arg_list);
}