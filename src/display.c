#include "string.h"
#include "vga.h"
#include "vesa.h"
#include "console.h"
#include "types.h"
#include "terminal.h"
#include "bool.h"
#include "vesa_display.h"
#include "display.h"
int display_mode_screen;
size_t screen_x;
size_t screen_y;
int display_init(int display_mode,uint32 width,uint32 height, uint32 bpp)
{
    int ret_int;
    if(display_mode == 0)
    {
        if(width == -200044 && height == -950481)
        {
            ret_int = vesa_init(600,800,bpp);
            int* out = find_biggest_mode();
            ret_int = vesa_init(out[0],out[1],bpp);
            
        }
        else
        {
            ret_int = vesa_init(width,height,bpp);
            display_mode_screen = 0;
        }
        
    }
    else if (display_mode == 1)
    {
        console_init(COLOR_GREEN1,COLOR_BLACK1);
        display_mode_screen = 1;
        ret_int = 0;
    }
    else if (display_mode == 2)
    {
        vga_graphics_init();
        display_mode_screen = 2;
        ret_int = 0;
    }
    return ret_int;
    
    
}
void next_line()
{
    if(display_mode_screen == 0)
    {
        set_screen_y(get_screen_y()+16);
    }
    else if (display_mode_screen == 1)
    {
        //printf("Coming soon");
        printf("\n");
    }
    else if (display_mode_screen == 2)
    {
       set_screen_y(get_screen_y()+8); 
    }
}
void set_mode_screen(int mode)
{
    display_mode_screen = mode;
}

void set_screen_x(int x)
{
    if(display_mode_screen == 0)
    {
        set_vesa_row(x);
    }
    else if (display_mode_screen == 1)
    {
        set_cursor_x(x);
    }
    else if (display_mode_screen == 2)
    {
       set_vga_row(x);
    }
    
    
    screen_x = x;
}
void set_screen_y(int y)
{
    if(display_mode_screen == 0)
    {
        set_vesa_colum(y);
    }
    else if (display_mode_screen == 1)
    {
        set_cursor_y(y);
    }
    else if (display_mode_screen == 2)
    {
       set_vga_column(y);
    }
    screen_y = y;
}
int get_screen_x()
{
    size_t x;
    if(display_mode_screen == 0)
    {
       x = get_vesa_row();
    }
    else if (display_mode_screen == 1)
    {
        x = get_cursor_x();
    }
    else if (display_mode_screen == 2)
    {
       x = get_vga_row();
    }
    return x;
}
int get_screen_y()
{
    size_t y;
    if(display_mode_screen == 0)
    {
       y = get_vesa_col();
    }
    else if (display_mode_screen == 1)
    {
        y = get_cursor_y();
    }
    else if (display_mode_screen == 2)
    {
       y = get_vga_column();
    }
    return y;
}

void set_font_color(uint32_t color)
{
    if(display_mode_screen == 0)
    {
       
    }
    else if (display_mode_screen == 1)
    {
        
    }
    else if (display_mode_screen == 2)
    {
      
    }
}


int clear_display()
{
    if(display_mode_screen == 1)
    {
        console_clear(COLOR_BLACK1,COLOR_BLACK1);
    }
}


static void print(const char *data, size_t data_length)
{
    if(display_mode_screen == 0)
    {
        print_vesa_d(data, data_length);
        // size_t i;
        // for (i = 0; i < data_length; i++)
        //     vga_putchar((int)((const unsigned char *)data)[i]);
    }
    else if (display_mode_screen == 1)
    {
        size_t i;
        for (i = 0; i < data_length; i++)
            console_putchar((int)((const unsigned char *)data)[i]);
    }
    else if (display_mode_screen == 2)
    {
       print_g(data,data_length);
    }
    
    
        
}











void reverse_d(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        swap(*(str + start), *(str + end));
        start++;
        end--;
    }
}
void itoa_d(char *str, int num, int base)
{
    int i = 0;
    boolean isNegative = false;

    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // reverse_d the string
    reverse_d(str, i);
}
void ftoa_fixed_v(char *buffer, double value)
{
    /* carry out a fixed conversion of a double value to a string, with a precision of 5 decimal digits.
     * Values with absolute values less than 0.000001 are rounded to 0.0
     * Note: this blindly assumes that the buffer will be large enough to hold the largest possible result.
     * The largest value we expect is an IEEE 754 double precision real, with maximum magnitude of approximately
     * e+308. The C standard requires an implementation to allow a single conversion to produce up to 512
     * characters, so that's what we really expect as the buffer size.
     */

    int exponent = 0;
    int places = 0;
    static const int width = 4;

    if (value == 0.0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    if (value < 0.0)
    {
        *buffer++ = '-';
        value = -value;
    }

    exponent = normalize_v(&value);

    while (exponent > 0)
    {
        int digit = value * 10;
        *buffer++ = digit + '0';
        value = value * 10 - digit;
        ++places;
        --exponent;
    }

    if (places == 0)
        *buffer++ = '0';

    *buffer++ = '.';

    while (exponent < 0 && places < width)
    {
        *buffer++ = '0';
        --exponent;
        ++places;
    }

    while (places < width)
    {
        int digit = value * 10.0;
        *buffer++ = digit + '0';
        value = value * 10.0 - digit;
        ++places;
    }
    *buffer = '\0';
}
void ftoa_sci_v(char *buffer, double value)
{
    int exponent = 0;
    int places = 0;
    static const int width = 4;

    if (value == 0.0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    if (value < 0.0)
    {
        *buffer++ = '-';
        value = -value;
    }

    exponent = normalize_v(&value);

    int digit = value * 10.0;
    *buffer++ = digit + '0';
    value = value * 10.0 - digit;
    --exponent;

    *buffer++ = '.';

    for (int i = 0; i < width; i++)
    {
        int digit = value * 10.0;
        *buffer++ = digit + '0';
        value = value * 10.0 - digit;
    }

    *buffer++ = 'e';
    itoa_d(buffer, exponent, 10);
}
int normalize_v(double *val)
{
    int exponent = 0;
    double value = *val;

    while (value >= 1.0)
    {
        value /= 10.0;
        ++exponent;
    }

    while (value < 0.1)
    {
        value *= 10.0;
        --exponent;
    }
    *val = value;
    return exponent;
}
int printf(const char *format, ...)
{
    
    va_list parameters;
    va_start(parameters, format);
    int written = 0;
    size_t amount;
    int rejected_bad_specifier = 0;
    while (*format != '\0')
    {
        if (*format != '%')
        {
        print_c:
            amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            print(format, amount);
            format += amount;
            written += amount;
            continue;
        }
        const char *format_begun_at = format;
        if (*(++format) == '%')
            goto print_c;
        if (rejected_bad_specifier)
        {
        incomprehensible_conversion:
            rejected_bad_specifier = 1;
            format = format_begun_at;
            goto print_c;
        }
        if (*format == 'c')
        {
            format++;
            char c = (char)va_arg(parameters, int /* char promotes to int */);
            print(&c, sizeof(c));
        }
        else if (*format == 'd')
        {
            //beep();
            format++;
            char *s;
            itoa_d(s, va_arg(parameters, int), 10);
            print(s, strlen(s));
            
        }
        else if (*format == 'f')
        {
            format++;
            char *s;
            ftoa_fixed_v(s, va_arg(parameters, double));
            print(s, strlen(s));
        }
        else if (*format == 'e')
        {
            format++;
            char *s;
            ftoa_sci_v(s, va_arg(parameters, double));
            print(s, strlen(s));
        }
        else if (*format == 'x')
        {
            format++;
            char *s;
            itoa_d(s, va_arg(parameters, unsigned int), 16);
            //print_g("0x", 2);
            print(s, strlen(s));
        }
        else if (*format == 'p')
        {
            format++;
            char *s;
            const void *ptr = va_arg(parameters, void *);
            uintptr_t uptr = (uintptr_t)ptr;
            itoa_d(s, uptr, 16);
            //print_g("0x", 2);
            print(s, strlen(s));
        }
        else if (*format == 's')
        {
            format++;
            const char *s = va_arg(parameters, const char *);
            print(s, strlen(s));
        }
        else
        {
            goto incomprehensible_conversion;
        }
    }
    va_end(parameters);
    return written;
}







