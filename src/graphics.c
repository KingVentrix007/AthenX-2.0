#include "bitmap.h"
#include "vesa.h"
#include "vga.h"
#include "font.h"
#include "stdint.h"
#include "stddef.h"
#include "stdarg.h"
#include "fonts.h"
#include "string.h"
#include "timer.h"
#include "speaker.h"
#include "logos.h"
#include "terminal.h"
#include "display.h"
#include "keyboard.h"
#include "maths.h"
#include "graphics.h"
int x = 0;
int y = 0;
unsigned char *VGA_address_13 = (void *)0xA0000;

int printChar(int x, int y, char c) {
	// Loop to "null terminator character"
	int match = 0;
	for (int l = 0; font[l].letter != 0; l++) {
		if (font[l].letter == c) {
			match = l;
			break;
		}
	}

	// Loop through 7 high 5 wide monochrome font
	int maxLength = 0;
	for (int py = 0; py < 7; py++) {
		for (int px = 0; px < 5; px++) {
			if (font[match].code[py][px] == '#') {
                vbe_putpixel(x+px,y+py,VBE_RGB(255,0,0));
				//putpixel(x + px, y + py,Default_font_color);

				// Dynamix width character spacing
				if (px > maxLength) {
					maxLength = px;
				}
			}
		}
	}

	return maxLength;
}
void print_null(int x, int y)
{
    for (uint32 py = y; py < y+26; py++)
    {
        for (uint32 px = x; px < x+24; px++)
                {
                    vbe_putpixel(px,py,VBE_RGB(0,0,0));
                }
    }
    
}

void draw_char(unsigned char c,int fgcolor, int bgcolor, int x, int y, int base_address){
	
	
    char *bitmap = arr_8x8_font;
    int x_char,y_char;
    static unsigned mask[] = {
        1u << 0u, //            1
        1u << 1u, //            2
        1u << 2u, //            4
        1u << 3u, //            8
        1u << 4u, //           16
        1u << 5u, //           32
        1u << 6u, //           64
        1u << 7u, //          128
        1u << 8u, //          256
    };
    unsigned char *gylph=bitmap+(int)c*8;
    for(y_char=0;y_char<8;++y_char){
        for(x_char=0;x_char<8;++x_char){
            //vbe_putpixel(x+(8-x_char),y+y_char,VBE_RGB(255,0,0));
            plot_pixel(x+(8-x_char),y+y_char,gylph[y_char]&mask[x_char]?fgcolor:bgcolor);
        }
    }

	
	
	
	
}
void cool_colors()
{
    
     uint32 x = 0;
    for (uint32 c = 0; c < vbe_get_width()/3; c++) {
        for (uint32 i = 0; i < vbe_get_height() ; i++) {
            vbe_putpixel(x, i, VBE_RGB(c % 255, 0, 0));
        }
        x++;
        //sleep(0.5);
    }
   
    for (uint32 c = 0; c < vbe_get_width()/3; c++) {
        for (uint32 i = 0; i < vbe_get_height(); i++) {
            vbe_putpixel(x, i, VBE_RGB(0, c % 255, 0));
        }
        x++;
         //sleep(0.5);
    }
   
    for (uint32 c = 0; c < vbe_get_width()/3; c++) {
        for (uint32 i = 0; i < vbe_get_height(); i++) {
            vbe_putpixel(x, i, VBE_RGB(0, 0, c % 255));
        }
        x++;
         //sleep(0.5);
    }
}
// int normalize(double *val)
// {
//     int exponent = 0;
//     double value = *val;

//     while (value >= 1.0)
//     {
//         value /= 10.0;
//         ++exponent;
//     }

//     while (value < 0.1)
//     {
//         value *= 10.0;
//         --exponent;
//     }
//     *val = value;
//     return exponent;
// }

// static void ftoa_fixed(char *buffer, double value)
// {
//     /* carry out a fixed conversion of a double value to a string, with a precision of 5 decimal digits.
//      * Values with absolute values less than 0.000001 are rounded to 0.0
//      * Note: this blindly assumes that the buffer will be large enough to hold the largest possible result.
//      * The largest value we expect is an IEEE 754 double precision real, with maximum magnitude of approximately
//      * e+308. The C standard requires an implementation to allow a single conversion to produce up to 512
//      * characters, so that's what we really expect as the buffer size.
//      */

//     int exponent = 0;
//     int places = 0;
//     static const int width = 4;

//     if (value == 0.0)
//     {
//         buffer[0] = '0';
//         buffer[1] = '\0';
//         return;
//     }

//     if (value < 0.0)
//     {
//         *buffer++ = '-';
//         value = -value;
//     }

//     exponent = normalize(&value);

//     while (exponent > 0)
//     {
//         int digit = value * 10;
//         *buffer++ = digit + '0';
//         value = value * 10 - digit;
//         ++places;
//         --exponent;
//     }

//     if (places == 0)
//         *buffer++ = '0';

//     *buffer++ = '.';

//     while (exponent < 0 && places < width)
//     {
//         *buffer++ = '0';
//         --exponent;
//         ++places;
//     }

//     while (places < width)
//     {
//         int digit = value * 10.0;
//         *buffer++ = digit + '0';
//         value = value * 10.0 - digit;
//         ++places;
//     }
//     *buffer = '\0';
// }

// void ftoa_sci(char *buffer, double value)
// {
//     int exponent = 0;
//     int places = 0;
//     static const int width = 4;

//     if (value == 0.0)
//     {
//         buffer[0] = '0';
//         buffer[1] = '\0';
//         return;
//     }

//     if (value < 0.0)
//     {
//         *buffer++ = '-';
//         value = -value;
//     }

//     exponent = normalize(&value);

//     int digit = value * 10.0;
//     *buffer++ = digit + '0';
//     value = value * 10.0 - digit;
//     --exponent;

//     *buffer++ = '.';

//     for (int i = 0; i < width; i++)
//     {
//         int digit = value * 10.0;
//         *buffer++ = digit + '0';
//         value = value * 10.0 - digit;
//     }

//     *buffer++ = 'e';
//     itoa(buffer, exponent, 10);
// }

// void terminal_putchar(char c)
// {
//     //draw_char(c,45,67,x,y,0);
//     if(c == 10)
//     {
//         y = y + 17;
//         x = 0;
//     }
//     else if(c == 8)
//     {

//         if (x < 0)
//         {
//             //beep();
//             x = 0;
//         }
//         else if(x >= vbe_get_width())
//         {
//             x = 5;
//             y = y + 17;
//             beep();
//         }
        
//         else
//         {
//              print_null(x,y);
//             x = x - 10;
//         }
       
//         //bitmap_draw_char(" ",x,y,VBE_RGB(0,0,0));
//     }
//     else
//     {
//         // if(x >= vbe_get_width())
//         // {
//         //     x = 5;
//         //     y = y + 17;
//         //     beep();
//         // }
//         // else if (x < 0)
//         // {
//         //     beep();
//         //     x = 0;
//         // }
//         //else
//         //{
//              bitmap_draw_char(c,x,y,VBE_RGB(255,0,0));
//         //printChar(x,y,c);
// 	        x = x + 10;
//         //}
       
//     }

// 	// printChar(x,y,c);
// 	// x = x + 8;
// }
// void terminal_write(const char *data, size_t size)
// {
//     size_t i;
//     for (i = 0; i < size; i++)
//         terminal_putchar(data[i]);
// }

// int putchar(int ic)
// {
//     char c = (char)ic;
//     //printChar(50,50,c);
//     terminal_write(&c, sizeof(c));

//     return ic;
// }

// static void print(const char *data, size_t data_length)
// {
//     size_t i;
//     for (i = 0; i < data_length; i++)
//         putchar((int)((const unsigned char *)data)[i]);
//         //bitmap_draw_char((int)((const unsigned char *)data)[i],50,70,VBE_RGB(0,35,200));
// }
// int printf(const char *format, ...)
// {
//     va_list parameters;
//     va_start(parameters, format);
//     int written = 0;
//     size_t amount;
//     int rejected_bad_specifier = 0;
//     while (*format != '\0')
//     {
//         if (*format != '%')
//         {
//         print_c:
//             amount = 1;
//             while (format[amount] && format[amount] != '%')
//                 amount++;
//             print(format, amount);
//             format += amount;
//             written += amount;
//             continue;
//         }
//         const char *format_begun_at = format;
//         if (*(++format) == '%')
//             goto print_c;
//         if (rejected_bad_specifier)
//         {
//         incomprehensible_conversion:
//             rejected_bad_specifier = 1;
//             format = format_begun_at;
//             goto print_c;
//         }
//         if (*format == 'c')
//         {
//             format++;
//             char c = (char)va_arg(parameters, int /* char promotes to int */);
//             print(&c, sizeof(c));
//         }
//         else if (*format == 'd')
//         {
//             format++;
//             char *s;
//             itoa(s, va_arg(parameters, int), 10);
//             //bitmap_draw_char('d',50,50,VBE_RGB(0,50,255));
//             print(s, strlen(s));
            
//         }
//         else if (*format == 'f')
//         {
//             format++;
//             char *s;
//             ftoa_fixed(s, va_arg(parameters, double));
//             print(s, strlen(s));
//         }
//         else if (*format == 'e')
//         {
//             format++;
//             char *s;
//             ftoa_sci(s, va_arg(parameters, double));
//             print(s, strlen(s));
//         }
//         else if (*format == 'x')
//         {
//             format++;
//             char *s;
//             itoa(s, va_arg(parameters, unsigned int), 16);
//             print("0x", 2);
//             print(s, strlen(s));
//         }
//         else if (*format == 'p')
//         {
//             format++;
//             char *s;
//             const void *ptr = va_arg(parameters, void *);
//             uintptr_t uptr = (uintptr_t)ptr;
//             itoa(s, uptr, 16);
//             print("0x", 2);
//             print(s, strlen(s));
//         }
//         else if (*format == 's')
//         {
//             format++;
//             const char *s = va_arg(parameters, const char *);
//             print(s, strlen(s));
//         }
//         else
//         {
//             goto incomprehensible_conversion;
//         }
//     }
//     va_end(parameters);
//     return written;
// }
void plot_pixel(int x, int y, char* pixel_color){
    //VGA_address_13 = 
  int pixel_location = (y*320) + x;
  //putpixel(x,y,pixel_color);
   unsigned char* location = (VGA_address_13+pixel_location);
	*location = pixel_color;
  //poke_b ((0xA0000 + pixel_location + base_address), pixel_color);
}

void clear_screen()
{
    for (uint32 y = 0; y < vbe_get_height(); y++)
    {
        for (uint32 x = 0; x < vbe_get_width(); x++)
        {
            vbe_putpixel(x,y,VBE_RGB(0,0,0));
        }
        
    }
    
}


void draw_large_img(int start_x, int start_y,int width, int height)
{

}

void draw_logo(int start_x, int start_y)
{
	//char *logo = log_img;
	cls_screen(COLOR_LIGHT_GREEN);
	int img_h = 220;
	int img_w = 320;
	for (int h = 0; h < img_h; h++)
	{
		for (int w = 0; w < img_w; w++)
		{
			if(strcmp(logo_img[h][w],'#') == 0){
                vbe_putpixel(start_x+w,start_y+h,VBE_RGB(0,255,0));
				plot_pixel(start_x+w,start_y+h,COLOR_GREEN);
			}
			else
			{
                vbe_putpixel(start_x+w,start_y+h,VBE_RGB(0,0,0));
				plot_pixel(start_x+w,start_y+h,COLOR_BLACK);
			}
			
		}
		
	}
    set_screen_x(350);
    set_screen_y(0);
    printf("Size of image in sectors %d", sizeof(logo_img)/512);
}


void draw_square(int x, int y, uint32 color) {
    int size = 10; // Square size is 10x10 pixels

    // Calculate the positions of each pixel in the square
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int pixel_x = x + i;
            int pixel_y = y + j;
            vbe_putpixel(pixel_x, pixel_y,color);
        }
    }
}

// Function to draw an image made up of 10x10 squares at a given position
void draw_image(int x, int y, int rows, int cols) {
    int square_size = 4; // Square size is 10x10 pixels

    // Calculate the positions of each square in the image
    int square_x;
    int square_y;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            square_x = x + (i * square_size);
            square_y = y + (j * square_size);
            if(strcmp(logo_img[j][i],'#') == 0){
                draw_square(square_x, square_y,VBE_RGB(0,255,0));
            }
            else
            {
                draw_square(square_x, square_y,VBE_RGB(0,0,0));
            }
            
        }
    }
    set_screen_x((square_x/2)-100);
    set_screen_y(square_y+square_size*3);
    printf("A T H E N X");
}

char* logo()
{
    draw_image(0,0,320,200);
    while(1==1)
    {
          char c = kb_getchar();
        
        char* s;
        s = ctos(s, c);
      
        if(strcmp(s,"b") == 0)
        {
            
            return s;
        }
        else if (strcmp(s,"h") == 0)
        {
            return s;
        }
        //printf("%s",c);
        
    }
}



typedef struct {
    double x, y, z;
} Point3D;
void vbe_drawline(int x1, int y1, int x2, int y2, int color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        vbe_putpixel(x1, y1, color);
        //printf("X:%fY:%f||",x1,y1);
        if (x1 == x2 && y1 == y2)
            break;
        

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Structure to represent a 3D point


// Function to perform 3D-to-2D projection and draw the shape with lines
void draw3DShape(Point3D* shape, int numVertices, Point3D viewpoint) {
    // Focal length of the camera (you may need to adjust this value)
    double focalLength = 100.0;
    int vertices[numVertices][2];
    // Perform 3D-to-2D projection and draw the vertices
    for (int i = 0; i < numVertices; i++) {
        // Calculate the perspective projection of each 3D point onto the 2D plane
        double projectedX = (focalLength * shape[i].x) / (focalLength - shape[i].z);
        double projectedY = (focalLength * shape[i].y) / (focalLength - shape[i].z);

        // Translate the projected points according to the viewpoint
        int screenX = (int)(viewpoint.x + projectedX);
        int screenY = (int)(viewpoint.y + projectedY);

        // Draw the point on the screen using the vbe_putpixel function
        draw_square(screenX, screenY, VBE_RGB(0,255,0)); // Assuming white color (adjust as needed)
        vertices[i][0] = screenX; 
        vertices[i][1] = screenY;
    }

    // Draw lines between the vertices to form the shape
    printf("Draw lines number of vertices %d\n", numVertices);
    int pos;
    for (int i = 0; i < numVertices; i++) {
        
        // int nextIndex = (i + 1) % numVertices; // Wrap around for the last vertex
        // double projectedX1 = (focalLength * shape[i].x) / (focalLength - shape[i].z);
        // double projectedY1 = (focalLength * shape[i].y) / (focalLength - shape[i].z);
        // double projectedX2 = (focalLength * shape[nextIndex].x) / (focalLength - shape[nextIndex].z);
        // double projectedY2 = (focalLength * shape[nextIndex].y) / (focalLength - shape[nextIndex].z);

        // // Translate the projected points according to the viewpoint
        // int screenX1 = (int)(viewpoint.x + projectedX1);
        // int screenY1 = (int)(viewpoint.y + projectedY1);
        // int screenX2 = (int)(viewpoint.x + projectedX2);
        // int screenY2 = (int)(viewpoint.y + projectedY2);

        // Draw the line on the screen using the vbe_drawline function
        //vbe_drawline(screenX1, screenY1, screenX2, screenY2, VBE_RGB(0,0,255)); // Assuming white color (adjust as needed)
        Point start = {vertices[i][0], vertices[i][1]};
        Point end = {vertices[i+1][0],vertices[i+1][1]};
        printf("SSX: %d, SSY: %d\n",start.x, start.y);
        printf("EEX: %d, EEY: %d\n",end.x, end.y);
        bresenham_line(start, end);


    }
    //  Point start = {vertices[pos][0], vertices[pos][1]};
    // Point end = {vertices[0][0],vertices[0][1]};
    // bresenham_line(start, end);

}

int demo_3D()
{
    Point3D shape[] = {
        {0.0, 0.0, 100.0}, // Vertex 1
        {50.0, 0.0, 200.0}, // Vertex 2
        {0.0, 50.0, 150.0}  // Vertex 3
        // Add more vertices to represent your shape
    };

    int numVertices = sizeof(shape) / sizeof(shape[0]);

    // Viewpoint of the camera (you can change this point to change the perspective)
    Point3D viewpoint = {320.0, 240.0, 0.0}; // Assuming the screen is 640x480

    // Draw the 3D shape on the 2D plane
    printf("Number of vertices: %d\n", numVertices);
    
    draw3DShape(shape, numVertices, viewpoint);
    //vbe_drawline(0,0,12,12, VBE_RGB(0,255,0));
    return 0;

}




void bresenham_line(Point start, Point end) {
    
    int dx = abs(end.x - start.x);
    int dy = abs(end.y - start.y);
    if(dx > vbe_get_width)
    {
        dx = vbe_get_width-dx;
    }
    if (dy > vbe_get_height)
    {
        dy = vbe_get_height-dy;
    }
    int x = start.x;
    int y = start.y;
    printf("SX: %d, SY: %d\n",start.x, start.y);
    printf("EX: %d, EY: %d\n",end.x, end.y);
    printf("DX:%d DY:%d\n", dx, dy);
    sleep(2);
    
    int inc_x = end.x >= start.x ? 1 : -1;
    int inc_y = end.y >= start.y ? 1 : -1;

    int is_steep = dy > dx;

    if (is_steep) {
        int temp = dx;
        dx = dy;
        dy = temp;
    }

    int error = 2 * dy - dx;

    for (int i = 0; i <= dx; i++) {
        vbe_putpixel(x,y,VBE_RGB(0,0,255));

        if (is_steep) {
            y += inc_y;
        } else {
            x += inc_x;
        }

        error += 2 * dy;

        if (error > 0) {
            if (is_steep) {
                x += inc_x;
            } else {
                y += inc_y;
            }
            error -= 2 * dx;
        }
    }
}

int draw_line()
{
    Point start = {0, 0};
    Point end = {7, 4};

    bresenham_line(start, end);
}