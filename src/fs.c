#include "ide.h"
#include "display.h"
#include "string.h"
#include "types.h"
#include "fs.h"
#include <stddef.h>



#define MAX_SIZE 100


struct fs_table fs_master_table;
struct format_table fs_format_table;

int init_fs()
{

    uint32 LBA = 5;
    char table[512];
    memset(table, 0, sizeof(table));
    ide_read_sectors(0, 1, 20, (uint32)table);
    memcpy(&fs_format_table, table, sizeof(fs_format_table));
    if(strcmp(fs_format_table.is_format,"True")==0)
    {
        char buf[900] = {0};
        memset(buf, 0, sizeof(buf));
        
        ide_read_sectors(0, 1, KERNEL_SECTOR_BASE+2, (uint32)buf);
        memcpy(&fs_master_table, buf, sizeof(fs_master_table));
        printf("File format: %s\n",fs_format_table.format);
        
        #define FILE_OUT 0
        #if FILE_OUT
            for (size_t i = 0; i < 512; i++)
            {
                if(fs_master_table.used_sectors[i] != 0)
                {
                    printf("\nUsed sectors: %d",fs_master_table.used_sectors[i]);
                }
                
            }
        #endif
    }
    else
    {
        printf("Disk not formatted\n");
        format_disk();
        run_once();
        printf("Formatted disk\n");
        char buf[900] = {0};
        memset(buf, 0, sizeof(buf));
        
        ide_read_sectors(0, 1, KERNEL_SECTOR_BASE+2, (uint32)buf);
        memcpy(&fs_master_table, buf, sizeof(fs_master_table));
        #define FILE_OUT 0
        #if FILE_OUT
            for (size_t i = 0; i < 512; i++)
            {
                if(fs_master_table.used_sectors[i] != 0)
                {
                    printf("\nUsed sectors: %d",fs_master_table.used_sectors[i]);
                }
                
            }
        #endif
    }
    
    
   
}

int run_once()
{
    struct fs_table min_tab;
    
    char buf[sizeof(min_tab)];
    min_tab.used_sectors[0] = FILE_SECTOR_BASE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf, &min_tab, sizeof(min_tab));
    ide_write_sectors(0,1,KERNEL_SECTOR_BASE+2,buf);
}
int update_table()
{
    
    struct fs_table min_tab;
    memcmp(&min_tab.used_sectors,fs_master_table.used_sectors,sizeof(fs_master_table.used_sectors));   //min_tab.used_sectors = fs_master_table.used_sectors;
    char buf[920];
    memset(buf, 0, sizeof(buf));
    memcpy(buf, &min_tab, sizeof(min_tab));
    ide_write_sectors(0,8,5,buf);
}
int isInArray(int num, int* arr, int size) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == num) {
            return 0;
        }
    }
    return 1;
}

void addIfNotInArray(int num, int* arr, int* size) {
    if (!isInArray(num, arr, *size)) {
        if (*size < MAX_SIZE) {
            arr[*size] = num;
            (*size)++;
        } else {
            printf("Error: Array is full. Cannot add more elements.\n");
        }
    }
}

void removeIntFromArray(int *arr, int *size, int num) {
    int found = 0;
    int index = -1;

    // Find the index of the number in the array
    for (int i = 0; i < *size; i++) {
        printf(arr[i]);
        if (arr[i] == num) {
            found = 1;
            index = i;
            break;
        }
       
    }

    // If the number is found, remove it from the array
    if (found) {
        for (int i = index; i < *size - 1; i++) {
            arr[i] = arr[i + 1];
        }
        (*size)--;
        printf("Number %d has been removed from the list.\n", num);
    } else {
        printf("Number %d is not in the list.\n", num);
    }
}
int list_files()
{
     for (uint32 i = FILE_SECTOR_BASE+1; i < 900; i++)
    {
        uint32 LBA = i;
        char buf[512] = {0};
        memset(buf, 0, sizeof(buf));
        struct FILE f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));
        //printf("File name: %s\n",f.filename);
        //printf("is_file: %s\n",f.is_file);
        if(isInArray(LBA,fs_master_table.used_sectors,900) == 0 && strcmp(f.is_file,"True")==0)
        {
            printf("\nFile name: %s",f.filename);
            printf("\nLBA %d",LBA);
            if(strcmp(f.is_file,"True") != 0 && isInArray(LBA,fs_master_table.used_sectors,sizeof(fs_master_table.used_sectors)/8) == 0)
            {
               
                removeIntFromArray(fs_master_table.used_sectors,sizeof(fs_master_table.used_sectors)/8,LBA);
            }
            // strcpy(f.filename,filename);
            // strcpy(f.data,data);
            // f.is_file == 1;
            // memset(buf, 0, sizeof(buf));
            // memcpy(buf, &f, sizeof(f));
            // ide_write_sectors(0,1,LBA,buf);
            //return LBA;
        }
        

       
        
        
    }
}


int format_disk()
{
    struct format_table format;
    
    char buf[512];
    strcpy(format.format,"SSFS_V1");
    strcpy(format.is_format,"True");
    
    format.sector_size = 512;

    memset(buf, 0, sizeof(buf));
    memcpy(buf, &format, sizeof(format));
    ide_write_sectors(0,1,20,buf);
}

int fs_master_table_p()
{
    printf("\n");
    clear_screen();
    
    printf("%d\n",sizeof(fs_master_table));
    for (size_t i = 0; i < 900; i++)
    {
        printf("%d|",fs_master_table.used_sectors[i]);

    }
    
}

int write(char filename[8], char data[512-10])
{
    char buf[512] = {0};
    memset(buf, 0, sizeof(buf));
    for (uint32 i = FILE_SECTOR_BASE+1; i < 900; i++)
    {
        uint32 LBA = i;
        
        struct FILE f;
        memset(buf, 0, sizeof(buf));
        
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));
        //ide_read_sectors(0, 1, i, (uint32)buf);
        // memcpy(&f, buf, sizeof(f));
        //printf("File lba: %d\n",LBA);
        //printf("Is_file: %s\n",f.is_file);
        
        printf("%d : ",fs_master_table.used_sectors[i-FILE_SECTOR_BASE+1]);
        if(isInArray(LBA,fs_master_table.used_sectors,900) != 0)
        {
             
                //printf("hello world");
                fs_master_table.used_sectors[i-FILE_SECTOR_BASE+1] = LBA;
                strcpy(f.filename,filename);
                if(sizeof(data) >= sizeof(f.data))
                {
                    printf("Waring");
                }
                else
                {
                    f.next_sector = 0;
                }
                strcpy(f.data,data);
                strcpy(f.is_file,"True");

                memset(buf, 0, sizeof(buf));
                memcpy(buf, &f, sizeof(f));
                ide_write_sectors(0,1,LBA,buf);
                printf("\nYour file is stored in LBA %d", LBA);
                fs_master_table_update();
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
    for (uint32 i = FILE_SECTOR_BASE+1; i < 900; i++)
    {
        uint32 LBA = i;
        char buf[512] = {0};
        memset(buf, 0, sizeof(buf));
        struct FILE f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));
        //printf("File name: %s\n",f.filename);
        //printf("is_file: %s\n",f.is_file);
        
        if(isInArray(LBA,fs_master_table.used_sectors,900) == 0 && strcmp(f.filename,filename) == 0)
        {
            // if(f.next_sector != 0)
            // {
            //     printf("here: %d",f.next_sector);
            // }
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

int delete_file(char *filename[8])
{
    for (uint32 i = FILE_SECTOR_BASE+1; i < 900; i++)
    {
        uint32 LBA = i;
        char buf[512] = {0};
        memset(buf, 0, sizeof(buf));
        struct FILE f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));
        //printf("File name: %s\n",f.filename);
        //printf("is_file: %s\n",f.is_file);
        if(strcmp(f.filename,filename) == 0)
        {
            // if(f.next_sector != 0)
            // {
            //     printf("here: %d",f.next_sector);
            // }
            memset(buf, 0, sizeof(buf));
            ide_write_sectors(0, 1, i, (uint32)buf);
            
            removeIntFromArray(fs_master_table.used_sectors,900,fs_master_table.used_sectors[i-FILE_SECTOR_BASE+1]);

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
int fs_master_table_update()
{
    struct fs_table min_tab;
    
    char buf[920];
    

    memset(buf, 0, sizeof(buf));
    memcpy(buf, &fs_master_table, sizeof(fs_master_table));
    ide_write_sectors(0,1,KERNEL_SECTOR_BASE+2,buf);
}