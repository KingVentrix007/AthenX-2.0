#include "string.h"
#include "vga.h"
#include "io_ports.h"
#include "types.h"
#include "bios32.h"
#include "graphics.h"
#include "vesa.h"
#include "types.h"
#include "terminal.h"
#include "bool.h"
#include "vesa_display.h"
size_t vga_row;
size_t vga_column;
//static uint16_t *const VGA_MEMORY = (uint16_t *)0xb8000;
uint8_t vga_color;
static uint16 *vga_buffer;
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
uint16 vga_item_entry(uint8 ch, VGA_COLOR_TYPE fore_color, VGA_COLOR_TYPE back_color) {
    uint16 ax = 0;
    uint8 ah = 0, al = 0;

    ah = back_color;
    ah <<= 4;
    ah |= fore_color;
    ax = ah;
    ax <<= 8;
    al = ch;
    ax |= al;

    return ax;
}

/**
 * set cursor position to given (x, y)
 * by writing to CRT controller registers
 */
void vga_set_cursor_pos(uint8 x, uint8 y) {
    // The screen is 80 characters wide...
    uint16 cursorLocation = y * VGA_WIDTH + x;
    outportb(0x3D4, 14);
    outportb(0x3D5, cursorLocation >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, cursorLocation);
}

/**
 * disable blinking top-left cursor
 * by writing to CRT controller registers
 */
void vga_disable_cursor() {
    outportb(0x3D4, 10);
    outportb(0x3D5, 32);
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define VGA_AC_INDEX 0x3C0
#define VGA_AC_WRITE 0x3C0
#define VGA_AC_READ 0x3C1
#define VGA_MISC_WRITE 0x3C2
#define VGA_SEQ_INDEX 0x3C4
#define VGA_SEQ_DATA 0x3C5
#define VGA_DAC_READ_INDEX 0x3C7
#define VGA_DAC_WRITE_INDEX 0x3C8
#define VGA_DAC_DATA 0x3C9
#define VGA_MISC_READ 0x3CC
#define VGA_GC_INDEX 0x3CE
#define VGA_GC_DATA 0x3CF
#define VGA_CRTC_INDEX 0x3D4 /* 0x3B4 */
#define VGA_CRTC_DATA 0x3D5 /* 0x3B5 */
#define VGA_INSTAT_READ 0x3DA
#define VGA_NUM_SEQ_REGS 5
#define VGA_NUM_CRTC_REGS 25
#define VGA_NUM_GC_REGS 9
#define VGA_NUM_AC_REGS 21
#define VGA_NUM_REGS (1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

unsigned int   VGA_width;
unsigned int   VGA_height;
unsigned int   VGA_bpp;
//unsigned char *VGA_address;
unsigned int VGA_colum;
unsigned int VGA_row;

 

unsigned char mode_320_200_256[] = {
	/* MISC
    *
    * 0x63 => 01100011
    * 7 6 5 4 3 2 1 0
    * 1 1 0 0 0 1 1 0
    * VSP HSP - - CS CS ERAM IOS
    * 7,6 - 480 lines
    * 5,4 - free
    * 3,2 - 28,322 MHZ Clock
    * 1 - Enable Ram
    * 0 - Map 0x3d4 to 0x3b4
    */
	0x63,
	/* SEQ */
	/**
    * index 0x00 - Reset
    * 0x03 = 11
    * Bits 1,0 Synchronous reset
    */
	0x03,
	/**
    * index 0x01
    * Clocking mode register
    * 8/9 Dot Clocks
    */
	0x01,
	/**
    * Map Mask Register, 0x02
    * 0x0F = 1111
    * Enable all 4 Maps Bits 0-3
    * chain 4 mode
    */
	0x0F,
	/**
    * map select register, 0x03
    * no character map enabled
    */
	0x00,
	/**
    * memory mode register 0x04
    * enables ch4,odd/even,extended memory
    */
	0x0E,
	/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
	0xFF,
	/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
	/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00, 0x00
};
// uint8_t input_bytes(uint16_t port)
// {
//     uint8_t ret;
//     __asm__ __volatile__("inb %1, %0"
//                          : "= a"(ret)
//                          : "Nd"(port));
//     return ret;
// }

void output_bytes(uint16_t port, uint8_t val)
{
    __asm__ __volatile__("outb %0, %1"
                         :
                         : "a"(val), "Nd"(port));
}

void write_registers(unsigned char *regs)
{
	unsigned i;

	/* write MISCELLANEOUS reg */
	output_bytes(VGA_MISC_WRITE, *regs);
	regs++;
	/* write SEQUENCER regs */
	for (i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		output_bytes(VGA_SEQ_INDEX, i);
		output_bytes(VGA_SEQ_DATA, *regs);
		regs++;
	}
	/* unlock CRTC registers */
	output_bytes(VGA_CRTC_INDEX, 0x03);
	output_bytes(VGA_CRTC_DATA, input_bytes(VGA_CRTC_DATA) | 0x80);
	output_bytes(VGA_CRTC_INDEX, 0x11);
	output_bytes(VGA_CRTC_DATA, input_bytes(VGA_CRTC_DATA) & ~0x80);
	/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
	/* write CRTC regs */
	for (i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		output_bytes(VGA_CRTC_INDEX, i);
		output_bytes(VGA_CRTC_DATA, *regs);
		regs++;
	}
	/* write GRAPHICS CONTROLLER regs */
	for (i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		output_bytes(VGA_GC_INDEX, i);
		output_bytes(VGA_GC_DATA, *regs);
		regs++;
	}
	/* write ATTRIBUTE CONTROLLER regs */
	for (i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)input_bytes(VGA_INSTAT_READ);
		output_bytes(VGA_AC_INDEX, i);
		output_bytes(VGA_AC_WRITE, *regs);
		regs++;
	}

	/* lock 16-color palette and unblank display */
	(void)input_bytes(VGA_INSTAT_READ);
	output_bytes(VGA_AC_INDEX, 0x20);
}

int vga_graphics_init() 
{
    // REGISTERS16 in_vga = {0}, out_vga = {0};
    // // set any given mode, mode is to find by resolution X & Y
    // // in.ax = 0x4F02;
    // // in.bx = mode;
    // in_vga.ax = 0x0013;
	
    //bios32_init();
    //int86(0x10, &in_vga,&out_vga); 
	//printf("%x",out_vga.ax);
	//beep();
	//asm("cli");
    //return -0; // call 
    write_registers(mode_320_200_256);
     vga_row = 0;
     vga_column = 0;
    VGA_clear_screen(COLOR_BLACK1);
}

void VGA_clear_screen(char *color)
{
   

    unsigned char *VGA_address_graphics = (void *)0xA0000;
	unsigned int x = 0;
	unsigned int y = 0;

	for (y = 0; y < 200; y++)
	{
		for (x = 0; x < 320; x++)
		{
			plot_pixel(x,y,color);
		}
	}
}


//! Finish later


#define BUFFER_SIZE 1024

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

void vga_initialize(enum vga_color font_color, enum vga_color background_color)
{
    vga_row = 0;
    vga_column = 0;
    vga_color = make_color(font_color, background_color);
    //vga_init(600,320,32);
    //vbe_putpixel(vga_row,vga_column,VBE_RGB(255,7,0));
    vga_buffer= (uint16 *)VGA_ADDRESS;
     
    cls_screen(COLOR_WHITE);
    // size_t y;
    // for (y = 0; y < VGA_HEIGHT; y++)
    // {
    //     size_t x;
    //     for (x = 0; x < VGA_WIDTH; x++)
    //     {
    //         const size_t index = y * VGA_WIDTH + x;
    //         vga_init(600,320,32);
    //         //vga_buffer[index] = make_vgaentry(' ', vga_color);
    //     }
    // }
}

void vga_scroll()
{
    //VGA_clear_screen(Default_screen_color);
    vga_column = 0;
    vga_row = 0;
    int i;
    for (i = 0; i < VGA_HEIGHT; i++)
    {
        int m;
        for (m = 0; m < VGA_WIDTH; m++)
        {
            vga_buffer[i * VGA_WIDTH + m] = vga_buffer[(i + 1) * VGA_WIDTH + m];
            
        }
        vga_row--;
    }
    // //vga_row = VGA_HEIGHT - 1;
}

void vga_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    //draw_char(c,color,COLOR_WHITE,y,x,0);
    // if(strcmp(c," ") == 0)
    // {
    //     //printf("HERE");
    // }
    //draw_char(c,color,Default_screen_color,y,x,0);
    // const size_t index = y * VGA_WIDTH + x;
    // vga_buffer[index] = vga_item_entry(c,COLOR_BRIGHT_GREEN1,COLOR_WHITE1);
    //cool_colors();
    //vbe_putpixel(x,y+50,VBE_RGB(255,0,0));
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
     
    //draw_char(c,COLOR_WHITE,COLOR_BLACK,x,y,0);
    
}

void vga_putchar(char c)
{
    if (c == '\n' || c == '\r')
    {
        vga_column = vga_column+16;
        vga_row = 0;
        if (vga_column == VGA_HEIGHT)
            vga_scroll();
        return;
    }
    else if (c == '\t')
    {
        vga_row = vga_row +4*8;
        return;
    }
    else if (c == '\b')
    {
        vga_row = vga_row - 8;
       
        vga_putentryat(' ', vga_color, vga_column, vga_row);
        vga_putentryat(' ', vga_color, vga_column, vga_row);
         //printf(" ");
        return;
    }
    vga_putentryat(c, default_font_color, vga_row,vga_column);
    vga_row = vga_row +9;
    //vga_column = vga_column+10;
   if(vga_row >= 320)
   {
        vga_column = vga_column+16;
        vga_row = 0;
        //printf("90023");
        //write_serial("Q",DEFAULT_COM_DEBUG_PORT);
        

   }
    //vga_column = vga_column -1;
}

void vga_write(const char *data, size_t size)
{
    size_t i;
    for (i = 0; i < size; i++)
        vga_putchar(data[i]);
}

void cls_screen_vga(char *color)
{
    for (uint32 x = 0; x < VGA_WIDTH; x++)
    {
        for (uint32 y = 0; y < VGA_HEIGHT; y++)
        {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = vga_item_entry(' ', COLOR_BRIGHT_RED1,COLOR_BLACK1);
            bitmap_draw_char(' ',x,y,VBE_RGB(0,0,255));
        }
        
    }
    
    vga_column = 0;
    vga_row = 0;
}

void print_g(const char *data, size_t data_length)
{
    size_t i;
    for (i = 0; i < data_length; i++)
        vga_putchar((int)((const unsigned char *)data)[i]);
}
int set_vga_row(int row)
{
    vga_row = row;
}
int set_vga_column(int col)
{
    vga_column = col;
}

int get_vga_row()
{
    return vga_row;
}
int get_vga_column()
{
    return vga_column;
}



