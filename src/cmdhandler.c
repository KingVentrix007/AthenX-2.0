#include "pci.h"
#include "printf.h"
#include "ext2.h"
#include "debug.h"
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
#include "serial.h"
//* This file handles all the commands passed to it from the main function //*

//* Main command handling
#define MAX_COMMAND_LENGTH 50
#define MAX_ARGUMENTS 10
void display_available_commands() {
    printf_("\nAvailable commands:\n");
    printf_("help - Display this list of commands\n");
    printf_("time - Display the current time\n");
    printf_("echo <message> - Print the provided message\n");
    printf_("write <filename> <type> - Write content to a file\n");
    printf_("read <filename> - Read content from a file\n");
    printf_("ls - List files in the current directory\n");
    printf_("rm <filename> - Delete a file\n");
    printf_("cls - Clear the screen\n");
    printf_("bg - Change background colors\n");
    printf_("logo - Display a logo\n");
    printf_("mkdir <directoryname> - Create a new directory\n");
    printf_("exit - Update file system partition table\n");
    printf_("set-xt <x> <y> - Set screen resolution\n");
    printf_("song - Play a song\n");
    printf_("format <disk> - Format a disk\n");
    printf_("table - Display file system partition table\n");
    printf_("3d - Display a 3D demo\n");
    printf_("rle - run length encoding tests\n");
    printf_("exe <filename> - run executable file\n");
    printf_("edit <filename> - edit a file\n");
}
void parse_command(const char* command) {
    FUNC_ADDR_NAME(&parse_command);
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
            printf_("This feature is still coming");
        }

         
        // Implement help command logic
        // For example: display a list of available commands
    } else if (strcmp(arguments[0], "time") == 0) {
        // Implement time command logic
        // For example: display the current time
        printf_("Coming soon");
    } else if (strcmp(arguments[0], "echo") == 0) {
        // Implement echo command logic
        // For example: print the following arguments
        for (int i = 1; i < arg_count; i++) {
            printf_("%c", arguments[i]);
            // Print each argument separated by a space
            // You can implement this based on your output functionality
        }
    
    } 
    else if (strcmp(arguments[0],"com") == 0)
    {
        char out = "C";
        printf_("\nCOM output: ");
        out = read_serial();
        printf_("C%c", out);
        // while(out != NULL)
        // {
        //     out = read_serial();
        //     printf_("%c", out);
        // }
         
       
    }
    else if (strcmp(arguments[0], "pci") == 0)
    {
        printf("\n");
        if(strcmp(arguments[1], "list") == 0)
        {
            list_pci_devices();
        }
        else if(strcmp(arguments[1], "reg") == 0)
        {
            list_registered_device();
        }
        //list_pci_devices();
        //printf("list");
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
            printf_("\nPress ` to exit");
            printf_("\nWelcome to Text:\n");
            int size = text_editor(MAX_FILE_SIZE,out);
            write_fs();
       //printf_(out);
        //strcpy(buf, out);

            //write_file(0,arguments[1], out);
            
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
        //char *cwd = kmalloc(sizeof(*cwd)); //Current working directory, in it's path
        char *cwd = "/";
        uint32 ino = ext2_path_to_inode(cwd);
        char **names = ext2_ls(ino);
        printf_("\n");
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
        printf_("data written\n");
    }
    else if(strcmp(arguments[0],"song") == 0)
    {
         crude_song();

    }
    else if(strcmp(arguments[0],"format") == 0)
    {
        int disk = atoi(arguments[1]);
        format_disk();
        //format_drive(disk);
    }
    else if(strcmp(arguments[0],"table") == 0)
    {
        fs_partition_table_main_p();
    }
    else if(strcmp(arguments[0],"3d") == 0)
    {
        //printf_("Please");
        ArtemisVision();
    }
    else if(strcmp(arguments[0],"rle") == 0)
    {
        main();
        
    }
    else if(strcmp(arguments[0],"fs") == 0)
    {
        clear_display();
        // printf_("SUPERBLOCK: %d\n",sizeof(SUPERBLOCK));
        // printf_("INODE_FILE: %d\n",sizeof(INODE_FILE));
        // printf_("INODE_DIR: %d\n",sizeof(INODE_DIR));
        // printf_("DATA_BLOCK: %d\n",sizeof(DATA_BLOCK));
        // write_superblock(512);
        // printf_("CONE_BLOCK\n");
        // read_superblock_al(512);
        // printf_("Done!\n");
        format_disk(0);
        
      

    }
    else if (strcmp(arguments[0],"exe") == 0)
    {
        char output[1024];
        read_add(arguments[1],output);
        cmd_handler(output);

    }
    
    else if(strcmp(arguments[0],"sect") == 0)
    {
        char myArray[1024] = {0}; // This sets all elements to 0, change this pattern if desired

    // Access and modify elements in the array
        for (int i = 0; i < 1024; i++) {
        // You can set the elements to any value you want, for example:
            myArray[i] = i % 256; // This sets each element with values from 0 to 255 in a repeating pattern
         }
        printf_("\n%c", myArray);
        ide_write_sectors(0,2,60,(uint32)myArray);
        char outb[1024] = {0};
        ide_read_sectors(0,2,60,(uint32)outb);
        printf_("\n%c", outb);

    }
    else if(strcmp(arguments[0],"edit") == 0)
    {
        char data[1024] = {0};
        memset(data,0,sizeof(data));
        read_add(arguments[1],data);
        printf_("\n%c",data);
        text_editor(1024,data);
    }
    else {
        printf_("{/330:255,0,0}{/331:0,200,0}");
        printf("\n[!] %s is not a valid command",arguments[0]);
        printf_("{/330:0,255,0}{/331:0,0,0}");
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
        printf_("\nlogin set\n");
        printf_("Username: %s\n", login.username_stored);
        printf_("Password: %s\n", login.password_stored);
        // printf_("data written\n");

        // read message from drive
        // memset(buf, 0, sizeof(buf));
        // ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);
        // printf_("read data: %s\n", buf);

        // memset(buf, 0, sizeof(buf));
        // ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA + 3, (uint32)buf);
        // memcpy(&e, buf, sizeof(e));
        // printf_("id: %d, name: %s\n", e.id, e.name);

    }
}

// void format_disk()
// {
//     run_once();
// }