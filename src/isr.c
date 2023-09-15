
#include "string.h"
#include "kheap.h"
#include "isr.h"
#include "idt.h"
#include "8259_pic.h"
#include "console.h"
#include "terminal.h"
#include "debug.h"
#include "display.h"
#include "time.h"
// For both exceptions and irq interrupt
ISR g_interrupt_handlers[NO_INTERRUPT_HANDLERS];
int isr_count = 0;
// for more details, see Intel manual -> Interrupt & Exception Handling
char *exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available (No Math Coprocessor)",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection",
    "Page Fault",
    "Unknown Interrupt (intel reserved)",
    "x87 FPU Floating-Point Error (Math Fault)",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/**
 * register given handler to interrupt handlers at given num
 */
void isr_register_interrupt_handler(int num, ISR handler) {
    printf_("IRQ %d registered\n", num);
    if (num < NO_INTERRUPT_HANDLERS)
        g_interrupt_handlers[num] = handler;
}

/*
 * turn off current interrupt
*/
void isr_end_interrupt(int num) {
    pic8259_eoi(num);
}

/**
 * invoke isr routine and send eoi to pic,
 * being called in irq.asm
 */
void isr_irq_handler(REGISTERS *reg) {
    if (g_interrupt_handlers[reg->int_no] != NULL) {
        ISR handler = g_interrupt_handlers[reg->int_no];
        handler(reg);
    }
    pic8259_eoi(reg->int_no);
}

static void print_registers(REGISTERS *reg, char caller[100]) {

    printf("%s REGISTERS:\n",caller);
    printf("err_code=%d\n", reg->err_code);
    printf("eax=0x%x, ebx=0x%x, ecx=0x%x, edx=0x%x\n", reg->eax, reg->ebx, reg->ecx, reg->edx);
    printf("edi=0x%x, esi=0x%x, ebp=0x%x, esp=0x%x\n", reg->edi, reg->esi, reg->ebp, reg->esp);
    printf("eip=0x%x, cs=0x%x, ss=0x%x, eflags=0x%x, useresp=0x%x\n", reg->eip, reg->ss, reg->eflags, reg->useresp);
    

}
struct stackframe {
  struct stackframe* ebp;
  uint32_t eip;
};
uint32_t *unwind_stack(REGISTERS *reg,int search_level)
{
    int MaxFrames = 100;
    uintptr_t eip = reg->eip;
    uintptr_t ebp = reg->ebp;
    
    printf("Stack trace:\n");
    int esp_in = 0;
    for (unsigned int frame = 0; frame < MaxFrames; ++frame) {
        // Print the current EIP
        printf_("{/330:255,0,0}");
        printf("Frame %d - EIP: 0x%016x    ", frame, (unsigned long long)eip);
        ADDER_NAME_LIST *addr;
        addr = print_list(eip);
        printf_("{/330:0,255,0}");
        //printf("Stack Trace for Frame %d:\n", frame);
        //TraceStackTrace(100,ebp);
        // Check for a valid stack frame (based on your stack frame layout)
        if (!ebp || ebp <= eip) {
            break; // Stop if we encounter an invalid frame or reach the end
        }

        // Unwind to the previous stack frame
        uintptr_t* stack_ptr = (uintptr_t*)ebp;
        //printf("0x%x\n",ebp);
        uintptr_t old_eip = eip;
        eip = stack_ptr[1]; // Assuming the return address is just above EBP
        //uintptr_t old_ebp;
        
        if(esp_in == 0)
        {
            esp_in = reg->esp;
        }
        ebp = stack_ptr[0]; // Move to the previous EBP
        //printf("%d\n",addr->num_params);
        if(search_level == 1)
        {
            inspect_stack_frame(stack_ptr,reg->esp,addr->num_params,addr->addrtype);
        }
        else if(search_level == 2)
        {
            inspect_stack_frame(stack_ptr,reg->esp,addr->num_params,addr->addrtype);
            extractLocalVars(esp_in,stack_ptr);
        }
        else
        {
            printf("\n");
        }
        
         
         esp_in = stack_ptr;
        
    }
    printf("\n");
    //printf("\n----------------------\n");
    //TraceStackTrace(100);
    // uintptr_t* return_address = (uintptr_t*)(EBP + sizeof(uintptr_t));
    // printf("Return Address: 0x%08x\n", *return_address);

    

    
}
void inspect_stack_frame(int* ebp,int *esp, int total_params, const char* param_types) {
   
   printf("(");
    // Assuming cdecl calling convention
    int* return_address = ebp + 1; // The return address is usually right above ebp

    // Assuming the parameters are stored right above the return address
    int* parameters = return_address + 1;
    int num_parameters = 0;
    //printf("TOTAL_PARAMS = %d\n",total_params);
    while (num_parameters < total_params && param_types[num_parameters] != '\0') {
        char param_type = param_types[num_parameters];
         //DEBUG("");
        switch (param_type) {
            case 'c':
                printf("%c", (char)*parameters);
                break;
            case 'i':
                printf("%d",*parameters);
                break;
            case 'x':
                printf("0x%x",*parameters);
                break;
            case 'p':
                printf("%p",(void*)*parameters);
                break;
            case 's':
                printf("%s",(char*)*parameters);
                break;
            case 'f':
                printf("%f",*(float*)parameters);
                break;
            case 'u':
                printf("Type is user defined");
                //printf("Parameter(char/int/float/string) %d: %c/%d/%f/%s\n", num_parameters + 1,
                       //(char)*parameters, *parameters, *(float*)parameters, (char*)*parameters);
                //printf("%p",(void*)*parameters);
                break;
            default:
                printf("Unknown parameter type %c    ", param_type);
                break;
        }

        num_parameters++;
        parameters++;
        if(total_params > 1 && num_parameters < total_params)
        {
            printf(",");
        }
        
    }
    printf(")\n");
   
}
void extractLocalVars(int* esp, int* ebp) {
    // Calculate the initial ESP to determine the end of the stack frame
    int* initialEsp = esp;
    // printf("\nESP:0x%x\n",esp);
    // printf("EBP:0x%x\n",ebp);
    // Check if we have reached the end of the stack frame
    int count = 0;
    while (ebp > initialEsp) {
        // Access local variables using negative offsets from EBP
        if(count >= 3)
        {
            int localVar = *(int*)ebp;
            printf("%d:::%s:::0x%016p\n", localVar,localVar,ebp);
        //uint32_t hex_var = *(uint32_t*)ebp;
        
        }
        
        count++;
        //printf("0x%x",hax_var);
        // Move down the stack frame
        ebp = ebp -1;
    }
    if(*(int*)ebp != 0)
    {
        //printf("0x%016x\n", ebp+1);
    }
    printf("End of stack frame reached.\n");
}

uintptr_t Trace(struct stackframe *stk)
{
    uintptr_t eip = stk->eip;
    uintptr_t ebp = stk->ebp;
    if (!ebp || ebp <= eip) {
            return -1; // Stop if we encounter an invalid frame or reach the end
        }

        // Unwind to the previous stack frame
        uintptr_t* stack_ptr = (uintptr_t*)ebp;
        eip = stack_ptr[1]; // Assuming the return address is just above EBP
        ebp = stack_ptr[0]; // Move to the previous EBP
        stk->eip = eip;
        stk->ebp = ebp;
        printf("0x%016x\n", (unsigned long long)stk->eip);
}
void TraceStackTrace(unsigned int MaxFrames,uintptr_t ebp_in) {
    struct stackframe *stk;
    uintptr_t ebp_value = ebp_in;
    if(ebp_in == 0)
    {
         __asm__ __volatile__("movl %%ebp, %0" : "=r" (ebp_value));
    }
   

    stk = (struct stackframe*)ebp_value;
    printf("Frame Trace(0x%x):\n",stk->eip);
    
   //printf("0x%016x\n", (unsigned long long)stk->eip);
    // printf("Initial stk->eip = 0x%x\n",stk->eip);
    // printf("Initial reg->eip = 0x%x\n",regs->eip);
    // printf("Initial stk->ebp = 0x%x\n",stk->ebp);
    // printf("Initial reg->ebp = 0x%x\n\n---------------------\n",regs->ebp);
    // for (unsigned int frame = 0; stk && frame < MaxFrames; ++frame) {
    //     // Unwind to the previous stack frame
    //     printf("EIP: 0x%016x\n", (unsigned long long)stk->eip);
    //      //printf("STK ebp = 0x%x\n",stk->ebp);
    //     stk = stk->ebp;
    // }
    //printf("\n");
}

/**
 * invoke exception routine,
 * being called in exception.asm
 */
void isr_exception_handler(REGISTERS reg) {
    //cls_screen(COLOR_BLUE);
    screen_of_death();
    clear_display();
    set_screen_x(0);
    set_screen_y(0);
    printf_("Software Failed Successfully\n");
    if (reg.int_no < 32) {
       
        if(isr_count<=5){
            
             isr_count++;
             sleep(100);
            printf("EXCEPTION: %s\n", exception_messages[reg.int_no]);
            print_registers(&reg,__FUNCTION__);
             //uint32_t *adder;
           
            printf("\nAddress of faulty instruction: 0x%016x\n",reg.eip);
            //printf_("\nFunction address: 0x%x\n", adder);
            ADDER_NAME_LIST function_addr_list;
            get_name_addr(&function_addr_list);
        // printf_("Last registered function called: \n%s : 0x%x\n",function_addr_list.names,function_addr_list.addr);
            //printf_("\n%s : 0x%x\n",function_addr_list.names,function_addr_list.addr);
            uint32_t *targetAddress = reg.eip;
            if (find_by_address(targetAddress)) {
                printf("Address 0x%x found in the list.\n", targetAddress);
            } else {
                //printf_("Address 0x%x not found in the list.\n", targetAddress);
            }
            print_list(reg.eip);
            //kheap_print_blocks();
            //print_stack_trace(reg.ebp);
            asm("sti");
             unwind_stack(&reg,2);
            debug_terminal(&reg);
             for (;;)
            ;
            ERROR("EXITED FROM DEBUG TERMINAL");
        }
       else
       {
            printf_("Max exceptions reached, hanging now\n");
            printf_("Please restart computer");
            printf("EXCEPTION: %s\n", exception_messages[reg.int_no]);
            print_registers(&reg,__FUNCTION__);
             //uint32_t *adder;
            //adder = unwind_stack(&reg);
            printf("\nAddress of fault: 0x%x\n",reg.eip);
            // asm("sti");
            // debug_terminal();
            for (;;)
            ;
       }
        
    }
    if (g_interrupt_handlers[reg.int_no] != NULL) {
        ISR handler = g_interrupt_handlers[reg.int_no];
        handler(&reg);
    }
}



uint32_t *resolve_symbol(uint32_t* addr)
{
    //return addr;
    
}