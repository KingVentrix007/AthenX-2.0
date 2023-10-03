#include "../include/tui.h"
#include "../include/vesa_display.h"
#include "../include/vesa.h"
#include "../include/display.h"
#include "../include/keyboard.h"
#include "../include/graphics.h"
#include "../include/stdbool.h"
#include "../include/printf.h"
#include "../include/installer.h"
int init_tui(TUI_MAIN win)
{
    int row = win.row;
    int col = win.col;
    int h = win.hight;
    int w = win.width;
    
}

int printArrayWithSelection(char* arr[], int arrSize) {
    int start_x = get_screen_x()+5;
    int start_y = get_screen_y()+4;
    int optionBoxX = start_y-4; // X-coordinate of the top-left corner of the option box
    int optionBoxY = start_x-5; // Y-coordinate of the top-left corner of the option box
    int optionBoxWidth = 200; // Width of the option box
    int optionBoxHeight = arrSize * 17; // Height of the option box
    int borderWidth = 2; // Width of the border

    // Draw the border around the option box
    for (int x = optionBoxX; x < optionBoxX + optionBoxWidth; x++) {
        for (int y = optionBoxY; y < optionBoxY + optionBoxHeight; y++) {
            // Check if the current pixel is on the border
            if (x < optionBoxX + borderWidth || x >= optionBoxX + optionBoxWidth - borderWidth ||
                y < optionBoxY + borderWidth || y >= optionBoxY + optionBoxHeight - borderWidth) {
                // Set the border color (you can choose a color value)
                vbe_putpixel(x, y, VBE_RGB(0,125,0));
            }
        }
    }
    int selectedIdx = 0; // Initialize the selected index to the first item
    int keyPressed;

    // Set the font and background color
    set_font_c(0,0,0); // Set font color to white
    set_background_color(255,255,255); // Set background color to black
    
    while (1) {
        // Clear the screen
        set_screen_x(start_x);
        set_screen_y(start_y);

        // Print the list elements with the selected item highlighted
        for (int i = 0; i < arrSize; i++) {
            if (i == selectedIdx) {
                // Highlight the selected item
                set_font_c(0,0,0); // Set font color to black
                set_background_color(0,255,0); // Set background color to white
            } else {
                 // Unhighlight other items
                set_font_c(0,255,0); // Set font color to white
                set_background_color(0,0,0); // Set background color to black
            }

            // Print the list item
            int x = get_screen_y();
            printf("%s\n", arr[i]);
            set_screen_y(x);
        }

        // Reset font and background colors for subsequent text
        set_font_color(VBE_RGB(0,255,0)); // Set font color to white
        set_background_color(0,0,0); // Set background color to black

        // Wait for a key press
        keyPressed = kb_get_scancode();

        // Check the key pressed
       // Check the key pressed
        if (keyPressed == SCAN_CODE_KEY_UP && selectedIdx > 0) {
            // Move selection up
            selectedIdx--;
        } else if (keyPressed == SCAN_CODE_KEY_DOWN && selectedIdx < arrSize - 1) {
            // Move selection down
            selectedIdx++;
        } else if (keyPressed == SCAN_CODE_KEY_ENTER) {
            // User pressed Enter, return the selected index
            return selectedIdx;
        }
    }
}
int text_input_start = 0;
int text_input_end = 0;
bool cursor_visible = true;

void toggle_cursor_visibility() {
    cursor_visible = !cursor_visible;
}

void repaint_rect()
{
     int borderWidth = 2;
    uint32 rectangle_border_color = VBE_RGB(0,0,255);
     uint32 rectangle_fill_color = VBE_RGB(255,255,255);
    // Define the dimensions and position of the rectangle
    int RECTANGLE_X = 0;
    int RECTANGLE_Y = (vbe_get_height() - 25); // 40 pixels from the bottom of the screen
    int RECTANGLE_WIDTH = vbe_get_width()-borderWidth*2;
    int RECTANGLE_HEIGHT = 25;

    // Draw the rectangle at the bottom of the screen
    for (int x = RECTANGLE_X; x < RECTANGLE_X + RECTANGLE_WIDTH; x++) {
        for (int y = RECTANGLE_Y; y < RECTANGLE_Y + RECTANGLE_HEIGHT; y++) {
            // Check if the current pixel is on the border
            if (x < RECTANGLE_X + borderWidth || x >= RECTANGLE_X + RECTANGLE_WIDTH - borderWidth ||
                y < RECTANGLE_Y + borderWidth || y >= RECTANGLE_Y + RECTANGLE_HEIGHT - borderWidth) {
                // Set the rectangle border color (you can choose a color value)
                vbe_putpixel(x, y, rectangle_border_color);
            } else {
                // Set the rectangle fill color (you can choose a color value)
                vbe_putpixel(x, y, rectangle_fill_color);
            }
        }
    }
    set_screen_x((vbe_get_height() - 20));
    set_screen_y(RECTANGLE_X+2);
    //printf(">");
}
void tui_main(int argc, char *argv[]) {
    set_background_color(255,255,255);
    clear_screen();
    // Initialize your screen drawing functions and setup here
    int borderWidth = 2;
    uint32 rectangle_border_color = VBE_RGB(0,0,255);
     uint32 rectangle_fill_color = VBE_RGB(255,255,255);
    // Define the dimensions and position of the rectangle
    int RECTANGLE_X = 0;
    int RECTANGLE_Y = (vbe_get_height() - 25); // 40 pixels from the bottom of the screen
    int RECTANGLE_WIDTH = vbe_get_width()-borderWidth*2;
    int RECTANGLE_HEIGHT = 25;

    // Draw the rectangle at the bottom of the screen
    for (int x = RECTANGLE_X; x < RECTANGLE_X + RECTANGLE_WIDTH; x++) {
        for (int y = RECTANGLE_Y; y < RECTANGLE_Y + RECTANGLE_HEIGHT; y++) {
            // Check if the current pixel is on the border
            if (x < RECTANGLE_X + borderWidth || x >= RECTANGLE_X + RECTANGLE_WIDTH - borderWidth ||
                y < RECTANGLE_Y + borderWidth || y >= RECTANGLE_Y + RECTANGLE_HEIGHT - borderWidth) {
                // Set the rectangle border color (you can choose a color value)
                vbe_putpixel(x, y, rectangle_border_color);
            } else {
                // Set the rectangle fill color (you can choose a color value)
                vbe_putpixel(x, y, rectangle_fill_color);
            }
        }
    }
  
    set_screen_x((vbe_get_height() - 20));
    set_screen_y(RECTANGLE_X+2);
    set_font_c(0,0,255);
    set_background_color(255,255,255);
    if(strcmp(argv,"install"))
    {
        tui_install();
    }
    printf(">");
    uint8_t byte;
    char buffer[512];
    memset(buffer,0,sizeof(buffer));
    buffer[0] = '\0';
    //DEBUG("HERE?");
    //beep();
    while(1)
    {    
            //beep();
           
            char c = kb_getchar();
            int ticks = get_ticks();
            if(ticks >= 500)
            {

                undraw_square(get_screen_x(),get_screen_y());
                reset_ticks();
            }
             if (ticks >= 50) {
                toggle_cursor_visibility();
                reset_ticks();
            }
             
            //printf_("%d",c);
            
           
            if(c == '\b')
            {
                //crude_song();
                if(backspace(buffer))
                {
                    printf("\b");
                    
                    //set_cursor_x(get_cursor_x()-2);
                    //printf_(" ");
                    //console_ungetchar();
                }
                else
                {
                    ///beep();
                }
                
                //printf_("\b");
            }
            
            else if (c == '\n' && is_all_one_type(buffer) == 0)
            {
                //printf_("\n");
                 set_font_c(0,0,255);
                set_background_color(255,255,255);
                int old_x = get_screen_x();
                int old_y = get_screen_y();
                undraw_square(get_screen_y(),get_screen_x());
                set_screen_x(0);
                set_screen_y(0);
                cmd_handler(buffer);
                set_screen_x((vbe_get_height() - 20));
                set_screen_y(RECTANGLE_X+2);
                repaint_rect();
                memset(buffer, 0,sizeof(buffer));
                //printf_("\n");
                 set_font_c(0,0,255);
                set_background_color(255,255,255);
                
                //next_line();
                //printf("\n");
                //set_screen_x(0);
                //set_terminal_colum(get_terminal_col()+16);
                //set_terminal_row(0);
                printf(">");
                
                //crude_song();
            }
            else if(c == '\n')
            {
                undraw_square(get_screen_y(),get_screen_x());
                set_screen_x((vbe_get_height() - 20));
                set_screen_y(RECTANGLE_X+2);
                repaint_rect();
                printf(">");
                memset(buffer,0,sizeof(buffer));
            }
            else if (c == '\0')
            {
                //DEBUG("NULL");
                
                //beep();
            }
            else
            {
                
                char* s;
                s = ctos(s, c);
                //printf_(s);
                //undraw_square(get_screen_x(),get_screen_y());
                // printf_(s);
                undraw_square(get_screen_y(),get_screen_x());
                printf(s);
                //printf_("X{}");
                //undraw_square(get_screen_x()-10,get_screen_y());
                //printf_(s);
                 
                 //printf_(s);
                //printf_(s);
                
                append(buffer,c);
               
                
            }
             if (cursor_visible) {
                draw_square_cursor(get_screen_y(),get_screen_x(),VBE_RGB(0, 0, 255));
            } else {
               undraw_square(get_screen_y(),get_screen_x());
    }
            //undraw_square(get_screen_x(),get_screen_y());
            
             //printf_("CAY");
            
            
    }
    
    // Print the '>' symbol inside the rectangle


    // You can add more code to handle user interaction or perform other tasks here
}


int tui_install()
{
    
}