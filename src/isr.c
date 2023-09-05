
#include "string.h"
#include "kheap.h"
#include "isr.h"
#include "idt.h"
#include "8259_pic.h"
#include "console.h"
#include "terminal.h"
#include "debug.h"
#include "display.h"
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

static void print_registers(REGISTERS *reg) {
    printf_("REGISTERS:\n");
    printf_("err_code=%d\n", reg->err_code);
    printf_("eax=0x%x, ebx=0x%x, ecx=0x%x, edx=0x%x\n", reg->eax, reg->ebx, reg->ecx, reg->edx);
    printf_("edi=0x%x, esi=0x%x, ebp=0x%x, esp=0x%x\n", reg->edi, reg->esi, reg->ebp, reg->esp);
    printf_("eip=0x%x, cs=0x%x, ss=0x%x, eflags=0x%x, useresp=0x%x\n", reg->eip, reg->ss, reg->eflags, reg->useresp);

}
uint32_t *unwind_stack(REGISTERS *reg)
{
    uint32_t* esp = reg->esp; // Retrieve saved ESP
    uint32_t* eip = reg->eip;
    uint32_t* ebp = reg->ebp;
    uint32_t return_addr = *(eip);
    //memcpy(&addr, eip, sizeof(eip));
    //printf_("addr = 0x%x\n",return_addr);
    return return_addr;
        // Resolve symbol for the return address
    //uint32_t* function_name = resolve_symbol(return_addr);
    

    
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
        isr_count++;
        printf_("EXCEPTION: %s\n", exception_messages[reg.int_no]);
        print_registers(&reg);
        if(isr_count<=5){
             uint32_t *adder;
            adder = unwind_stack(&reg);
            printf_("\nAddress of fault: 0x%x\n",reg.eip);
            //printf_("\nFunction address: 0x%x\n", adder);
            ADDER_NAME_LIST function_addr_list;
            get_name_addr(&function_addr_list);
        // printf_("Last registered function called: \n%s : 0x%x\n",function_addr_list.names,function_addr_list.addr);
            //printf_("\n%s : 0x%x\n",function_addr_list.names,function_addr_list.addr);
            uint32_t *targetAddress = reg.eip;
            if (find_by_address(targetAddress)) {
                printf_("Address 0x%x found in the list.\n", targetAddress);
            } else {
                //printf_("Address 0x%x not found in the list.\n", targetAddress);
            }
            print_list(reg.eip);
            kheap_print_blocks();
            print_stack_trace(reg.ebp);
            //asm("sti");
            //debug_terminal();
             for (;;)
            ;
            ERROR("EXITED FROM DEBUG TERMINAL");
        }
       else
       {
            printf_("Max exceptions reached, hanging now\n");
            printf_("Please restart computer");
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