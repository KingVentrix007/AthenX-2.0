#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"
typedef struct {
    int x;
    int y;
} Point;

typedef struct
{
    int start_x;
    int start_y;
    int width;
    int height;
    
    uint32 color;
    BOOL filled;
    int line_width; 

}WINDOW;
typedef struct
{
    uint8_t *ptr;
    uint32_t *ptr32;
    uint64_t *ptr64;
}IMAGE_POINTERS;
typedef struct
{
    char name[512];
    int width;
    int height;
    int Bpp;
    IMAGE_POINTERS ptrs;
    

}IMAGE;

int printChar(int x, int y, char c);
void cool_colors();
void clear_screen();
void plot_pixel(int x, int y, char* pixel_color);
void draw_logo(int start_x, int start_y);
//void draw_char(unsigned char c,int fgcolor, int bgcolor, int x, int y, int base_address);
void draw_image(int x, int y, int rows, int cols);
char* logo();
int demo_3D();
void draw_line(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint32 color);
void drawLine(int x0, int y0, int x1, int y1);
int ArtemisVision();
void bresenham_line(Point start, Point end);
void draw_window(int x, int y, int length);
void draw_char_g(unsigned char c,int fgcolor, int bgcolor, int x, int y, int base_address);
int init_window(WINDOW *win);
void draw_error_image();
void draw_low_res_img(IMAGE img_header);
void draw_hi_res_img(int w, int h);
void draw_rect(uint16 x, uint16 y, uint16 width, uint16 height, uint32 color);
#endif


