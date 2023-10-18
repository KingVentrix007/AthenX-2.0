#ifndef __SYSCALL__H
#define __SYSCALL__H
#include "types.h"
int syscall(int syscall_number, int param1, int param2);
#endif