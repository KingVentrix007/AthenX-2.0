#ifndef DISPLAY_H
#define DISPLAY_H
#include "types.h"
int printf(const char *format, ...);
int display_init(int display_mode,uint32 width,uint32 height, uint32 bpp);
void set_mode_screen(int mode);
void set_screen_x(int x);
void set_screen_y(int y);
int get_screen_x();
int get_screen_y();

void next_line();
#endif
