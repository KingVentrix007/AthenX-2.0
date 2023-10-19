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
#define SYSCALL_EXIT 0x80
#define SYSCALL_PRINT 0x81
#define SYSCALL_TEST_RET 0x83
int system_call_handler_c(int syscall_number, int param1, int param2) {
    int result = 0;
    // printf("\nsyscall_number: %d\n", syscall_number);
    // printf("param1: %d\n", param1);
    // printf("param2: %d\n", param2);
    switch (syscall_number) {
        case SYSCALL_PRINT:
            printf("Parm%s\n",param1);
            // Perform the print operation (you'll need to implement this)
            // For example, if you have a kernel function called print_message:
            // result = print_message((const char*)param1);
            result = 9977;
            break;

        case SYSCALL_TEST_RET:
            
            // Return the value 6677 for the test_ret syscall
            result = 6677;
            break;

        // Add more cases for other system calls as needed
        case SYSCALL_EXIT:
            asm volatile("jmp process_kernel_return");
        default:
            printf("IDK - >%d(0x%x)\n",syscall_number,syscall_number);
            // Handle unknown system calls or errors
            // You can set an error code or take appropriate action.
            break;
    }
    pic8259_eoi(42);
    // printf("returning");
    return result;
}