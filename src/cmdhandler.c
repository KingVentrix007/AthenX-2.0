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
void cmd_handler(char *buffer[512])
{
    if(strcmp(buffer, "bg") == 0)
    {
        cool_colors();
    }
    else if (strcmp(buffer,"cls") == 0)
    {
       //vbe_putpixel(100,100,VBE_RGB(0,255,0));
       set_screen_x(0);
       set_screen_y(0);
        clear_screen();
       //cool_colors();
        
    }
    else if(strlen(buffer) > 0 && strstr(buffer,"write(") != NULL)
    {
        
        char *parser;
        char string[32];
        parser = strstr(buffer, "write(");
        parser += strlen("write(");
        parse_string(string, parser, ')');
        const int DRIVE = 0;//ata_get_drive_by_model("QEMU HARDDISK");
        //const uint32 LBA = atoi(string);
        
       
        const uint8 NO_OF_SECTORS = 1;
        char buf[ATA_SECTOR_SIZE] = {0};
      
        
        char out[512];
       printf("\nText:");
       text_editor(512,out);
       //printf(out);
        strcpy(buf, out);

        write(string, "txt",out);
        // ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);
        // printf("\nLBA: %d\n",LBA);
        // memset(buf, 0, sizeof(buf));
        // memcpy(buf, &e, sizeof(e));
        // ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA + 1, (uint32)buf);
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
    else if (strlen(buffer) > 0 && strstr(buffer,"read(") != NULL)
    {
        char *parser;
        char string[1];
        parser = strstr(buffer, "read(");
        parser += strlen("read(");
        parse_string(string, parser, ')');
        const int DRIVE = 0;//ata_get_drive_by_model("QEMU HARDDISK");
        //const uint32 LBA = atoi(string);
        const uint8 NO_OF_SECTORS = 1;
        char buf[ATA_SECTOR_SIZE] = {0};

        memset(buf, 0, sizeof(buf));
        read(string);
        //ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);
        //printf("read data: %s\n", buf);

       
    }
    else if (strcmp(buffer,"list") == 0)
    {
        printf("\n");
        list_files();
    }
    else if (strstr(buffer,"mkdir(") != NULL)
    {
        char *parser;
        char string[32];
        parser = strstr(buffer, "write(");
        parser += strlen("write(");
        parse_string(string, parser, ')');
        const int DRIVE = 0;//ata_get_drive_by_model("QEMU HARDDISK");
        //const uint32 LBA = atoi(string);
        
       
        const uint8 NO_OF_SECTORS = 1;
        char buf[ATA_SECTOR_SIZE] = {0};
      
        
        char out[512];
       printf("\nText:");
       text_editor(512,out);
       //printf(out);
        strcpy(buf, out);

        write(string, "txt", out);
    }
    // else if (strcmp(buffer,"write1!") == 0)
    // {
    //     uint32 l;
    //     l = write("food", "eat food");
    //     printf("L: %d\n",l);
    // }
    // else if (strcmp(buffer,"read1!") == 0)
    // {
    //     uint32 l;
    //     l = read("food");
    //     //printf("L: %d\n",l);
    // }
    
    else if(strcmp(buffer,"exit") == 0)
    {
        fs_master_table_update();
    }
    
    else if (strcmp(buffer,"set-xy") == 0)
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
        sc_size.x = 1280;
        sc_size.y = 1024;
        memcpy(buf, &sc_size, sizeof(sc_size));
        ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);
        printf("data written\n");
    }
    // else if (strcmp(buffer,"text") == 0)
    // {
    //    char out[512];
    //    printf("\nText:");
    //    text_editor(512,out);
    //    printf(out);
    // }
    
    else if (strcmp(buffer, "song") == 0)
    {
        crude_song();
    }
    else if (strcmp(buffer,"modes") == 0)
    {
        clear_screen();
        set_vesa_row(0);
        set_vesa_colum(0);
        vbe_print_available_modes();
    }
    else if(strcmp(buffer,"format disk") == 0)
    {
        format_disk();
    }
    else if (strcmp(buffer,"table") == 0)
    {
        fs_master_table_p();
    }
    
    else if (strstr(buffer,"rm(") != NULL) 
    {
        char *parser;
        char string[1];
        parser = strstr(buffer, "rm(");
        parser += strlen("rm(");
        parse_string(string, parser, ')');
        const int DRIVE = 0;//ata_get_drive_by_model("QEMU HARDDISK");
        //const uint32 LBA = atoi(string);
        const uint8 NO_OF_SECTORS = 1;
        char buf[ATA_SECTOR_SIZE] = {0};

        memset(buf, 0, sizeof(buf));
        delete_file(string);
    }
    
    
    else
    {
        
        printf("\n%s is not a recognised commands",buffer);
    }
   
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