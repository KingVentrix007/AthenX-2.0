#ifndef __SYS_HANDLER__H
#define __SYS_HANDLER__H
#include "isr.h"
void handle_print_system_call(REGISTERS *reg);
char* handler_get_char_system_call(REGISTERS *reg);
#endif