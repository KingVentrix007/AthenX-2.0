// filename: isr.c
#include "../include/keyboard.h"
#include "../include/printf.h"
#include "../include/string.h"
#include "../include/kheap.h"
#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/8259_pic.h"
#include "../include/console.h"
#include "../include/terminal.h"
#include "../include/debug.h"
#include "../include/display.h"
#include "../include/timer.h"
#include "../include/kernel.h"
#include "../include/sys_handler.h"
#include "../include/x86_reg.h"
#include "../include/syscall.h"
// For both exceptions and irq interrupt
ISR g_interrupt_handlers[NO_INTERRUPT_HANDLERS];
int isr_count = 0;
volatile uint32_t *syscall_return = 0;
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

void push_value_to_stack(uint32_t value) {
    asm volatile (
        "push %0\n" // Push the value onto the stack
        :
        : "r" (value)
    );
}
uint32_t get_value_from_stack() {
    uint32_t value;
    asm volatile (
        "pop %0\n" // Pop the value from the stack into a variable
        : "=r" (value)
    );
    return value;
}
/**
 * invoke isr routine and send eoi to pic,
 * being called in irq.asm
 */
void isr_irq_handler(REGISTERS *reg) {
    if (g_interrupt_handlers[reg->int_no] != NULL) {
        ISR handler = g_interrupt_handlers[reg->int_no];
        if(reg->int_no == 42)
        {
            //print_registers(reg,"HOLA");
        }
        
        handler(reg);
        if(reg->int_no == 42)
        {
            //  uint32_t ret = get_EAX();
            // printf("\nret->%d\n",ret);
            // asm volatile (
            //     "pop %0"            // Pop the value from the stack into ret
            //     : "=r" (ret)        // Output operand: ret
            // );
            // printf("\nret->%d\n",ret);
            // asm volatile (
            //     "pop %0"            // Pop the value from the stack into ret
            //     : "=r" (ret)        // Output operand: ret
            // );
            // printf("ret->%d\n",ret);
            //print_registers(reg,"HOLA");
        }
    }
    pic8259_eoi(reg->int_no);
}

void print_registers(REGISTERS *reg, char caller[100]) {

    printf("%s REGISTERS:\n","");
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
    printf_("{/330:255,0,0}");
    //printf("\n[ATTENTION] THE FUNCTIONS ARE IN REVERSE ORDER, FUNCTION 0 IS THE LAST FUNCTION CALLED\nTHE LAST FUNCTION [!!]COULD[!!] BE WRONG BECAUSE _start IS IN ASM AND CANT USE THE LOG SCRIPT");
    printf_("{/330:0,255,0}");
    int MaxFrames = 100;
    ADDER_NAME_LIST *call[100];
    int num_call = 0;
    uintptr_t eip = reg->eip;
    uintptr_t ebp = reg->ebp;
    

    
    //printf("\nStack trace:\n");
    int esp_in = 0;
    for (unsigned int frame = 0; frame < MaxFrames; ++frame) {
        // Print the current EIP
        //printf_("{/330:0,255,255}");
        //printf("Frame %d - EIP: 0x%016x    ", frame, (unsigned long long)eip);
        ADDER_NAME_LIST *addr;
        addr = print_list(eip);
       
        //printf_("{/330:0,255,0}");
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
            //inspect_stack_frame(stack_ptr,reg->esp,addr->num_params,addr->addrtype);
            addr->stack_ptr = stack_ptr;
            addr->esp = reg->esp;
            addr->esp_in = esp_in;
            call[num_call] = addr;
            num_call++;
            //printf("0x%016x\n",addr->addr);
             printf_("{/330:0,255,0}");
            //extractLocalVars(esp_in,stack_ptr);
            //printf("-------\n");
            //extractLocalVarsReverse(esp_in,stack_ptr); 
        }
        else
        {
            printf("\n");
        }
        
         
         esp_in = stack_ptr;
        
    }
    printf("\nCall Sequence(FUNCTIONS ARE IN THE ORDER THEY WHERE CALLED)\n");
    int counter = 1;
    //printf("num %d",num_call);
    display_call_sequnce(call,num_call);
    char mode;
    printf("Press 'b' to enter debug terminal or press 'a' for advanced\n");
    while (1)
    {
        mode = kb_getchar();
        //printf("%c",mode);
        if(mode == 'b')
        {
            //printf("HERE");
            asm("sti");
            debug_terminal(reg);
        }
        else if (mode == 'a'){
            break;
        }
        
    }
    
    
     for (int i = num_call - 1; i >= 0; i--) {
        if((i-1) < 0)
        {
            printf_("{/330:255,0,0}");
            printf("[ERROR]: ");
        }
        printf("Function: %d -> %s",counter,call[i]->names);
        printf_("{/330:0,255,0}");
        
        inspect_stack_frame(call[i]->stack_ptr,call[i]->esp,call[i]->num_params,call[i]->addrtype);
        // if(i+1 < num_call)
        // {   //printf("%d\n",i+1);
        //      printf("0x%016x (caller: %s())\n",call[i]->addr,call[i+1]->names);
            
        // }
        // else
        // {
        //      printf("0x%016x (caller: %s)\n",call[i]->addr,"First Function in stack");
        // }       
        printf_("{/330:0,255,0}");
        #define TO_MUCH_INFO
        #ifdef TO_MUCH_INFO
        extractLocalVars(call[i]->esp_in,call[i]->stack_ptr);
        #endif
        counter++;
    }
    printf("\n");
    //kheap_print_blocks();
    //KERNEL_MEMORY_MAP *map;
    //get_map(&map);
    //display_kernel_memory_map(&map);
    //MULTIBOOT_INFO *info = get_mb_info();
    //printMultibootInfo(info);
    return 0;
    
    
    
    //printf("\n----------------------\n");
    //TraceStackTrace(100);
    // uintptr_t* return_address = (uintptr_t*)(EBP + sizeof(uintptr_t));
    // printf("Return Address: 0x%08x\n", *return_address);

    

    
}
void display_call_sequnce(ADDER_NAME_LIST *call[100], int num_call)
{
     int counter = 1;
    for (int i = num_call - 1; i >= 0; i--) {
        if((i-1) < 0)
        {
            printf_("{/330:255,0,0}");
            printf("[ERROR]: ");
        }
        printf("Function: %d -> %s\n",counter,call[i]->names);
        printf_("{/330:0,255,0}");
        counter++;
    }

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
               printf_("{/330:255,0,255}");
                printf("%d",*parameters);
                 printf_("{/330:0,255,0}");
                break;
            case 'x':
                printf("0x%x",*parameters);
                break;
            case 'p':
               printf_("{/330:128,0,128}");
                printf("%p",(void*)*parameters);
                 printf_("{/330:0,255,0}");
                break;
            case 's':
                printf_("{/330:0,50,250}");
                printf("%s",(char*)*parameters);
                 printf_("{/330:0,255,0}");
                break;
            case 'f':
                printf("%f",*(float*)parameters);
                break;
            case 'u':
                printf_("{/330:128,128,128}");
                printf("Type is user defined");
                //printf("Parameter(char/int/float/string) %d: %c/%d/%f/%s\n", num_parameters + 1,
                       //(char)*parameters, *parameters, *(float*)parameters, (char*)*parameters);
                //printf("%p",(void*)*parameters);
                break;
            default:
                printf_("{/330:128,128,128}");
                printf("Unknown parameter type %c    ", param_type);
                printf_("{/330:0,255,0}");
                break;
        }

        num_parameters++;
        parameters++;
       
        if(total_params > 1 && num_parameters < total_params)
        {
            printf(",");
        }
        
    }
    printf(") -->");
    printf_("{/330:255,215,0}");
   
}
void extractLocalVars(int* esp, int* ebp) {
    // Calculate the initial ESP to determine the end of the stack frame
    int* initialEsp = esp;
    int numLocalVars = ((int)ebp - (int)esp) / sizeof(int);
    int numLocalsChar = ((int)ebp - (int)esp) / sizeof(char);
    printf("Number of local vars: %d /%d\n",numLocalVars-3,numLocalsChar);
    // printf("\nESP:0x%x\n",esp);
    // printf("EBP:0x%x\n",ebp);
    // Check if we have reached the end of the stack frame
    int count = 0;
    while (ebp > initialEsp) {
        // Access local variables using negative offsets from EBP
        if(count >= 3)
        {
            int localVar = *(int*)ebp;
            if(ebp-1 == initialEsp && localVar != 0)
            {
                //ADDER_NAME_LIST *func_adder = print_list(localVar);
                
                 printf_("{/330:0,255,255}");
                printf("(EIP): 0x%016x\n",localVar);
                printf_("{/330:0,255,0}");
                
            }
            else
            {
                printf_("{/330:255,255,255}");
                printf("Local Data(0x%x) %d:",ebp, count-2);
                printf_("{/330:255,0,255}");
                printf(" (int)-> %d  ", localVar);
                printf_("{/330:255,255,255}");
                printf("(0x%x) ", localVar);
                 printf_("{/330:0,50,250}");
                printf("(str)-> %s\n", localVar);
                //printf("LocalVar %d: (int)-> %d (0x%x) (str)-> %s\n", count-2, localVar,localVar,localVar);
                printf_("{/330:0,255,0}");
            }
            
        //uint32_t hex_var = *(uint32_t*)ebp;
        
        }
        
        count++;
        //printf("0x%x",hax_var);
        // Move down the stack frame
        ebp = ebp - 1;
    }
    // if(*(int*)ebp != 0)
    // {
    //     printf("0x%016x\n", ebp-1);
    // }
    printf_("{/330:128,128,128}");
    printf("End of stack frame reached.\n");
    printf_("{/330:0,255,0}");
}
void extractLocalVarsReverse(int* esp, int* ebp) {
    // Calculate the number of local variables
    int numLocalVars = ((int)ebp - (int)esp) / sizeof(int);

    // Assuming 4-byte integers, adjust if your platform uses different sizes

    // Initialize a pointer to the first local variable
    int* localVarPtr = ebp + 2;  // Skip the return address and old EBP

    printf("Local Variables:\n");

    // Print local variables in reverse order
     printf_("{/330:255,255,255}");
    for (int i = numLocalVars - 1; i >= 0; i--) {
        printf("LocalVar %d: (int)%d || (str)%s\n", i, localVarPtr[i],localVarPtr[i]);
    }
     //printf_("{/330:0,255,0}");
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
    
   printf("0x%016x\n", (unsigned long long)stk->eip);
    printf("Initial stk->eip = 0x%x\n",stk->eip);
    //printf("Initial reg->eip = 0x%x\n",regs->eip);
    printf("Initial stk->ebp = 0x%x\n",stk->ebp);
    //printf("Initial reg->ebp = 0x%x\n\n---------------------\n",regs->ebp);
    for (unsigned int frame = 0; stk && frame < MaxFrames; ++frame) {
        // Unwind to the previous stack frame
        printf("EIP: 0x%016x\n", (unsigned long long)stk->eip);
        REGISTERS *regs = get_regs();
        regs->eip = stk->eip;
        regs->ebp = stk->ebp;
        unwind_stack(regs,2);
         //printf("STK ebp = 0x%x\n",stk->ebp);
        stk = stk->ebp;
    }
    printf("\n");
    return;
}
REGISTERS *get_regs()
{
    REGISTERS *regs;
    int eax, ebx, ecx, edx,esi,edi,esp,ebp;

    // Inline assembly to move register values to variables
    asm volatile(
        "movl %%eax, %0\n\t"
        "movl %%ebx, %1\n\t"
        "movl %%ecx, %2\n\t"
        "movl %%edx, %3\n\t"
        
        : "=r" (eax), "=r" (ebx), "=r" (ecx), "=r" (edx)
        :
        :);
    asm volatile(
        "movl %%esi, %0\n\t"
        "movl %%edi, %1\n\t"
        "movl %%esp, %2\n\t"
        "movl %%ebp, %3\n\t"

        : "=r" (esi), "=r" (edi), "=r" (esp), "=r" (ebp)
        :
        :);
        regs->eax = eax;
        regs->ebp = ebp;
        regs->ebx = ebx;
        regs->ecx = ecx;
        regs->edx = edx;
        regs->esi = esi;
        regs ->edi = edi;
        regs->esp = esp;
        regs->ebp = ebp;
        return regs;

}

/**
 * invoke exception routine,
 * being called in exception.asm
 */
void isr_exception_handler(REGISTERS reg) {
    //cls_screen(COLOR_BLUE);
    //screen_of_death();
    //clear_display();
    //set_cursor_x(900);
    //set_screen_y(900+200);
    //printf("FAILURE");
    //set_screen_x(0);
    //set_screen_y(0);
    printf_("Software Failed Successfully\n");
    printf_("Failded\n");
    if (reg.int_no < 32) {
        printf("32\n");
        if(isr_count<=5){
            printf_("5\n");
             isr_count++;
             //sleep(100);
              printf_("{/330:255,0,0}");
            printf("EXCEPTION: %s\n", exception_messages[reg.int_no]);
              
              printf_("{/330:173,216,230}");
            print_registers(&reg,__FUNCTION__);
            printf_("{/330:0,255,0}");
             //uint32_t *adder;
           
            printf("\nAddress of faulty instruction: 0x%016x --> ",reg.eip);
            
              
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
            for(;;);
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
            asm volatile("cli");
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



int my_system_call(uint32_t syscall_number, uint32_t arg1) {
    int result;
    
    asm volatile (
        "movl %[syscall_number], %%eax   \n"  // Set system call number in eax
        "movl %[arg1], %%ebx              \n"  // Set argument 1 in ebx
        "int $0x80                         \n"  // Trigger the system call
        "movl %%eax, %[result]            \n"  // Store the result in 'result'
        : [result] "=r" (result)
        : [syscall_number] "r" (syscall_number), [arg1] "r" (arg1)
        : "eax", "ebx"
    );
    
    return result;
}



int copy_from_user(void *dst, const void *src, size_t size) {
    // Check for NULL pointers
    if (dst == NULL || src == NULL) {
        return -1; // Return an error code indicating invalid pointers
    }

    // Perform the copy
    for (size_t i = 0; i < size; i++) {
        // Use the following line to ensure it works on both little-endian and big-endian systems
        ((unsigned char *)dst)[i] = ((const unsigned char *)src)[i];
    }

    return 0; // Return 0 to indicate success
}





int int_print(REGISTERS* reg)
{
    // printf("int print\n");
    //   printf("ds: %d\n", reg->ds);
    //     printf("edi: %d\n", reg->edi);
    //     printf("esi: %d\n", reg->esi);
    //     printf("ebp: %d\n", reg->ebp);
    //     printf("esp: %d\n", reg->esp);
    //     printf("ebx: %d\n", reg->ebx);
    //     printf("edx: %d\n", reg->edx);
    //     printf("ecx: %d\n", reg->ecx);
    //     printf("eax: %d\n", reg->eax);
        // printf("int_no: %d\n", reg->int_no);
        // printf("err_code: %d\n", reg->err_code);
        // printf("eip: %d\n", reg->eip);
        // printf("cs: %d\n", reg->cs);
        // printf("eflags: %d\n", reg->eflags);
        // printf("useresp: %d\n", reg->useresp);
        // printf("ss: %d\n", reg->ss);
}

