#ifndef __MULTITASKING__
#define __MULTITASKING__
#include "../include/types.h"
#include "isr.h"
typedef struct
{
    void *kernel_top;
    int state;
    uint32 address_space;
    uint32 next_task;
    REGISTERS regs;
    

}process_control_block;



#endif