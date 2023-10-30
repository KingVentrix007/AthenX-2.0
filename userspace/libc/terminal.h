#ifndef __TERMINAL__H
#define __TERMINAL__H
 #include "syscall.h"
int set_terminal_state(int x, int y, int r, int g, int b);
int set_pos(int x,int y);
int set_font_c(int R, int G, int B);
int set_bg_c(int R, int G, int B);
int set_x(int x);
int get_x();
int get_y();
int set_y(int y);
#endif