#ifndef __TERMINAL__H
#define __TERMINAL__H
#include "../../include/syscall.h"
int set_terminal_state(int x, int y, int r, int g, int b);
int set_pos(int x,int y);

#endif