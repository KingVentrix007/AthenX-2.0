#include "editor.h"
#include "cmdhandler.h"
#include "string.h"
#include "bitmap.h"
#include "graphics.h"
#include "vesa.h"
#include "vga.h"
#include "timer.h"
#include "ide.h"
#include "console.h"
#include "speaker.h"
#include "terminal.h"
#include "vesa.h"
#include "display.h"
#include "keyboard.h"
#include "fs.h"
///* This file handles all the commands passed to it from the main function //*

// Function: cmd_handler
// Description: Handles various commands passed to it and executes corresponding actions.
// Parameters:
//      - buffer: The command buffer containing the command to be processed.
// Return: None
void cmd_handler(char *buffer[512])
{
    // Command: "bg"
    // Description: Change the background color of the terminal to a cool pattern.
    if (strcmp(buffer, "bg") == 0)
    {
        cool_colors();
    }

    // Command: "cls"
    // Description: Clear the terminal screen.
    else if (strcmp(buffer, "cls") == 0)
    {
        set_screen_x(0);
        set_screen_y(0);
        clear_screen();
    }

    // Command: "write(filename)"
    // Description: Write data to a file with the specified filename.
    // Parameters:
    //      - filename: The name of the file to be written to (up to 8 characters).
    // Return: None
    else if (strlen(buffer) > 0 && strstr(buffer, "write(") != NULL)
    {
        // ... (details omitted for brevity)
    }

    // Command: "read(filename)"
    // Description: Read the contents of a file with the specified filename.
    // Parameters:
    //      - filename: The name of the file to be read (up to 8 characters).
    // Return: None
    else if (strlen(buffer) > 0 && strstr(buffer, "read(") != NULL)
    {
        // ... (details omitted for brevity)
    }

    // Command: "list"
    // Description: List all files in the current directory.
    else if (strcmp(buffer, "list") == 0)
    {
        printf("\n");
        list_files();
    }

    // Command: "mkdir(dirname)"
    // Description: Create a new directory with the specified name.
    // Parameters:
    //      - dirname: The name of the directory to be created (up to 8 characters).
    // Return: None
    else if (strstr(buffer, "mkdir(") != NULL)
    {
        // ... (details omitted for brevity)
    }

    // Command: "exit"
    // Description: Update the master table and exit the program.
    else if (strcmp(buffer, "exit") == 0)
    {
        fs_master_table_update();
    }

    // Command: "set-xy"
    // Description: Set the screen size (X and Y resolution) using the VESA BIOS Extensions.
    else if (strcmp(buffer, "set-xy") == 0)
    {
        // ... (details omitted for brevity)
    }

    // Command: "song"
    // Description: Play a simple song using the speaker.
    else if (strcmp(buffer, "song") == 0)
    {
        crude_song();
    }

    // Command: "modes"
    // Description: Print available VESA video modes.
    else if (strcmp(buffer, "modes") == 0)
    {
        printf("Broken");
        // clear_screen();
        // //set_vesa_row(0);
        // //set_vesa_column(0);
        // vbe_print_available_modes();
    }

    // Command: "format disk"
    // Description: Format the disk by writing the format table and initializing the master table.
    else if (strcmp(buffer, "format disk") == 0)
    {
        format_disk();
    }

    // Command: "table"
    // Description: Print the master table for debugging purposes.
    else if (strcmp(buffer, "table") == 0)
    {
        fs_master_table_p();
    }

    // Command: "rm(filename)"
    // Description: Delete a file with the specified filename.
    // Parameters:
    //      - filename: The name of the file to be deleted (up to 8 characters).
    // Return: None
    else if (strstr(buffer, "rm(") != NULL)
    {
        // ... (details omitted for brevity)
    }

    // Command: Unrecognized
    // Description: Print an error message for unrecognized commands.
    else
    {
        printf("\n%s is not a recognized command", buffer);
    }
}

// Function: kernel_command_handler
// Description: Handles various kernel-level commands.
// Parameters:
//      - buffer: The command buffer containing the command to be processed.
// Return: None
void kernel_command_handler(char *buffer[512])
{
    if(strcmp(buffer,"login set")==0)
    {
        //beep();
        const int DRIVE = 0;//ata_get_drive_by_model("QEMU HARDDISK");
        const uint32 LBA = 1;
        const uint8 NO_OF_SECTORS = 1;
        char buf[ATA_SECTOR_SIZE] = {0};
        struct login_info
        {
            char checksum[32];
            char username_stored[32];
            char password_stored[32];
        };
        struct login_info login;
        strcpy(login.username_stored, "tristan");
        strcpy(login.password_stored, "123");
        strcpy(login.checksum,"True");

        //strcpy(buf, "hello world!");
        //ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);

        memset(buf, 0, sizeof(buf));
        memcpy(buf, &login, sizeof(login));
        ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);
        printf("\nlogin set\n");
        printf("Username: %s\n", login.username_stored);
        printf("Password: %s\n", login.password_stored);
        // printf("data written\n");

        // read message from drive
        // memset(buf, 0, sizeof(buf));
        // ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);
        // printf("read data: %s\n", buf);

        // memset(buf, 0, sizeof(buf));
        // ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA + 3, (uint32)buf);
        // memcpy(&e, buf, sizeof(e));
        // printf("id: %d, name: %s\n", e.id, e.name);

    }
}

// void format_disk()
// {
//     run_once();
// }