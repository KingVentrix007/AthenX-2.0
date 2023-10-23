#include "../include/elf_exe.h"
#include "exe.h"
#include "../include/fat_filelib.h"
#include "../include/installer.h"
#include "../include/kheap.h"
#include "../include/isr.h"
#include "../include/stdio.h"
#include "../include/errno.h"
#include "../include/pci.h"
#include "../include/printf.h"
// #include "../include/ext2.h"
#include "../include/debug.h"
#include "../include/rle.h"
#include "../include/editor.h"
#include "../include/cmdhandler.h"
#include "../include/string.h"
#include "../include/bitmap.h"
#include "../include/graphics.h"
#include "../include/vesa.h"
#include "../include/vga.h"
#include "../include/timer.h"
#include "../include/ide.h"
#include "../include/console.h"
#include "../include/speaker.h"
#include "../include/terminal.h"
#include "../include/vesa.h"
#include "../include/display.h"
#include "../include/keyboard.h"
#include "../include/fs.h"
#include "../include/serial.h"
#include "../include/man.h"
#include "../include/ssfs.h"
#include "../include/tui.h"

//* This file handles all the commands passed to it from the main function //*

//* Main command handling
#define MAX_COMMAND_LENGTH 50
#define MAX_ARGUMENTS 10
char cwd[FATFS_MAX_LONG_FILENAME] = "/root/";
void display_available_commands(const char* cmd) {
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
    printf("rle - run length encoding tests\n");
    printf("exe <filename> - run executable file\n");
    printf("edit <filename> - edit a file\n");
    printf("new format\n-----------------------------------------------\n");
    
}

void man(const char* cmd)
{
    printf("\n");
    man_main(cmd);
}


int set_cwd(char *s_cwd)
{
    //cwd = s_cwd;
    strcpy(cwd,s_cwd);
}
void removeFirstNChars(char* str, int n) {
    int length = strlen(str);

    // Check if n is greater than the length of the string
    if (n >= length) {
        // If n is greater or equal to the length of the string,
        // the result will be an empty string.
        str[0] = '\0';
    } else {
        // Use memmove to shift the remaining characters to the beginning
        memmove(str, str + n, length - n + 1);
    }
}
int search_program(const char (*list)[20], int list_size, const char* target) {
    for (int i = 0; i < list_size; i++) {
        if (strcmp(list[i], target) == 0) {
            return i;  // Match found, return the index
        }
    }
    return -1;  // Match not found
}

void parse_command(const char* command, char programs[MAX_PROGRAMS][20]) {
    FUNC_ADDR_NAME(&parse_command,1,"s");
    char command_copy[MAX_COMMAND_LENGTH];
    strcpy(command_copy, command);

    // Tokenize the command by spaces
    char* token;
     char* saveptr;
    char* arguments[MAX_ARGUMENTS];
    char* sub_args[MAX_ARGUMENTS];
    int sub_arg_count = 0;
    int arg_count = 0;
    int in_quote = 0;
    token = strtok(command_copy, " ");
   while (token != NULL) {
        if (arg_count >= MAX_ARGUMENTS) {
            printf("Too many arguments!\n");
            break;
        }

        // Check if it's an argument or sub-argument
        if (token[0] == '-' && token[1] == '-') {
            sub_args[sub_arg_count++] = token;
        } else {
            arguments[arg_count++] = token;
        }

        token = strtok(NULL, " ");
    }
    // printf("\nArgs: \n");
    // for (size_t i = 0; i < arg_count; i++)
    // {
    //     printf("%s\n",arguments[i]);
    // }
    // printf("Sub args %d:\n",sub_arg_count);
    // for (size_t q = 0; q < sub_arg_count; q++)
    // {
    //     printf("%s\n",sub_args[q]);
    // }
    if(sub_arg_count >= 1)
    {
        if(strcmp(sub_args[0],"--help") == 0)
        {
            man(arguments[0]);
        }
    }
    
    // Check for different commands and execute them
    else if (strcmp(arguments[0], "help") == 0) {
        if(arg_count <= 1)
        {
            display_available_commands("");
            
        }
        else
        {
            display_available_commands(arguments[1]);
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
    else if (strcmp(arguments[0],"com") == 0)
    {
        char out = "C";
        printf("\nCOM output: ");
        out = read_serial();
        printf("C%c", out);
        // while(out != NULL)
        // {
        //     out = read_serial();
        //     printf("%c", out);
        // }
         
       
    }
    else if (strcmp(arguments[0],"man") == 0)
    {
        if(arg_count >= 2)
        {
            man(arguments[1]);
        }
        else
        {
            clear_screen();
            man_list();
        }
        
    }
    
    else if (strcmp(arguments[0], "inspect") == 0)
    {
        if(strcmp(arguments[1],"err") == 0)
        {

            inspect_errors();
        }
        else if (strcmp(arguments[1],"drive") == 0)
        {
             
            removeFirstNChars(sub_args[0],2);
            int drive = atoi(sub_args[0]);
            char buffer[512];
            for (size_t i = 0; i < get_sectors(drive); i++)
            {

                memset(buffer,0,sizeof(buffer));
                ide_read_sectors(drive,1,i,(uint32)buffer);
                printf("\nout put\n: %s\n",buffer);
            }
           
        }
        
        else if(strcmp(arguments[1],"log") == 0)
        {
            if(strcmp(sub_args[0],"--all") == 0)
            {
                printf("\n");
                print_events();
            }
            else if (strcmp(sub_args[0],"--find") == 0)
            {
                TIME startTime, endTime;
                bool isError;
                const char specificCode[MAX_STRING_LENGTH] = ""; 
                const char functionName[MAX_STRING_LENGTH]; 
                printf("Enter start time (hh mm ss yyyy mm dd): ");
                scanf("%d %d %d %d %d %d", &startTime.h, &startTime.m, &startTime.s, &startTime.y, &startTime.mo, &startTime.d);
                while (get_chr() != '\n');
                printf("\nEnter end time (hh mm ss yyyy mm dd): ");
                scanf("%d %d %d %d %d %d", &endTime.h, &endTime.m, &endTime.s, &endTime.y, &endTime.mo, &endTime.d);
                printf("%d",endTime.h);
                while (get_chr() != '\n');
                printf("\nEnter error status (0 for false, 1 for true): ");
                scanf("%d", &isError);

                printf("\nEnter specific function name (or leave blank for any): ");
                scanf("%s", functionName);
                filterAndPrintEvents(&startTime,&endTime,isError,functionName);

            }
            
            
        }
        
        
        else if (strcmp(arguments[1],"pci") == 0)
        {
            if(strcmp(sub_args[0],"--list") == 0)
            {
                list_pci_devices();
            }
            if(strcmp(sub_args[0],"--find") == 0 && sub_args[1] != NULL)
            {
                printf("HERE");
            }
            
        }
        else if (strcmp(arguments[1],"stack") == 0)
        {
            printf("\n");
            
            //TraceStackTrace(1000,0);
        }
        
        else if (strcmp(arguments[1],"drivers") == 0 )
        {
            
            if(strcmp(sub_args[0],"--all") == 0)
            {
                 list_registered_device();
            }
            else if(strcmp(sub_args[0],"--live") == 0)
            {
                list_running_devices();
            }
           
        }
        // else if (strcmp(arguments[1],"test") == 0)
        // {
        //     char b[MAX_STRING_LENGTH];
        //     scanf("%s",b);
        //     printf("%s",b);
        // }
        
        
        
        
    }
    else if (strcmp(arguments[0],"install") == 0)
        {
            if(arg_count >= 3)
            {
                //install(atoi(arguments[1]),atoi(arguments[2]),ISO_FILE_SIZE_IN_SECTORS);
            }
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
    else if (strcmp(arguments[0],"load") == 0)
    {
       //load_exe_elf("/sys/test_2");
       if(arg_count == 1)
       {
        char* file_path = "/sys/shell"; // Replace with the path to your binary or ELF file
        int *ret;
        printf("\n");
        int myArgc = 3;
        char* myArgv[] = {"Tristan", "/root/", "arg2"};

        load_elf_file(file_path,myArgc,myArgv);
        
       }else if (arg_count >= 2)
       {
         printf("\n");
         int myArgc = 3;
         char* myArgv[] = {"Tristan", "/root/", "arg2"};
         load_elf_file(arguments[1],arg_count,arguments);
       }
       
        
        //printf("\nloaded\n");
        //process_execute(file_path,arg_count,arguments,&ret);
        //return ret;
    }
    
    
    else if (strcmp(arguments[0], "fim") == 0)
    {
        if(arg_count >= 2)
        {
            char out[MAX_FILE_SIZE];
            set_font_c(0,0,0);
            set_background_color(255,255,255);
            //memset(out,0,sizeof(out));
            clear_screen();
            set_screen_x(0);
            set_cursor_y(0);
            //cmd_handler("cls");
            
            printf("\nPress ENTER to exit");
            printf("\nWelcome to Text:\n");
            int bufferSize = 0;
            char path[FATFS_MAX_LONG_FILENAME] = "";
            strcat(path,cwd);
            strcat(path,"/");
            strcat(path,arguments[1]);
            //printf("HERE\n");
            fim(out,path);
    //         int len = text_editor(out,bufferSize );

    //         //printf("LEN->%d",len);
    //         //// printf("len = %d\n",len);
    //         //scanf("%s",out);
    //    //printf(out);
    //     //strcpy(buf, out);
    //         //printf("out->%s",out);
    //         FILE *file;
    //         char path[FATFS_MAX_LONG_FILENAME] = "";
    //         strcat(path,cwd);
    //         strcat(path,"/");
    //         strcat(path,arguments[1]);
    //         printf("\n");
    //         printf("Writing file at: %s\n",path);
    //         file = fl_fopen(path,"ab");
    //         size_t dataLength = 0;
    //         while (out[dataLength] != '\0') {
    //             dataLength++;
    //     }
    //         size_t elementsWritten = fl_fwrite(out, sizeof(char), dataLength, file);
    //          if (elementsWritten != dataLength) {
    //             printf("Error");
    //         // Failed to write all data to the file
    //          fclose(file); // Close the file before returning
    //         return -1;
    //     }

    // Close the file
        //fclose(file);
            //fl_fwrite();
            //fprintf()
            // if(file == NULL)
            // {
            //     printf("ERROR reading file\n");
            //     return -1;
            // }
            // char ch;
            // while ((ch = fgetc(file)) != EOF) {
            //     printf("%c",ch); // Print each character
            // }
            
            printf("\n");

        }
        else
        {
            printf("To many arguments");
        }
    }
    else if(strcmp(arguments[0],"cat") == 0)
    {
       
        char path[FATFS_MAX_LONG_FILENAME] = "";
        strcat(path,cwd);
        strcat(path,"/");
        strcat(path,arguments[1]);
        printf("\n");
        printf("Reading file at: %s\n",path);
        clear_display();
        clear_screen();
        set_screen_x(0);
        set_screen_y(0);
        displayFileContents(path);
        // if(file == NULL)
        // {
        //     printf("ERROR reading file\n");
        //     return -1;
        // }
        // char ch;
        // while ((ch = fgetc(file)) != EOF) {
        //     printf("%c",ch); // Print each character
        // }
        // fclose(file);
        printf("\n");
        
       
    }
    else if(strcmp(arguments[0],"pwd") == 0)
    {
        printf("%s\n",cwd);
    }
    else if (strcmp(arguments[0],"touch") == 0)
    {
        char path[FATFS_MAX_LONG_FILENAME] = "";
        strcat(path,cwd);
        strcat(path,"/");
        strcat(path,arguments[1]);
        FILE* file = fl_fopen(path, "a");

        if (file == NULL) {
        perror("Failed to open file"); // Print an error message if the file can't be opened
        return 1; // Return an error code
        }
    }
    
    else if(strcmp(arguments[0],"ls") == 0)
    {
        //char *cwd = kmalloc(sizeof(*cwd)); //Current working directory, in it's path
        // fl_listdirectory("/");
        // fl_listdirectory("rootfs");
        // fl_listdirectory("rootfs/");
        // fl_listdirectory("/rootfs");
        // fl_listdirectory(cwd);
        
    }
    
    else if(strcmp(arguments[0],"cd") == 0)
    {
        if (arg_count >= 2) {
    if (strcmp(arguments[1], "..") == 0) {
        // Check if the current directory is the rootfs directory
        if (strcmp(cwd, "root") == 0 || strcmp(cwd, "/root/") == 0) {
            // Allow going one level above rootfs
            strcpy(cwd, "/");
        } else if (strstr(cwd, "/") != 0) {
            // Find the last '/' character in the current directory path
            char* last_slash = strrchr(cwd, '/');
            
            if (last_slash != NULL) {
                //printf("Here\n");
                // Remove the last directory from the current path
                *last_slash = '\0';
                char* last_slash = strrchr(cwd, '/');
            
                if (last_slash != NULL) {
                    //printf("Here23\n");
                    // Remove the last directory from the current path
                    *last_slash = '\0';
                    
                    }
            } else {
                // If there is no '/', set the current directory to rootfs
                strcpy(cwd, "root");
            }
        }
        if(cwd[strlen(cwd)-1] == '/' && cwd[strlen(cwd)-2] == '/')
        {
            cwd[strlen(cwd)-1] = '\0';
        }
        if(cwd[strlen(cwd)-1] != '/')
        {
            strcat(cwd,"/");
        }
    } else {
        char tmp[FATFS_MAX_LONG_FILENAME];
        strcpy(tmp,cwd);
        if(tmp[strlen(tmp)-1] != '/')
        {
             strcat(tmp, "/");
        }
       
        strcat(tmp, arguments[1]);
        
        if (fl_is_dir(tmp) == 1) {
            strcpy(cwd, tmp);
        } else {
            printf("\n%s is not a valid path\n", tmp);
        }
        if(cwd[strlen(cwd)-1] == '/' && cwd[strlen(cwd)-2] == '/')
        {
            DEBUG("");
            cwd[strlen(cwd)-1] = '\0';
        }
        if(cwd[strlen(cwd)-1] != '/')
        {
            strcat(cwd,"/");
        }
    }
}
    }
    else if(strcmp(arguments[0],"rm") == 0)
    {
        char path[FATFS_MAX_LONG_FILENAME] = "";
        strcat(path,cwd);
        strcat(path,"/");
        strcat(path,arguments[1]);
        remove(path);
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
        char tmp[FATFS_MAX_LONG_FILENAME];
        strcpy(tmp,cwd);
        strcat(tmp,arguments[1]);
        if(fl_is_dir(tmp) == 0)
        {
            mkdir(tmp);
        }
        else
        {
            printf("Dictionary exists\n");
        }
    }
    else if (strcmp(arguments[0],"exit") == 0)
    {
        //fs_partition_table_main_update();
        ssfs_update_table();
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
        if(arg_count == 3)
        {
              int disk = atoi(arguments[1]);
              char name[100];
            strcpy(name,arguments[2]);
            ssfs_format_disk(0,name);
        }
        else
        {
            printf("invalid parameters");
        }
      
        //format_drive(disk);
    }
    else if(strcmp(arguments[0],"table") == 0)
    {
        fs_partition_table_main_p();
    }
    else if(strcmp(arguments[0],"3d") == 0)
    {
        //printf("Please");
        ArtemisVision();
    }
    else if(strcmp(arguments[0],"rle") == 0)
    {
        //main();
        
    }
    else if(strcmp(arguments[0],"fs") == 0)
    {
        clear_display();
        // printf("SUPERBLOCK: %d\n",sizeof(SUPERBLOCK));
        // printf("INODE_FILE: %d\n",sizeof(INODE_FILE));
        // printf("INODE_DIR: %d\n",sizeof(INODE_DIR));
        // printf("DATA_BLOCK: %d\n",sizeof(DATA_BLOCK));
        // write_superblock(512);
        // printf("CONE_BLOCK\n");
        // read_superblock_al(512);
        // printf("Done!\n");
        format_disk(0);
        
      

    }
    else if (strcmp(arguments[0],"exe") == 0)
    {
        char output[1024];
        read_add(arguments[1],output);
        

    }
    
    else if(strcmp(arguments[0],"raw") == 0)
    {
        if(strcmp(arguments[1],"read") == 0)
        {
            if(arg_count == 4)
            {
                 int drive = atoi(arguments[2]);
                 char b[100];
                char buffer[512] = {0};
                memset(buffer,0,sizeof(buffer));
                int lba = atoi(arguments[3]);
                ide_read_sectors(drive,1,lba,(uint32)buffer);
                printf("\nOutput:\n%s",buffer);
                // for (size_t i = 0; i < strlen("Memory Configuration")+1; i++)
                //     {
                //             append(b,buffer[i]);
                //     }
                //         printf("\nIUT:\n%s",b);
            }
            else if (arg_count >= 5)
            {
               
                 int drive = atoi(arguments[2]);
                char buffer[512] = {0};
                char b[100];
                memset(buffer,0,sizeof(buffer));
                int lba_start = atoi(arguments[3]);
                int lba_end = atoi(arguments[4]);
                for (size_t lba = lba_start; lba < lba_end; lba++)
                {
                       ide_read_sectors(drive,1,lba,(uint32)buffer);
                        printf("%s",buffer);
                        for (size_t i = 0; i < strlen("Memory Configuration"); i++)
                        {
                            append(b,buffer[i]);
                        }
                        printf("\nIUT:\n%s",b);
                        
                        memset(buffer,0,sizeof(buffer));
                        
                }
                
             
            }
            
           

        }
        else if (strcmp(arguments[1],"write") == 0)
        {
            char *buffer = kmalloc(atoi(arguments[3])*8);
            
            //memset(buffer,0,sizeof(buffer)); 
            printf("Enter data:\n");
            scanf("%s",buffer);
            
            int num_sectors = (strlen(buffer)/512)+1; 
            int lba = atoi(arguments[2]);
            ide_write_sectors(0,num_sectors,lba,(uint32)buffer);
            //printf("\nOutput:\n%s",buffer);
        }
        
    }
    else if(strcmp(arguments[0],"edit") == 0)
    {
        char data[1024] = {0};
        memset(data,0,sizeof(data));
        read_add(arguments[1],data);
        printf("\n%c",data);
        text_editor(1024,data);
    }
    
    else {
        int index = search_program(programs,2,arguments[0]);
        if(index != -1)
        {
            char tmp[20] = "/sys/";
            strcat(tmp,programs[index]);
            load_elf_file(tmp,arg_count,arguments);
        }
        else
        {
            printf_("{/330:255,0,0}{/331:0,200,0}");
            printf("\n[!] %s is not a valid command",arguments[0]);
            printf_("{/330:0,255,0}{/331:0,0,0}");
        }
        
        // Handle invalid or unknown commands
        // For example: display an error message
    }
}
void cmd_handler(char *buffer[512],char programs[MAX_PROGRAMS][20])
{
    parse_command(buffer,programs);
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
char *get_cwd()
{
    return cwd;
}
// void format_disk()
// {
//     run_once();
// }