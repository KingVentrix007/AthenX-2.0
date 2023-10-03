#include "../include/timer.h"
#include "../include/installer.h"
#include "../include/types.h"
#include <stddef.h>
#include "../include/string.h"
#include "../include/ide.h"
#include "../include/vesa_display.h"
#include "../include/printf.h"
#include "../include/keyboard.h"
#include "../include/display.h"
#include "../include/graphics.h"
#include "../include/icons.h"
void display_percentage(uint32_t sectors_written, uint32_t total_sectors,int x, int y,const char* message) {
    // Calculate the percentage
    float percentage = (float)sectors_written / total_sectors * 100.0f;

    // Convert the percentage to a string
    char percentage_str[16];
    snprintf(percentage_str, sizeof(percentage_str), "%.2f%%", percentage);

    // Set the screen position for displaying the percentage
    set_screen_x(x);
    set_screen_y(y);

    // Display the percentage on the screen
    printf("%s: %s of %d sectors",message, percentage_str,total_sectors);
}
// Function to remove a section concerning "AthenX (install)" from the buffer
void remove_athenx_install_section(char* buffer) {
    char* start = strstr(buffer, "menuentry \"AthenX (install)\"");
    if (start != NULL) {
        // Find the closing curly brace of the section
        char* end = strstr(start, "}");
        if (end != NULL) {
            // Calculate the length of the section, including the closing curly brace
            size_t section_length = end - start + strlen("}");

            // Remove the entire section concerning "AthenX (install)" from the buffer
            memmove(start, end + 1, strlen(buffer) - section_length + 1);
        }
    }
}
bool is_sector_all_zeros(uint8_t* buffer, size_t sector_size) {
    for (size_t i = 0; i < sector_size; i++) {
        if (buffer[i] != 0) {
            return false;
        }
    }
    return true;
}

typedef struct {
    int x;
    int y;
} Point_rect;

Point_rect calculateRectanglePosition(int screenWidth, int screenHeight, int rectWidth, int rectHeight) {
    Point_rect position;
    
    // Calculate the x-coordinate to center the rectangle horizontally.
    position.x = (screenWidth - rectWidth) / 2;
    
    // Calculate the y-coordinate to center the rectangle vertically.
    position.y = (screenHeight - rectHeight) / 2;
    
    return position;
}

/**
 * Function Name: install
 * Description: installs AthenX from one disk to another
 *
 * Parameters:
 *   disk_1 (int) - the disk to install from
 *   disk_2 (int) - the disk to install to
 *   size (uint32) - the size of the install
 * 
 *
 * Return:
 *   none
 */
int install(int drive_1,int drive_2,uint32_t size) {
   
     
    int sx = 100;
    int sy = 100;
    int drive_main = 0;
    int drive_second = 0;
    IMAGE file_transfer;
    file_transfer.Bpp = 8;
    file_transfer.width = 96;
    file_transfer.height = 96;
    file_transfer.x = 50+96;
    file_transfer.y = 50;
    file_transfer.ptrs.ptr = &transefer_map;
    int img_startX = 50+96; // Initial x-coordinate
    int img_endX = 900; // End x-coordinate
    uint32_t slideSpeed = 1;
//    bg_img.ptrs.ptr = &HDD_map;
    // //bg_img.Bpp = 8;
    // bg_img.width = 96;
    // bg_img.height = 96;
    // bg_img.x = x;
    // bg_img.y = y;
    //strcpy(bg_img.name, "HDD" );
    while(true)
    {
         struct NamedIndex *list = getList();
        int list_size = getListSize();
         set_screen_x(0);
        set_screen_y(0);
         printf("Select the device you want to install from\n");
        drive_main = select_drives_from_list(list,list_size);
        printf("\n\n\nDrive %d (%s) is the install medium",list[drive_main].index,list[drive_main].name);
        drawHDD(0,50,0);
        int ox1 = get_screen_x();
        int oy1 = get_screen_y();
        set_screen_y(50);
        set_screen_x(20);
        printf("Install Medium");
        set_screen_x(ox1);
        set_screen_y(oy1);
        // sleep(3);
        // beep();
        printf("                                                                                  ");
        set_screen_x(18);
        set_screen_y(0);
        printf("Select the device you want to install too \n");
        drive_second = select_drives_from_list(list,list_size);
        printf("\n\n\n\nDrive %d (%s) is the device being installed on\n",list[drive_second].index,list[drive_second].name);
        drawHDD(img_endX+60+96,50,0);
        if(drive_main != drive_second)
        {
            break;
        }
        else
        {
            printf("\nDrives cannot be the same\n");
            set_screen_x(sx);
            set_screen_y(sy);
        }
    }
    set_font_c(0,255,0); // Set font color to black
    set_background_color(0,0,0); // Set background color to white
    printf("\n");
    printf("\n");
   
    
    int total_sectors_on_source_disk = size;//size*3; 
    // Define variables for source and destination drives
    uint8 source_drive = drive_main; // Replace with the appropriate source drive number
    uint8 dest_drive = drive_second;   // Replace with the appropriate destination drive number
    
    // Specify the number of sectors to copy in one go
    uint8 num_sectors_to_copy = 1;

    // Define a buffer to hold data from the source disk
    uint32 buffer[512]; // Assuming a 512-byte sector size

    // Loop through all sectors on the source disk and copy to the destination disk
    uint32 sector;
    TIME start_time = get_time();
    int ticks = get_ticks();
    printf("install started at %d:%d:%d\n",start_time.h,start_time.m,start_time.s);
    int x = get_vesa_row();
    int y = get_vesa_col();
    char *options[] = {"Quick","Format"};
    set_screen_x(200);
    set_screen_y(400);
    int install_mode = select_install_mode(options,2);
    if(strcmp(options[install_mode], "Format") == 0)
    {
         for (sector = 0; sector < total_sectors_on_source_disk; sector++) {
            memset(buffer, 0, sizeof(buffer)); // Set the buffer to all zeros

            // Write the sector data with zeros to the destination disk
            int write_result = ide_write_sectors(dest_drive, num_sectors_to_copy, sector, buffer);
            if (write_result != 0) {
                // Handle write error
                // You can add error handling code here.
                }
            display_percentage(sector, total_sectors_on_source_disk,x,y,"Formatting");
            draw_loading_bar(sector,total_sectors_on_source_disk,100);
        }
    }
   
    x = x +30;
    memset(buffer, 0, sizeof(buffer));
    set_screen_x(get_screen_x()+28);
    
       // x = x + 18;
        set_screen_x(y);
        set_screen_y(x);
    set_screen_y(50);
    set_screen_x(38);
    int skiped_sector = 0;
    for (sector = 0; sector < total_sectors_on_source_disk; sector++) {
        // Read a sector from the source disk
        int read_result = ide_read_sectors(source_drive, num_sectors_to_copy, sector, buffer);
        if (read_result != 0) {
            // Handle read error
            // You can add error handling code here.
        }

         //printf("%s", buffer);
        char* install_found = strstr((char*)buffer, "AthenX (install)");
        if (install_found != NULL) {
            //printf("\nFound 'AthenX (install)' at sector %u!\n", sector);
            //printf("\n%s",((char*)buffer));
            remove_athenx_install_section(buffer);
        }
        else if (is_sector_all_zeros(buffer,512) == true)
        {
           skiped_sector++;
        }
        else
        {
            int write_result = ide_write_sectors(dest_drive, num_sectors_to_copy, sector, buffer);
            if (write_result != 0) {
                // Handle write error
                // You can add error handling code here.
            }
        }
       
        
        // Write the sector data to the destination disk
        // printf("\n");
        // printf("\n\n");
        // printf("\n\n");
        // set_screen_y(300);
       
        
        display_percentage(sector, total_sectors_on_source_disk,x,y,"Copying");
       // draw_loading_bar(sector,total_sectors_on_source_disk,100);
        draw_sliding_image(img_startX, img_endX, slideSpeed, &file_transfer);
    }
     printf("Skipped %d sectors\n",skiped_sector);

    // Close both disks if needed
    // You can add code here to close the disks if your operating system requires it.
    TIME end_time = get_time();
    printf("Install completed at %d:%d:%d\n",end_time.h,end_time.m,end_time.s);
    return 0;
}
// // Define constants for arrow keys and Enter key
// #define UP_ARROW_KEY 72
// #define DOWN_ARROW_KEY 80
// #define ENTER_KEY 13

// Function to display the list and allow the user to select a name
int select_drives_from_list(struct NamedIndex list[], int listSize) {
     Point_rect rectPosition = calculateRectanglePosition(vbe_get_width(),vbe_get_height(),200,100);
    int start_x = rectPosition.y+50;
    int start_y = rectPosition.x-240;
    int optionBoxX = rectPosition.x; // X-coordinate of the top-left corner of the option box
    int optionBoxY = rectPosition.y-200; // Y-coordinate of the top-left corner of the option box
    int optionBoxWidth = 200; // Width of the option box
    int optionBoxHeight = 100; // Height of the option box
    int borderWidth = 2; // Width of the border

    // Draw the border around the option box
    // for (int x = optionBoxX; x < optionBoxX + optionBoxWidth; x++) {
    //     for (int y = optionBoxY; y < optionBoxY + optionBoxHeight; y++) {
    //         // Check if the current pixel is on the border
    //         if (x < optionBoxX + borderWidth || x >= optionBoxX + optionBoxWidth - borderWidth ||
    //             y < optionBoxY + borderWidth || y >= optionBoxY + optionBoxHeight - borderWidth) {
    //             // Set the border color (you can choose a color value)
    //             vbe_putpixel(x, y, VBE_RGB(0,125,0));
    //         }
    //     }
    // }
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
        for (int i = 0; i < listSize; i++) {
            if (i == selectedIdx) {
                // Highlight the selected item
                set_font_c(0,0,0); // Set font color to black
                set_background_color(0,255,0); // Set background color to white
            } else {
                 // Unhighlight other items
                set_font_c(0,255,0); // Set font color to white
                set_background_color(0,0,0); // Set background color to black
            }
            
            display_image(list[i].name,start_x+(i*100),start_y);
            // Print the list item
            int x = get_screen_y();
            //printf("%d - %s (%s)\n", list[i].index,list[i].name,list[i].model);
            set_screen_y(x);
        }

        // Reset font and background colors for subsequent text
        set_font_color(VBE_RGB(0,255,0)); // Set font color to white
        set_background_color(0,0,0); // Set background color to black

        // Wait for a key press
        keyPressed = kb_get_scancode();

        // Check the key pressed
       // Check the key pressed
        if (keyPressed == SCAN_CODE_KEY_LEFT && selectedIdx > 0) {
            // Move selection up
            selectedIdx--;
        } else if (keyPressed == SCAN_CODE_KEY_RIGHT && selectedIdx < listSize - 1) {
            // Move selection down
            selectedIdx++;
        } else if (keyPressed == SCAN_CODE_KEY_ENTER) {
            // User pressed Enter, return the selected index
            return selectedIdx;
        }
    }
}
// Function to display a code-based image based on device name
void display_image(const char* deviceName,int x,int y) {
    // Replace this with code to display code-based images
    if (strstr(deviceName, "HARDDISK") != NULL) {
       drawHDD(x,y,0);
       int old_x = get_screen_x();
       int old_y = get_screen_y();
       set_screen_x(y-20);
       set_screen_y(x+20);
       printf("HDD");
       set_screen_x(old_x);
       set_screen_y(old_y);
       
        //printf("HARDDISK: [code-based image]\n");
    } else if (strcmp(deviceName, "SATA") == 0) {
        
        // Display code-based image for SATA (e.g., a square with the word SSD)
        //printf("SATA: [code-based image]\n");
    } else if (strstr(deviceName, "DVD-ROM") != NULL) {
        drawDVD(x,y);
        int old_x = get_screen_x();
       int old_y = get_screen_y()+100;
        set_screen_x(y-20);
       set_screen_y(x+20);
       printf("DVD-ROM");
       set_screen_x(old_x);
       set_screen_y(old_y);
        // Display code-based image for CD-ROM (e.g., a circle with smaller quarter circles)
       // printf("CD-ROM: [code-based image]\n");
    } else {
        // Handle other devices or do nothing
        //printf("%s: [unknown device]\n", deviceName);
    }
}

/**
 * Function Name: drawHDD
 * Description: Draws a crude representation of an HDD.
 *
 * Parameters:
 *   x (int) - The x-coordinate of the top-left corner of the HDD rectangle.
 *   y (int) - The y-coordinate of the top-left corner of the HDD rectangle.
 *   color (uint32) - The color to use for drawing the HDD.
 *
 * Return: None
 */
void drawHDD(int x, int y, uint32 color) {
    // Draw the HDD rectangle
    IMAGE bg_img;
    bg_img.ptrs.ptr = &HDD_map;
    bg_img.Bpp = 8;
    bg_img.width = 96;
    bg_img.height = 96;
    bg_img.x = x;
    bg_img.y = y;
    strcpy(bg_img.name, "HDD" );
    draw_low_res_img(bg_img);
}
void drawSSD(int x, int y, uint32 color) {
    // Draw the HDD rectangle
    IMAGE bg_img;
    bg_img.ptrs.ptr = &SDD_map;
    bg_img.Bpp = 8;
    bg_img.width = 96;
    bg_img.height = 96;
    bg_img.x = x;
    bg_img.y = y;
    strcpy(bg_img.name, "SSD" );
    draw_low_res_img(bg_img);
}
drawDVD(int x,int y)
{
    IMAGE bg_img;
    bg_img.ptrs.ptr = &DVD_ROM_map;
    bg_img.Bpp = 8;
    bg_img.width = 96;
    bg_img.height = 96;
    bg_img.x = x;
    bg_img.y = y;
    strcpy(bg_img.name, "DVD" );
    draw_low_res_img(bg_img);

}
// Define RGB color macros using VBE_RGB
#define RED VBE_RGB(255, 0, 0)
#define GREEN VBE_RGB(0, 255, 0)
#define BLUE VBE_RGB(0, 0, 255)

// Function to draw a loading bar with a border and percentage
void draw_loading_bar(uint32_t completed, uint32_t total, uint32_t numBlocks) {
    // Calculate the percentage of completion
    float percentage = (float)completed / total;

    // Calculate the number of blocks to display in the loading bar
    uint32_t blocksToShow = (uint32_t)(percentage * numBlocks);

    // Define block size (10x5 pixels)
    uint32_t blockSizeX = 10;
    uint32_t blockSizeY = 5;

    // Define border color
    uint32_t borderColor = BLUE; // You can choose your desired border color

    // Define the position and size of the loading bar
    uint32_t barX = 5; // X-coordinate of the loading bar
    uint32_t barY = get_screen_x()+18; // Y-coordinate of the loading bar
    uint32_t barWidth = numBlocks * blockSizeX; // Width of the loading bar
    uint32_t barHeight = blockSizeY; // Height of the loading bar

    // Draw the border around the loading bar
    for (uint32_t x = barX - 1; x < barX + barWidth + 1; x++) {
        for (uint32_t y = barY - 1; y < barY + barHeight + 1; y++) {
            if (x == barX - 1 || x == barX + barWidth || y == barY - 1 || y == barY + barHeight) {
                vbe_putpixel(x, y, borderColor);
            }
        }
    }

    // Draw the loading bar itself
    for (uint32_t i = 0; i < numBlocks; i++) {
        if (i < blocksToShow) {
            // Draw a colored block for completed portion
            for (uint32_t x = 0; x < blockSizeX; x++) {
                for (uint32_t y = 0; y < blockSizeY; y++) {
                    vbe_putpixel(barX + i * blockSizeX + x, barY + y, GREEN); // You can choose your desired color
                }
            }
        } else {
            // Draw a blank block for remaining portion
            for (uint32_t x = 0; x < blockSizeX; x++) {
                for (uint32_t y = 0; y < blockSizeY; y++) {
                    vbe_putpixel(barX + i * blockSizeX + x, barY + y, 0); // 0 represents black (no color)
                }
            }
        }
    }

    // Display the percentage at the end of the loading bar
    // set_screen_x(barWidth);
    // set_screen_y(barY);
    //printf(" %.2f%%\r", percentage * 100); // Display percentage and overwrite the previous output
    //fflush(stdout); // Flush output to ensure it's immediately displayed
}

void draw_sliding_image(int startX, int endX, uint32_t slideSpeed, IMAGE *img) {
    for (size_t i = img->x; i <img->x+1; i++)
    {
        for (size_t y = img->y; y < img->y+96; y++)
        {
            vbe_putpixel(i,y,VBE_RGB(0,0,0));
        }
        
    }
    // Draw the sliding image at its current position
    draw_low_res_img_ptr(img); // Call the function to draw the image at its position

    // Update the position of the sliding image for the next frame
    
    
    img->x += slideSpeed; // Adjust the x-coordinate to control the slide speed

    // If the image reaches the end, reset it to the start
    if (img->x >= endX) {
        for (size_t i = img->x; i <img->x+96; i++)
        {
            for (size_t y = img->y; y < img->y+96; y++)
            {
                vbe_putpixel(i,y,VBE_RGB(0,0,0));
            }
        
        }
        img->x = startX;
    }
}

int select_install_mode(char *options[],int num_options)
{
    int ret = printArrayWithSelection(options,num_options);
    return ret;
}