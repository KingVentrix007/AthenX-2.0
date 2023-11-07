#include "printf.h"

int parse_ansi(char *format)
{
    int count = 0;
    while (1)
    {
        if((*format >= 65 && *format <= 90) || (*format >= 97 && *format <= 122))
        {
            return count;
        }
        else
        {
            *format++;
        // printf("%c",*format);
            count++;
        }
        
    }
    // printf("%d",count);

    
    
}