#include "../include/isr.h"
#include "../include/printf.h"
#include "../include/keyboard.h"
void handle_print_system_call(REGISTERS *reg) {
    // Assuming the message is stored in reg->eax
    const char *message = (const char *)reg->ebx;

    // Display the message (you need to implement the display function)
    printf(message);
    // const char *message2 = (const char *)reg->ebx;
    // printf(message2);
}

char* handler_get_char_system_call(REGISTERS *reg)
{
    asm("sti");
    //printf("sys call get chr\n");
     //return "C";
     
        while(1)
        {
            char *c = get_chr();
            if(c == -1 || c == 0)
            {

            }
            else
            {
                return c;
            }
           
        }
        
        //printf("C%c",chr);
     
    
}
