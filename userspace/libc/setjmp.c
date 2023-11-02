#include "setjmp.h"
#include "stdio.h"

void save_reg(struct RegisterState* saved_regs)
{
    // asm volatile (
    //     "movl %%edi, %0\n"
    //     "movl %%esi, %1\n"
    //     "movl %%ebp, %2\n"
    //     "movl %%esp, %3\n"
    //     "movl %%ebx, %4\n"
    //     "movl %%edx, %5\n"
    //     "movl %%ecx, %6\n"
    //     "movl %%eax, %7\n"
    //     : "=m" (saved_regs->edi), "=m" (saved_regs->esi), "=m" (saved_regs->ebp),
    //       "=m" (saved_regs->esp), "=m" (saved_regs->ebx), "=m" (saved_regs->edx),
    //       "=m" (saved_regs->ecx), "=m" (saved_regs->eax)
    //     :
    //     : "memory"
    // );
}