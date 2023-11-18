#include "../include/display.h"

#include "../include/terminal.h"
#include <stddef.h>
#include "../include/bitmap.h"
#include "../include/vesa.h"
#include "../include/vga.h"
#include "../include/string.h"
#include "../include/bool.h"
//#include "../include/graphics.h"


void kprintf(char * template,...)
{
    //PANIC_T("null");
    va_list parameters;
    va_start(parameters, template);
    int written = 0;
    size_t amount;
    int rejected_bad_specifier = 0;
    while (*template != '\0')
    {
        // if (*template != '%')
        // {
        // print_c:
        //     amount = 1;
        //     while (template[amount] && template[amount] != '%')
        //         amount++;
        //     print(template, amount);
        //     template += amount;
        //     written += amount;
        //     continue;
        // }
        // const char *template_begun_at = template;
        // if (*(++template) == '%')
        //     goto print_c;
        // if (rejected_bad_specifier)
        // {
        // incomprehensible_conversion:
        //     rejected_bad_specifier = 1;
        //     template = template_begun_at;
        //     goto print_c;
        // }
        // if (*template == 'c')
        // {
        //     template++;
        //     char c = (char)va_arg(parameters, int /* char promotes to int */);
        //     print(&c, sizeof(c));
        // }
        // else if (*template == 'd')
        // {
        //     template++;
        //     char *s;
        //     itoa2(s, va_arg(parameters, int), 10);
        //     print(s, strlen(s));
        // }
        // else if (*template == 'f')
        // {
        //     template++;
        //     char *s;
        //     ftoa_fixed(s, va_arg(parameters, double));
        //     print(s, strlen(s));
        // }
        // else if (*template == 'e')
        // {
        //     template++;
        //     char *s;
        //     ftoa_sci(s, va_arg(parameters, double));
        //     print(s, strlen(s));
        // }
        // else if (*template == 'x')
        // {
        //     template++;
        //     char *s;
        //     itoa2(s, va_arg(parameters, unsigned int), 16);
        //     print("0x", 2);
        //     print(s, strlen(s));
        // }
        // else if (*template == 'p')
        // {
        //     template++;
        //     char *s;
        //     const void *ptr = va_arg(parameters, void *);
        //     uintptr_t uptr = (uintptr_t)ptr;
        //     itoa2(s, uptr, 16);
        //     print("0x", 2);
        //     print(s, strlen(s));
        // }
        // else if (*template == 's')
        // {
        //     template++;
        //     const char *s = va_arg(parameters, const char *);
        //     print(s, strlen(s));
        // }
        // else
        // {
        //     goto incomprehensible_conversion;
        // }
    }
    va_end(parameters);
    // //return written;
}