#ifndef DISPLAY_H
#define DISPLAY_H
#include "../include/types.h"

int printf_(const char *format, ...);
int display_init(int display_mode,uint32 width,uint32 height, uint32 bpp);
void set_mode_screen(int mode);
void set_screen_x(int x);
void set_screen_y(int y);
int get_screen_x();
int get_screen_y();
int clear_display();
void next_line();
uint32 get_font_color();
uint32 get_bg_color();
uint32 set_background_color(int r, int g, int b);
int set_font_c(int r, int g, int b);


#endif
