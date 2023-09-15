#include "../include/stdio.h"
#include "keyboard.h"
#include "string.h"
#include "stdarg.h"
char get_chr() {
    
    char c;
    while(1==1)
    {
        c = kb_getchar();
        if(c != '\0')
        {
            printf("%c",c);

            return c;
        }

    }
    // do {
    //     
        
    // } while (c == '\n' || c == '\0');  // Continue until a non-newline, non-null character is read
    // printf("%c",c);
    // if(c == '\n')
    // {
    //     printf("NEW");
    // }
    // return c;
}
int get_int() {
    int result = 0;
    int sign = 1;
    char c;

    while (1) {
        c = get_chr();

        if (c == '+') {
            continue;
        } else if (c == '-') {
            sign = -1;
        } else if (c >= '0' && c <= '9') {
            result = (result * 10) + (c - '0');
        } else {
            break;
        }
    }

    return result * sign;
}
void scanf(char* format, ...) {
    va_list args;
    va_start(args, format);

    char c;

    while ((c = *format++) != '\0') {
        if (c == '%') {
            char format_specifier = *format++;  // Read the format specifier from the format string
            if (format_specifier == '\0') {
                break;  // Stop if the format string ends unexpectedly
            }

            // Check the format specifier
            if (format_specifier == 'c') {
                char* dest = va_arg(args, char*);
                *dest = get_chr();
                //printf("%c", *dest);  // Print the character
            } else if (format_specifier == 's') {
                char* dest = va_arg(args, char*);
                int i = 0;
                while (1) {
                    char next_char = get_chr();
                    if (next_char == EOF || next_char == '\n' || next_char == '\0') {
                        break;  // Stop if end of input or newline is reached
                    }
                    dest[i++] = next_char;
                }
                dest[i] = '\0';  // Null-terminate the string
                //printf("%s", dest);  // Print the string
            } else if (format_specifier == 'd') {
                int* dest = va_arg(args, int*);
                *dest = get_int();
                
                // Implement reading integers here
            } else if (format_specifier == 'f') {
                double* dest = va_arg(args, double*);
                // Implement reading floating-point numbers here
            }
            // Add more format specifiers as needed
        } else {
            // Handle non-format specifiers here
        }
    }

    va_end(args);
}