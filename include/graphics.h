#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"
int printChar(int x, int y, char c);
void cool_colors();
void clear_screen();
void plot_pixel(int x, int y, char* pixel_color);
void draw_logo(int start_x, int start_y);
void draw_char(unsigned char c,int fgcolor, int bgcolor, int x, int y, int base_address);
#endif


