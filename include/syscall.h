#ifndef __SYSCALL__H
#define __SYSCALL__H
#include "types.h"
extern int system_call(uint32_t syscall_number, uint32_t arg1, uint32_t *results);
int system_call_2(uint32_t syscall_number, uint32_t* args, uint32_t* results, int num_args);
int set_ret(uint32_t ret);
uint32_t get_ret();
int syscall_wrapper(int intnum, ...);
int my_system_call_user(int syscall_number, int arg1);
#endif