// #define SSFN_IMPLEMENTATION 
// #define SSFN_CONSOLEBITMAP_TRUECOLOR  
// #include "../include/ssfn.h"
#include "../include/graphics.h"
#include "../include/fonts.h"
#include "../include/serial.h"
#include "../include/vesa_display.h"
#include "../include/string.h"
#include "../include/bitmap.h"
#include "../include/font.h"
#include "../include/display.h"
#include "../include/keyboard.h"
#include "../include/fonts.h"
size_t vesa_row;
size_t vesa_column;
//static uint16_t *const VGA_MEMORY = (uint16_t *)0xb8000;
uint8_t vesa_color;
static uint16 *vesa_buffer;
bool controlled_scroll = false;
bool scroll = false;
#define BUFFER_SIZE 1024
int font_mode = 0; // 0 = normal, 1 = bold, 2= large

static inline uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t make_vgaentry(char c, uint8_t color)
{
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | color16 << 8;
}


int set_font_mode(int mode)
{
    font_mode = mode;
}


void vesa_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    switch (color) 
    {
    case COLOR_BLACK:
        color = VBE_RGB(0,0,0);
        break;
    case COLOR_BLUE:
        color = VBE_RGB(0,0,255);
        break;
    case COLOR_GREEN:
        color = VBE_RGB(0,255,0);
        break;
    case COLOR_RED:
        color = VBE_RGB(255,0,0);
        break;
    default:
        color = VBE_RGB(255,0,0);
        break;
    }
    if(font_mode == 0)
    {
         drawchar(c,x,y);
        
    }
    else if (font_mode == 1)
    {
         drawchar_bold(c,x,y);
    }
    
    
    
    //bitmap_draw_char(c,x,y,VBE_RGB(0,255,0));
    
}
int scroll_auto = 0;
int set_scroll_mode(int mode)
{
    scroll_auto = mode;
}
void vesa_putchar(char c)
{
    if (c == '\n' || c == '\r')
    {
       
        if (vesa_row >= vbe_get_height()-16 && scroll_auto == 0)
        {
            // int dirct = kb_get_scancode();
            // while(dirct != SCAN_CODE_KEY_DOWN);
            vesa_scroll(0);
            printf("");
           
            
            
            //printf("HERE");
        }
        if(scroll_auto == 0)
        {
            vesa_column = 0;
            vesa_row = vesa_row +16;
        }
        else if(scroll_auto == 1)
        {
            vesa_column = 0;
            vesa_row = vesa_row +16;
        }
        else if (scroll_auto == 2)
        {
            if(vesa_row >= vbe_get_height()-16)
            {
                 char *cur = get_char();
                while(cur != SCAN_CODE_KEY_DOWN)
                {
                    cur = get_char();
                }
                vesa_scroll(0);
                printf("");
            }
            else
            {
                 vesa_column = 0;
                vesa_row = vesa_row +16;
            }
           
            

        }
        
        else
        {

        }
        //  set_screen_x(0);
        // set_screen_y(0);
        //     clear_screen();
        //     clear_display();
             
       
        return;
    }
    else if (c == '\t')
    {
        vesa_column = vesa_column +4*8;
        return;
    }
    else if (c == '\b')
    {
        
        for (size_t x = vesa_row; x < vesa_row+12; x++)
        {
           for (size_t y = vesa_column; y < vesa_column+20; y++)
           {
             vbe_putpixel(y,x,get_bg_color());
           }
           
        }
        vesa_column = vesa_column - 8;
        vesa_putentryat(' ', vesa_color, vesa_column, vesa_row);
        vesa_putentryat(' ', vesa_color, vesa_column, vesa_row);


        return;
    }
    vesa_putentryat(c, default_font_color,vesa_column, vesa_row);
    vesa_column = vesa_column +8;
   if(vesa_column >= vbe_get_width()-16)
   {
        vesa_row = vesa_row+16;
        vesa_column = 0;


   }
    //vesa_column = vesa_column -1;
}

void vesa_write(const char *data, size_t size)
{
    size_t i;
    for (i = 0; i < size; i++)
        vesa_putchar(data[i]);
}

int putchar_vesa(int ic)
{
    char c = (char)ic;
    vesa_write(&c, sizeof(c));
    //write_serial(c,DEFAULT_COM_DEBUG_PORT);
    return ic;
}
void cls_screen(char *color)
{
    for (uint32 x = 0; x < VGA_WIDTH; x++)
    {
        for (uint32 y = 0; y < VGA_HEIGHT; y++)
        {
            const size_t index = y * VGA_WIDTH + x;
            vesa_buffer[index] = vga_item_entry(' ', COLOR_BRIGHT_RED1,COLOR_BLACK1);
            drawchar(' ',x,y);
        }
        
    }
    
    vesa_column = 0;
    vesa_row = 0;
}


void print_vesa_d(const char *data, size_t data_length)
{
    size_t i;
    for (i = 0; i < data_length; i++)
        putchar_vesa((int)((const unsigned char *)data)[i]);
        
        
}


int get_vesa_row(void)
{
    return vesa_row;
}

int get_vesa_col(void)
{
    return vesa_column;
}
int set_vesa_row(int row)
{
    vesa_row = row;
}

int set_vesa_colum(int col)
{
    vesa_column = col;
}

void vesa_set_colors(enum vga_color font_color, enum vga_color background_color)
{
    vesa_color = make_color(font_color, background_color);
    default_font_color = font_color;
    default_bg_color = background_color;
}

char* get_font()
{
    return font8x16;
}


void drawchar(char character, int x, int y) {
    if (character < 0 || character >= sizeof(font8x16) / sizeof(font8x16[0])) {
        // Character not in font, handle error or return
        return;
    }
    
    const unsigned char *bitmap_data = font8x16[character];
    
    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 8; ++col) {
            unsigned char pixel = bitmap_data[row];
            if ((pixel >> (7 - col)) & 0x01) {
                // Draw the pixel at position (x + col, y + row)
                // Use your custom draw_pixel function here
                uint32 color = get_font_color();
                vbe_putpixel(x + col, y + row,color);
            }
            else
            {
                uint32 color = get_bg_color();
                vbe_putpixel(x + col, y + row,color);
            }
        }
    }
}
void drawchar_bold(char character, int x, int y) {
    if (character < 0 || character >= sizeof(font8x16) / sizeof(font8x16[0])) {
        // Character not in font, handle error or return
        return;
    }

    const unsigned char *bitmap_data = font8x16[character];

    // Draw the character twice to create a bold effect
    for (int bold_factor = 0; bold_factor < 2; ++bold_factor) {
        for (int row = 0; row < 16; ++row) {
            for (int col = 0; col < 8; ++col) {
                unsigned char pixel = bitmap_data[row];
                if ((pixel >> (7 - col)) & 0x01) {
                    // Draw the pixel at position (x + col, y + row + bold_factor)
                    // Use your custom draw_pixel function here
                    uint32 color = get_font_color();
                    vbe_putpixel(x + col, y + row + bold_factor, color);
                } else {
                    // Draw the background pixel
                    uint32 color = get_bg_color();
                    vbe_putpixel(x + col, y + row + bold_factor, color);
                }
            }
        }
    }
}
void draw_square_cursor(int x, int y, int color) {
    for (int i = x; i < x + 8; i++) {
        for (int j = y; j < y + 12; j++) {
            vbe_putpixel(i, j, color);
        }
    }
}

void undraw_square(int x, int y) {
    for (int i = x; i < x + 10; i++) {
        for (int j = y; j < y + 12; j++) {
            vbe_putpixel(i, j, get_bg_color());  // Set pixel to black (assuming 0 represents black)
        }
    }
}

void draw_character_1d(char character, int x, int y, int font_width, int font_height) {
    // if (character < 0 || character >= sizeof(arr_8x16_font) / (font_width * font_height)) {
    //     // Character not in font, handle error or return
    //     return;
    // }
    
    const unsigned char *font_data = &arr_8x16_font[character * font_width * font_height];
    
    for (int row = 0; row < font_height; ++row) {
        for (int col = 0; col < font_width; ++col) {
            unsigned char pixel = font_data[row * font_width + col];
            if (pixel) {
                // Draw the pixel at position (x + col, y + row)
                // Use your custom draw_pixel function here
                vbe_putpixel(x + col, y + row,VBE_RGB(255,0,255));
            }
        }
    }
}
