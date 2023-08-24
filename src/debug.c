#include <string.h>
#include <vesa.h>
#include <isr.h>
int screen_of_death()
{
    for (size_t y = 0; y < 1024; y++)
    {
        for (size_t x = 0; x < 1280; x++)
        {
            vbe_putpixel(x,y,VBE_RGB(0,51,187));
        }
        
    }
    
}


