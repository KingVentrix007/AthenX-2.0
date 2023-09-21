#include "timer.h"
#include "installer.h"
#include "types.h"
#include <stddef.h>
#include "string.h"
#include "ide.h"
#include "vesa_display.h"
#include "printf.h"
#include "keyboard.h"
#include "display.h"

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
    int sx = get_screen_x();
    int sy = get_screen_y();
    int drive_main = 0;
    int drive_second = 0;
    while(true)
    {
         struct NamedIndex *list = getList();
        int list_size = getListSize();
         printf("Select the device you want to install from\n");
        drive_main = select_drives_from_list(list,list_size);
        printf("\n\n\nDrive %d (%s) is the install medium",list[drive_main].index,list[drive_main].name);
        sleep(3);
        beep();
        printf("                                                                                  ");
        set_screen_x(sx);
        set_screen_y(sy);
        printf("Select the device you want to install too \n");
        drive_second = select_drives_from_list(list,list_size);
        printf("\n\n\n\nDrive %d (%s) is the device being installed on\n",list[drive_second].index,list[drive_second].name);
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
    int x = get_vesa_row();
    int y = get_vesa_col();
    for (sector = 0; sector < total_sectors_on_source_disk; sector++) {
        memset(buffer, 0, sizeof(buffer)); // Set the buffer to all zeros

        // Write the sector data with zeros to the destination disk
        int write_result = ide_write_sectors(dest_drive, num_sectors_to_copy, sector, buffer);
        if (write_result != 0) {
            // Handle write error
            // You can add error handling code here.
        }
         display_percentage(sector, total_sectors_on_source_disk,x,y,"Formatting");
    }
    x = x +17;
    memset(buffer, 0, sizeof(buffer));
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
        if (is_sector_all_zeros(buffer,512) == true)
        {
           printf(".");
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
        
        display_percentage(sector, total_sectors_on_source_disk,x,y,"Copying");
    }

    // Close both disks if needed
    // You can add code here to close the disks if your operating system requires it.

    return 0;
}
// // Define constants for arrow keys and Enter key
// #define UP_ARROW_KEY 72
// #define DOWN_ARROW_KEY 80
// #define ENTER_KEY 13

// Function to display the list and allow the user to select a name
int select_drives_from_list(struct NamedIndex list[], int listSize) {
    int start_x = get_screen_x()+5;
    int start_y = get_screen_y()+4;
    int optionBoxX = start_y-4; // X-coordinate of the top-left corner of the option box
    int optionBoxY = start_x-5; // Y-coordinate of the top-left corner of the option box
    int optionBoxWidth = 200; // Width of the option box
    int optionBoxHeight = 100; // Height of the option box
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

            // Print the list item
            int x = get_screen_y();
            printf("%d - %s (%s)\n", list[i].index,list[i].name,list[i].model);
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
        } else if (keyPressed == SCAN_CODE_KEY_DOWN && selectedIdx < listSize - 1) {
            // Move selection down
            selectedIdx++;
        } else if (keyPressed == SCAN_CODE_KEY_ENTER) {
            // User pressed Enter, return the selected index
            return selectedIdx;
        }
    }
}
