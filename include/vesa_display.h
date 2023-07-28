
#ifndef VESA_DISPLAY_H
#define VESA_DISPLAY_H
#include "types.h"
#include <stddef.h>
#include "vga.h"
#include "vesa.h"
#include "types.h"
#include <stdint.h>
#include "bool.h"
//void vesa_initialize(enum vga_color font_color, enum vga_color background_color);
void vesa_set_colors(enum vga_color font_color, enum vga_color background_color);

int get_vesa_row(void);
int get_vesa_col(void);
enum vga_color change_font_color();
void change_background_color();
void cls_screen(char *color);
int set_vesa_row(int row);
int set_vesa_colum(int col);
void print_vesa_d(const char *data, size_t data_length);
static enum vga_color default_font_color = COLOR_RED1;
static enum vga_color default_bg_color = COLOR_BLACK1;
#endif