#include "ide.h"
#include "display.h"
#include "string.h"
#include "types.h"
#include "fs.h"
#include <stddef.h>


char lba_used[512];

struct file
{
    char filename[8];
    char data[ATA_SECTOR_SIZE-8-5];
    char is_file[5];
};


int write(char filename[8], char data[512-10])
{
    for (uint32 i = 13; i < 900; i++)
    {
        uint32 LBA = i;
        char buf[512] = {0};
        memset(buf, 0, sizeof(buf));
        struct file f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));
        //printf("File lba: %d\n",LBA);
        //printf("Is_file: %s\n",f.is_file);
        if(strcmp(f.is_file,"True") != 0)
        {
            strcpy(f.filename,filename);
            strcpy(f.data,data);
            strcpy(f.is_file,"True");
            memset(buf, 0, sizeof(buf));
            memcpy(buf, &f, sizeof(f));
            ide_write_sectors(0,1,LBA,buf);
            // printf(" \nis_file: %s\n",f.is_file);
            // printf("File name: %s\n",f.filename);
            // printf("File data: %s\n",f.data);
            // printf("lba: %d",LBA);
            return LBA;
        }
        

       
        
        
    }
    return -1;
    
    
   
}
int read(char filename[8])
{
    for (uint32 i = 13; i < 900; i++)
    {
        uint32 LBA = i;
        char buf[512] = {0};
        memset(buf, 0, sizeof(buf));
        struct file f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));
        //printf("File name: %s\n",f.filename);
        //printf("is_file: %s\n",f.is_file);
        if(strcmp(f.is_file, "True") == 0 && strcmp(f.filename,filename) == 0)
        {
            printf("\nFile data\n%s",f.data);
            // strcpy(f.filename,filename);
            // strcpy(f.data,data);
            // f.is_file == 1;
            // memset(buf, 0, sizeof(buf));
            // memcpy(buf, &f, sizeof(f));
            // ide_write_sectors(0,1,LBA,buf);
            return LBA;
        }
        

       
        
        
    }
}


