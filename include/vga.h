#ifndef VGA_H
#define VGA_H

#include "types.h"
#include <stddef.h>
#define VGA_ADDRESS        0xB8000
#define VGA_TOTAL_ITEMS    2200

#define VGA_WIDTH     80
#define VGA_HEIGHT    24
#define VGA_MAX_WIDTH 320
#define VGA_MAX_HEIGHT 200
typedef enum {
    COLOR_BLACK1,
    COLOR_BLUE1,
    COLOR_GREEN1,
    COLOR_CYAN1,
    COLOR_RED1,
    COLOR_MAGENTA1,
    COLOR_BROWN1,
    COLOR_GREY1,
    COLOR_DARK_GREY1,
    COLOR_BRIGHT_BLUE1,
    COLOR_BRIGHT_GREEN1,
    COLOR_BRIGHT_CYAN1,
    COLOR_BRIGHT_RED1,
    COLOR_BRIGHT_MAGENTA1,
    COLOR_YELLOW1,
    COLOR_WHITE1,
} VGA_COLOR_TYPE;

/**
 * 16 bit video buffer elements(register ax)
 * 8 bits(ah) higher : 
 * lower 4 bits - forec olor
 * higher 4 bits - back color

 * 8 bits(al) lower :
 * 8 bits : ASCII character to print
 * 
 * returns complete item with fore & back color to be placed at VGA address
*/
uint16 vga_item_entry(uint8 ch, VGA_COLOR_TYPE fore_color, VGA_COLOR_TYPE back_color);

/**
 * set cursor position to given (x, y)
 * by writing to CRT controller registers
 */
void vga_set_cursor_pos(uint8 x, uint8 y);

/**
 * disable blinking top-left cursor
 * by writing to CRT controller registers
 */
void vga_disable_cursor();


int vga_graphics_init();


//void vga_initialize(enum vga_color font_color, enum vga_color background_color);
void vga_set_colors(enum vga_color font_color, enum vga_color background_color);
void print_g(const char *data, size_t data_length);
//int printg(const char *format, ...);
int get_vga_row(void);
int get_vga_col(void);
void print_g(const char *data, size_t data_length);
enum vga_color change_font_color();
void change_background_color();
void cls_screen(char *color);
int set_vga_row(int row);
int set_vga_colum(int col);
void ftoa_fixed_v(char *buffer, double value);
#endif
