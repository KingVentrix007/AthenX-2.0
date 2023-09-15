
#include "vesa.h"
#include "bios32.h"
#include "console.h"
#include "io_ports.h"
#include "isr.h"
#include "string.h"
#include "types.h"
#include "vesa_display.h"
// vbe information
VBE20_INFOBLOCK g_vbe_infoblock;
VBE20_MODEINFOBLOCK g_vbe_modeinfoblock;
// selected mode
int g_selected_mode = -1;
// selected mode width & height
uint32 g_width = 0, g_height = 0;
// buffer pointer pointing to video memory
uint32 *g_vbe_buffer = NULL;

uint32 vbe_get_width() {
    return g_width;
}

uint32 vbe_get_height() {
    return g_height;
}

// get vbe info
int get_vbe_info() {
    REGISTERS16 in = {0}, out = {0};
    // set specific value 0x4F00 in ax to get vbe info into bios memory area
    in.ax = 0x4F00;
    // set address pointer to BIOS_CONVENTIONAL_MEMORY where vbe info struct will be stored
    in.di = BIOS_CONVENTIONAL_MEMORY;
    int86(0x10, &in, &out);  // call video interrupt 0x10
    // copy vbe info data to our global variable g_vbe_infoblock
    memcpy(&g_vbe_infoblock, (void *)BIOS_CONVENTIONAL_MEMORY, sizeof(VBE20_INFOBLOCK));
    return (out.ax == 0x4F);
}

void get_vbe_mode_info(uint16 mode, VBE20_MODEINFOBLOCK *mode_info) {
    REGISTERS16 in = {0}, out = {0};
    // set specific value 0x4F00 in ax to get vbe mode info into some other bios memory area
    in.ax = 0x4F01;
    in.cx = mode; // set mode info to get
    in.di = BIOS_CONVENTIONAL_MEMORY + 1024;  // address pointer, different than used in get_vbe_info()
    int86(0x10, &in, &out);  // call video interrupt 0x10
    // copy vbe mode info data to parameter mode_info
    memcpy(mode_info, (void *)BIOS_CONVENTIONAL_MEMORY + 1024, sizeof(VBE20_MODEINFOBLOCK));
}

void vbe_set_mode(uint32 mode) {
    REGISTERS16 in = {0}, out = {0};
    // set any given mode, mode is to find by resolution X & Y
    in.ax = 0x4F02;
    in.bx = mode;
    int86(0x10, &in, &out);  // call video interrupt 0x10 to set mode
}

// find the vbe mode by width & height & bits per pixel
uint32 vbe_find_mode(uint32 width, uint32 height, uint32 bpp) {
    // iterate through video modes list
    uint16 *mode_list = (uint16 *)g_vbe_infoblock.VideoModePtr;
    uint16 mode = *mode_list++;
    while (mode != 0xffff) {
        // get each mode info
        get_vbe_mode_info(mode, &g_vbe_modeinfoblock);
        if (g_vbe_modeinfoblock.XResolution == width && g_vbe_modeinfoblock.YResolution == height && g_vbe_modeinfoblock.BitsPerPixel == bpp) {
            return mode;
        }
        mode = *mode_list++;
    }
    return -1;
}
void vese_mem(void *src, int n)
{
    memcpy(*(g_vbe_buffer),src,n);
}
// print availabel modes to console
void vbe_print_available_modes() {
    VBE20_MODEINFOBLOCK modeinfoblock;

    // iterate through video modes list
    uint16 *mode_list = (uint16 *)g_vbe_infoblock.VideoModePtr;
    uint16 mode = *mode_list++;
    while (mode != 0xffff) {
        get_vbe_mode_info(mode, &modeinfoblock);
        if(modeinfoblock.BitsPerPixel >= 32)
        {
            printf_("Mode: %d, X: %d, Y: %d, Bpp: %d\n", mode, modeinfoblock.XResolution, modeinfoblock.YResolution,modeinfoblock.BitsPerPixel);
        }
        
        
        mode = *mode_list++;
    }
}
int * find_biggest_mode()
{
    int x, y;

    VBE20_MODEINFOBLOCK modeinfoblock;

    // iterate through video modes list
    uint16 *mode_list = (uint16 *)g_vbe_infoblock.VideoModePtr;
    uint16 mode = *mode_list++;
    while (mode != 0xffff) {
        get_vbe_mode_info(mode, &modeinfoblock);
        printf_("Mode: %d, X: %d, Y: %d\n", mode, modeinfoblock.XResolution, modeinfoblock.YResolution);
        if(x > modeinfoblock.XResolution && y > modeinfoblock.YResolution)
        {
            x = modeinfoblock.XResolution;
            y = modeinfoblock.YResolution;
        }
        mode = *mode_list++;
    }
    int* out;
    out[0] = x;
    out[1] = y;

    return out;
}

// set rgb values in 32 bit number
uint32 vbe_rgb(uint8 red, uint8 green, uint8 blue) {
    uint32 color = red;
    color <<= 16;
    color |= (green << 8);
    color |= blue;
    return color;
}
uint32_t vbe_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    // Combine the R, G, B, and A values into a single 32-bit pixel value
    uint32_t pixel_value = ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    return pixel_value;
}
// put the pixel on the given x,y point
void vbe_putpixel(int x, int y, uint32 color) {
    uint32 i = y * g_width + x;
    *(g_vbe_buffer + i) = color;
}
void vesa_scroll()
{
    //VGA_clear_screen(Default_screen_color);
    int SCREEN_WIDTH = vbe_get_width();
    int SCREEN_HEIGHT = vbe_get_height();
    int BYTES_PER_PIXEL = 32;
    int lines = 1;
    if(1==1)
    {
      size_t bytesPerLine = SCREEN_WIDTH *4;

    // Calculate the number of bytes to move
        size_t bytesToMove = bytesPerLine * (SCREEN_HEIGHT - lines);

        // Perform the scroll by moving the framebuffer data
        memmove((g_vbe_buffer), (g_vbe_buffer) + bytesToMove, bytesToMove);

        // Clear the newly revealed lines
        //memset((g_vbe_buffer) + bytesToMove, 0, bytesPerLine * lines);
        //vesa_row = vesa_row-100;
        set_vesa_row(0);
        //set_vesa_colum(100);
        int x = get_vesa_row();
        //set_vesa_row(x-100);
        set_vesa_colum(0);
    }
    else
    {

    }
    
    // //vesa_row = VGA_HEIGHT - 1;
}

void vbe_putpixel_v2(int x, int y,int color, unsigned char *buffer)
{
    uint32 i = y * g_width + x;
    *(buffer + i) = color;
}

int vesa_init(uint32 width, uint32 height, uint32 bpp) {
    bios32_init();
    printf_("initializing vesa vbe 2.0\n");
    if (!get_vbe_info()) {
        printf_("No VESA VBE 2.0 detected\n");
        return -1;
    }
    // set this to 1 to print all available modes to console
    #define PRINT_MODES 0
    #if PRINT_MODES
        printf_("Press UP and DOWN arrow keys to scroll\n");
        printf_("Modes:\n");
        vbe_print_available_modes();
        return 1;
    #else
        g_selected_mode = vbe_find_mode(width, height, bpp);
        if (g_selected_mode == -1) {
            printf_("failed to find mode for %d-%d\n", width, height);
            return -1;
        }
        printf_("\nselected mode: %d \n", g_selected_mode);
        // set selection resolution to width & height
        g_width = g_vbe_modeinfoblock.XResolution;
        g_height = g_vbe_modeinfoblock.YResolution;
        // set selected mode video physical address point to buffer for pixel plotting
        g_vbe_buffer = (uint32 *)g_vbe_modeinfoblock.PhysBasePtr;
        // set the mode to start graphics window
        vbe_set_mode(g_selected_mode);
    #endif
    printf_("END\n");
    return 0;
}
