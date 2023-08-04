#include "rle.h"
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
//* This file handles all the commands passed to it from the main function //*

//* Main command handling
#define MAX_COMMAND_LENGTH 50
#define MAX_ARGUMENTS 10
void display_available_commands() {
    printf("\nAvailable commands:\n");
    printf("help - Display this list of commands\n");
    printf("time - Display the current time\n");
    printf("echo <message> - Print the provided message\n");
    printf("write <filename> <type> - Write content to a file\n");
    printf("read <filename> - Read content from a file\n");
    printf("ls - List files in the current directory\n");
    printf("rm <filename> - Delete a file\n");
    printf("cls - Clear the screen\n");
    printf("bg - Change background colors\n");
    printf("logo - Display a logo\n");
    printf("mkdir <directoryname> - Create a new directory\n");
    printf("exit - Update file system partition table\n");
    printf("set-xt <x> <y> - Set screen resolution\n");
    printf("song - Play a song\n");
    printf("format <disk> - Format a disk\n");
    printf("table - Display file system partition table\n");
    printf("3d - Display a 3D demo\n");
}
void parse_command(const char* command) {
    char command_copy[MAX_COMMAND_LENGTH];
    strcpy(command_copy, command);

    // Tokenize the command by spaces
    char* token;
    char* arguments[MAX_ARGUMENTS];
    int arg_count = 0;

    token = strtok(command_copy, " ");
    while (token != NULL) {
        arguments[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    // Check for different commands and execute them
    if (strcmp(arguments[0], "help") == 0) {
        if(arg_count <= 1)
        {
            display_available_commands();
            
        }
        else
        {
            printf("This feature is still coming");
        }

         
        // Implement help command logic
        // For example: display a list of available commands
    } else if (strcmp(arguments[0], "time") == 0) {
        // Implement time command logic
        // For example: display the current time
        printf("Coming soon");
    } else if (strcmp(arguments[0], "echo") == 0) {
        // Implement echo command logic
        // For example: print the following arguments
        for (int i = 1; i < arg_count; i++) {
            printf("%c", arguments[i]);
            // Print each argument separated by a space
            // You can implement this based on your output functionality
        }
    
    } 
    else if (strcmp(arguments[0], "write") == 0)
    {
        if(arg_count == 3)
        {
            char out[MAX_FILE_SIZE];
            clear_screen();
            set_screen_x(87000000);
            set_cursor_y(5656754);
            cmd_handler("cls");
            printf("\nPress ` to exit");
            printf("\nWelcome to Text:\n");
            text_editor(MAX_FILE_SIZE,out);
       //printf(out);
        //strcpy(buf, out);

            write(arguments[1], arguments[2],out);

        }
        else
        {
            printf("To many arguments");
        }
    }
    else if(strcmp(arguments[0],"read") == 0)
    {
        if(arg_count == 2)
        {
             read(arguments[1]);
        }
       
    }
    else if(strcmp(arguments[0],"ls") == 0)
    {
        list_files();
    }
    else if(strcmp(arguments[0],"rm") == 0)
    {
        delete_file(arguments[1]);
    }
    else if (strcmp(arguments[0],"cls") == 0)
    {
        set_screen_x(0);
       set_screen_y(0);
        clear_screen();
        clear_display();
    }
    else if(strcmp(arguments[0],"bg") == 0)
    {
        cool_colors();
    }
    else if(strcmp(arguments[0],"logo") == 0)
    {
         set_screen_x(0); // Set screen x position to 0;
        set_screen_y(0); // Set screen y position to 0
        clear_screen(); // Clear screen
        
        logo(0,0,320,200);
    }
    else if (strcmp(arguments[0],"mkdir") == 0)
    {
        make_dir(arguments[1]);
    }
    else if (strcmp(arguments[0],"exit") == 0)
    {
        fs_partition_table_main_update();
    }
    else if (strcmp(arguments[0],"set-xy") == 0)
    {
        const int DRIVE = 0;//ata_get_drive_by_model("QEMU HARDDISK");
        const uint32 LBA = KERNEL_SECTOR_BASE+1;
        const uint8 NO_OF_SECTORS = 1;
        char buf[ATA_SECTOR_SIZE] = {0};
        struct scree_size {
            char checksum[32];
            int x;
            int y;
        };

        struct scree_size sc_size;
        strcpy(sc_size.checksum,"True");
        sc_size.x = atoi(arguments[1]);
        sc_size.y = atoi(arguments[2]);
        memcpy(buf, &sc_size, sizeof(sc_size));
        ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);
        printf("data written\n");
    }
    else if(strcmp(arguments[0],"song") == 0)
    {
         crude_song();

    }
    else if(strcmp(arguments[0],"format") == 0)
    {
        int disk = atoi(arguments[1]);
        format_disk(disk);
    }
    else if(strcmp(arguments[0],"table") == 0)
    {
        fs_partition_table_main_p();
    }
    else if(strcmp(arguments[0],"3d") == 0)
    {
        printf("Please");
        draw_window(0,0,50);
    }
    else if(strcmp(arguments[0],"rle") == 0)
    {
        main();
        
    }
    else {
        printf("[!] %s is not a valid command\n",arguments[0]);
        // Handle invalid or unknown commands
        // For example: display an error message
    }
}
void cmd_handler(char *buffer[512])
{
    parse_command(buffer);
    return;
    


}

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