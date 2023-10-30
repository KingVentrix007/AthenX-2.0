
#include "../include/vesa_display.h"
//#include "../include/vtconsole.h"
#include "../include/debug.h"
#include "../include/string.h"
#include "../include/vga.h"
#include "../include/vesa.h"
#include "../include/console.h"
#include "../include/types.h"
#include "../include/terminal.h"
#include "../include/bool.h"
#include "../include/vesa_display.h"
#include "../include/display.h"
#include "../include/keyboard.h"
int display_mode_screen;
size_t screen_x;
size_t screen_y;
uint32 font_color;
uint32 background_color;
int display_init(int display_mode,uint32 width,uint32 height, uint32 bpp)
{
    int ret_int;
    if(display_mode == 0)
    {
        // if(width == -200044 && height == -950481)
        // {
        //     ret_int = vesa_init(600,800,bpp);
        //     int* out = find_biggest_mode();
        //     ret_int = vesa_init(out[0],out[1],bpp);
            
        // }
        
        
            //asm("cli");
            //printf_("HERE");
            set_font_c(0,255,0);
            ret_int = vesa_init(width,height,bpp);
            printf_("\nMADE IT OUT\n");
            display_mode_screen = 0;
            //asm("sti");
        
        
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
int set_font_c(int r,int g,int b)
{
   font_color = VBE_RGB(r,g,b);
    // beep();
    // printf_("set_font_c(%d,%d,%d)",r,g,b);
}
int set_font_c32(uint32_t c)
{
    font_color = c;
}
int set_bg_color32(uint32_t c)
{
    background_color = c;
}
uint32 get_font_color()
{
    return font_color;
}
uint32 get_bg_color()
{
    return background_color;
}
uint32 set_background_color(int r, int g, int b)
{
    background_color = VBE_RGB(r,g,b);
}
void next_line()
{
    if(display_mode_screen == 0)
    {
        if(get_cursor_y() <= vbe_get_height())
        {
            set_screen_y(get_screen_y()+16);
        }
        else if(get_screen_y() >= vbe_get_height())
        {
            clear_display();
            cls_screen(VBE_RGB(0,0,0));
        }
        

    }
    else if (display_mode_screen == 1)
    {
        //printf_("Coming soon");
        printf_("\n");
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
    int x;
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
// int set_font_c(int r,int g,int b)
// {
//    font_color = VBE_RGB(r,g,b);
//     // beep();
//     // printf_("set_font_c(%d,%d,%d)",r,g,b);
// }

int clear_display()
{
    if(display_mode_screen == 1)
    {
        console_clear(COLOR_BLACK1,COLOR_BLACK1);
    }
    cls_screen(VBE_RGB(0,0,0));
    set_screen_x(0);
    set_screen_y(0);
        // clear_screen();
        // clear_display();
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
int printf_(const char *format, ...)
{
    //FUNC_ADDR_NAME(&printf_);
    va_list parameters;
    va_start(parameters, format);
    int written = 0;
    size_t amount;
    int rejected_bad_specifier = 0;
    while (*format != '\0')
    {
        if (*format == '{')
        {
            // Parse RGB values
            if (*(format + 1) == '/' && *(format + 2) == '3' && *(format + 3) == '3' && *(format + 4) == '0' && *(format + 5) == ':')
            {
                int r = 0, g = 0, b = 0;
                const char *rgb_start = format + 6;
                while (*rgb_start >= '0' && *rgb_start <= '9')
                {
                    r = r * 10 + (*rgb_start - '0');
                    rgb_start++;
                }
                if (*rgb_start != ',')
                    goto print_c;

                rgb_start++;
                const char *g_start = rgb_start;
                while (*g_start >= '0' && *g_start <= '9')
                {
                    g = g * 10 + (*g_start - '0');
                    g_start++;
                }
                if (*g_start != ',')
                    goto print_c;

                g_start++;
                const char *b_start = g_start;
                while (*b_start >= '0' && *b_start <= '9')
                {
                    b = b * 10 + (*b_start - '0');
                    b_start++;
                }

                set_font_c(r, g, b);
                format = b_start+1;
                continue;
            }
            else if (*(format + 1) == '/' && *(format + 2) == '3' && *(format + 3) == '3' && *(format + 4) == '1' && *(format + 5) == ':')
            {
                int r = 0, g = 0, b = 0;
                const char *rgb_start = format + 6;
                while (*rgb_start >= '0' && *rgb_start <= '9')
                {
                    r = r * 10 + (*rgb_start - '0');
                    rgb_start++;
                }
                if (*rgb_start != ',')
                    goto print_c;

                rgb_start++;
                const char *g_start = rgb_start;
                while (*g_start >= '0' && *g_start <= '9')
                {
                    g = g * 10 + (*g_start - '0');
                    g_start++;
                }
                if (*g_start != ',')
                    goto print_c;

                g_start++;
                const char *b_start = g_start;
                while (*b_start >= '0' && *b_start <= '9')
                {
                    b = b * 10 + (*b_start - '0');
                    b_start++;
                }

                set_background_color(r, g, b);
                format = b_start+1;
                continue;
            }
            
        }
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

int kassert(int ret,int expected_ret,int level)
{
    if(ret != expected_ret)
    {
        if(level == 1)
        {
            return 1;
        }
        else if (level == 2)
        {
            printf_("WANING");
        }
        else if (level >= 3);
        {
            //__asm__("int $3");
        }
        
        
    }
    if(ret == expected_ret)
    {
        return 0;
    }
}





// vtconsole_t* vtc;
// void init_vtc()
// {
//     vtc = vtconsole(vbe_get_width(), vbe_get_height(), ter, cursor_move_callback);
// }
// void print_vtc(const char* s)
// {
//     vtconsole_write(vtc, s, strlen(s));
// }


