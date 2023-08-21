#include "cmdhandler.h"
#include "ide.h"
#include "display.h"
#include "string.h"
#include "types.h"
#include "fs.h"
#include <stddef.h>
#include "kheap.h"
#define MAX_SIZE 100000000000000

// Global Variables
struct fs_partition_table fs_partition_table_main;
struct format_table fs_format_table;
char current_dir[8] = "root";
SUPERBLOCK superblock;

char *free_block_list = NULL; //List of inodes

// Function: init_fs
// Description: Initializes the file system. It reads the format table and the master table from the disk.
//              If the disk is not formatted, it formats the disk, creates a new master table, and writes it to the disk.
// Return: Integer - Returns 0 if successful.
int init_fs()
{
    // Read format table from disk
    
    int ret = 0;
    uint32 LBA = 5;
    char table[512];
    memset(table, 0, sizeof(table));
    ret = ide_read_sectors(0, 1, 20, (uint32)table);
    if(ret != 0) 
    {
        return ret;

    }
    memcpy(&fs_format_table, table, sizeof(fs_format_table));

    // Check if the disk is formatted
    if (strcmp(fs_format_table.is_format, "True") == 0)
    {
        // Read the master table from disk
        printf("File format: %s\n", fs_format_table.format);
        char buf[900] = {0};
        memset(buf, 0, sizeof(buf));
        ret = ide_read_sectors(0, 1, KERNEL_SECTOR_BASE + 2, (uint32)buf);
        if(ret != 0)
        {
            return ret;
        }
        memcpy(&fs_partition_table_main, buf, sizeof(fs_partition_table_main));

        
        //printf("Size of int --> %d\n",sizeof(fs_partition_table_main.used_sectors));
        //printf("Size of unsigned int --> %d\n",sizeof(unsigned int));

        // Debug: Print used sectors in the master table
        #define FILE_OUT 0
        #if FILE_OUT
            for (size_t i = 0; i < 512; i++)
            {
                if (fs_partition_table_main.used_sectors[i] != 0)
                {
                    printf("\nUsed sectors: %d", fs_partition_table_main.used_sectors[i]);
                }
            }
        #endif
    }
    else
    {
        // If the disk is not formatted, format the disk and create a new master table
        printf("Disk not formatted\n");
        return -1;
        format_disk(0);
        run_once();
        printf("Formatted disk\n");
        char buf[900] = {0};
        memset(buf, 0, sizeof(buf));
        ret = ide_read_sectors(0, 1, KERNEL_SECTOR_BASE + 2, (uint32)buf);
        if(ret != 0)
        {
            return ret;
        }
        memcpy(&fs_partition_table_main, buf, sizeof(fs_partition_table_main));

        // Debug: Print used sectors in the master table
        #define FILE_OUT 0
        #if FILE_OUT
            for (size_t i = 0; i < 512; i++)
            {
                if (fs_partition_table_main.used_sectors[i] != 0)
                {
                    printf("\nUsed sectors: %d", fs_partition_table_main.used_sectors[i]);
                }
            }
        #endif
    }
    return 0;
}

// Function: run_once
// Description: Initializes the master table for the first time by setting the first used sector to FILE_SECTOR_BASE.
// Return: Integer - Returns 0 if successful.
int run_once()
{
    struct fs_partition_table min_tab;
    char buf[sizeof(min_tab)];
    min_tab.used_sectors[0] = FILE_SECTOR_BASE;

    memset(buf, 0, sizeof(buf));
    memcpy(buf, &min_tab, sizeof(min_tab));
    ide_write_sectors(0, 1, KERNEL_SECTOR_BASE + 2, buf);
    return 0;
}

// Function: update_table
// Description: Updates the master table on the disk with the current in-memory master table.
// Return: Integer - Returns 0 if successful.
int update_table()
{
    int ret = 0;
    struct fs_partition_table min_tab;
    memcmp(&min_tab.used_sectors, fs_partition_table_main.used_sectors, sizeof(fs_partition_table_main.used_sectors)); //min_tab.used_sectors = fs_partition_table_main.used_sectors;
    char buf[920];
    memset(buf, 0, sizeof(buf));
    memcpy(buf, &min_tab, sizeof(min_tab));
    ret = ide_write_sectors(0, 8, KERNEL_SECTOR_BASE + 2, buf);
    return ret;
}

// Function: make_dir
// Description: Creates a new directory with the specified name.
// Parameters:
//      - dir_name: The name of the new directory (up to 8 characters).
// Return: Integer - Returns the Logical Block Address (LBA) of the created directory if successful, or -1 if failed.
int make_dir(char *dir_name[8])
{
    char buf[512] = {0};
    memset(buf, 0, sizeof(buf));
    for (uint32 i = FILE_SECTOR_BASE + 1; i < 900; i++)
    {
        uint32 LBA = i;
        #define DEBUG
        #ifdef DEBUG
            printf("%d : ", fs_partition_table_main.used_sectors[i - FILE_SECTOR_BASE + 1]);
        #endif

        // Check if the sector is available for use
        if (isInArray(LBA, fs_partition_table_main.used_sectors, 128) != 0)
        {
            struct DICTIONARY d;
            memset(buf, 0, sizeof(buf));
            ide_read_sectors(0, 1, i, (uint32)buf);
            memcpy(&d, buf, sizeof(d));
            fs_partition_table_main.used_sectors[i - FILE_SECTOR_BASE + 1] = LBA;
            strcpy(d.dict_name, dir_name);

            memset(buf, 0, sizeof(buf));
            memcpy(buf, &d, sizeof(d));
            ide_write_sectors(0, 1, LBA, buf);
            printf("\nYour directory is stored in LBA %d", LBA);
            fs_partition_table_main_update();
            return LBA;
        }
    }
    return -1;
}

// Function: isInArray
// Description: Checks if a given number is present in an array.
// Parameters:
//      - num: The number to search for in the array.
//      - arr: The array to search in.
//      - size: The size of the array.
// Return: Integer - Returns 0 if the number is present in the array, or 1 if it's not found.
int isInArray(int num, int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == num)
        {
            return 0;
        }
    }
    return 1;
}

// Function: addIfNotInArray
// Description: Adds a number to an array if it's not already present.
// Parameters:
//      - num: The number to add to the array.
//      - arr: The array to add the number to.
//      - size: A pointer to the size of the array.
// Return: None
void addIfNotInArray(int num, int *arr, int *size)
{
    if (!isInArray(num, arr, *size))
    {
        if (*size < MAX_SIZE)
        {
            arr[*size] = num;
            (*size)++;
        }
        else
        {
            printf("Error: Array is full. Cannot add more elements.\n");
        }
    }
}

// Function: removeIntFromArray
// Description: Removes a number from an array.
// Parameters:
//      - arr: The array to remove the number from.
//      - size: A pointer to the size of the array.
//      - num: The number to remove from the array.
// Return: None
void removeIntFromArray(int *arr, int *size, int num)
{
    int found = 0;
    int index = -1;

    // Find the index of the number in the array
    for (int i = 0; i < *size; i++)
    {
        if (arr[i] == num)
        {
            found = 1;
            index = i;
            break;
        }
    }

    // If the number is found, remove it from the array
    if (found)
    {
        for (int i = index; i < *size - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
        (*size)--;
        printf("Number %d has been removed from the list.\n", num);
    }
    else
    {
        printf("Number %d is not in the list.\n", num);
    }
}

// Function: clean_fs_partition_table_main
// Description: Cleans the master table by removing a given number from the used sectors array.
// Parameters:
//      - num: The number to remove from the used sectors array.
// Return: None
void clean_fs_partition_table_main(int num)
{
    int *array[900];
    for (size_t i = 0; i < 128; i++)
    {
        if (fs_partition_table_main.used_sectors[i] == num)
        {
            fs_partition_table_main.used_sectors[i] = 0;
        }
    }
    //TODO: Add a return value
    update_table();
}

// Function: list_files
// Description: Lists all files in the current directory.
// Return: None
void list_files()
{
    //TODO: Add a return value
    for (uint32 i = FILE_SECTOR_BASE + 1; i < 900; i++)
    {
        uint32 LBA = i;
        char buf[512] = {0};
        memset(buf, 0, sizeof(buf));
        struct FILE_HEADER_V1 f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));

        // Check if the sector is used and the file belongs to the current directory
        if (isInArray(LBA, fs_partition_table_main.used_sectors, 128) == 0 &&
            strcmp(f.is_file, "True") == 0 && strcmp(f.dictionary, current_dir) == 0)
        {
            printf("\nFile name: %s", f.filename);
            printf("\nDirectory name: %s", f.dictionary);
            printf("\nLBA %d", LBA);

            // Clean up master table if necessary
            if (strcmp(f.is_file, "True") != 0 && isInArray(LBA, fs_partition_table_main.used_sectors, sizeof(fs_partition_table_main.used_sectors) / 8) == 0)
            {
                clean_fs_partition_table_main(LBA);
            }
        }
    }
}

// Function: format_disk
// Description: Formats the disk by writing the format table and initializing the master table.
// Return: Integer - Returns 0 if successful.
int format_disk(int drive)
{
    struct format_table format;
    char buf[512];

    // Set format table
    strcpy(format.format, "SSFS_V1");
    strcpy(format.is_format, "True");
    format.sector_size = 512;

    // Clear used sectors in the master table
    for (size_t i = 0; i < 128; i++)
    {
        fs_partition_table_main.used_sectors[i] = 0;
    }

    memset(buf, 0, sizeof(buf));
    memcpy(buf, &format, sizeof(format));
    int ret = 0;
    ret = ide_write_sectors(drive, 1, 20, buf);
    cmd_handler("set-xy");
    return ret;
}

// Function: fs_partition_table_main_p
// Description: Prints the master table for debugging purposes.
// Return: None
int fs_partition_table_main_p()
{
    printf("\n");
    clear_screen();
    printf("%d\n", sizeof(fs_partition_table_main));
    for (size_t i = 0; i < 128; i++)
    {
        printf("%d|", fs_partition_table_main.used_sectors[i]);
    }
    return 0;
}
int str_len(char *str) 
{
    int len = 0;
    while(*str)
    {
        len++;
        *str++;
    }
    return len;
}
// Function: write
// Description: Writes data to a file with the specified name in the current directory.
// Parameters:
//      - filename: The name of the file to write to (up to 8 characters).
//      - file_type: The type of the file (up to 3 characters).
//      - data: The data to be written to the file.
// Return: Integer - Returns the Logical Block Address (LBA) of the file if successful, or -1 if failed.
int write(char filename[8], char file_type[3], char data[MAX_FILE_SIZE])
{
    char buf[512] = {0};
    memset(buf, 0, sizeof(buf));
    int ret = 0;
    //printf("\nData: %d\n",sizeof(data));
    for (uint32 i = FILE_SECTOR_BASE + 1; i < 900; i++)
    {
        uint32 LBA = i;
        #define DEBUG 0
        #if DEBUG
            printf("\n%d : ", fs_partition_table_main.used_sectors[i - FILE_SECTOR_BASE + 1]);
        #endif

        // Check if the sector is available for use
        if (isInArray(LBA, fs_partition_table_main.used_sectors, 128) != 0)
        {
            struct FILE_HEADER_V1 f;
            memset(buf, 0, sizeof(buf));
            ret = ide_read_sectors(0, 1, i, (uint32)buf);
            if(ret != 0)
            {
                return ret;
            }
            memcpy(&f, buf, sizeof(f));
            fs_partition_table_main.used_sectors[i - FILE_SECTOR_BASE + 1] = LBA;
            strcpy(f.filename, filename);
            
            struct BLOCK fs_data;
            int count = 0;
            //printf("\n");
            for (size_t x = 0; x < (strlen(data)/512)+1; x++)
            {
                //printf("Write: f.data[%d] = %d\n",x,LBA+x+1);
                f.file_data_lbas[x] = LBA+x+1;
                count = count + 1;
                //printf("Count: %d\n",count);
                

            }
            f.num_sectors = count;
            
            
            
            strcpy(f.is_file, "True");
            strcpy(f.dictionary, current_dir);
            f.file_size = strlen(data);
            //printf("\nBuf: %d\n",sizeof(buf));
            memset(buf, 0, sizeof(buf));
            memcpy(buf, &f, sizeof(f));
            ret = ide_write_sectors(0, 1, LBA, (uint32)buf);
            if(ret != 0)
            {
                return ret;
            }
            int start = 0;
            for (size_t w = 0; w < count; w++)
            {
                
                //printf("W");
                memset(fs_data.data,0,sizeof(fs_data.data));
                memset(buf, 0, sizeof(buf));
                for (size_t e = start; e < start+512; e++)
                {
                    if(strlen(data) <= start)
                    {
                       printf("%d ", e);
                    }
                     append(buf, data[e]);
                    
                    
                }
                append(buf,'\0');
                strcpy(fs_data.data, buf);
                 memset(buf, 0, sizeof(buf));
                 memcpy(buf, &fs_data, sizeof(fs_data));
                ret = ide_write_sectors(0,1,f.file_data_lbas[w],(uint32)buf);
                if(ret != 0)
                {
                    return ret;
                }
                // printf("Data at LBA+1+w: %d\n",f.file_data_lbas[w]);
                // printf("%s\n",buf);
                start = start + 512;
                
                //printf("\n fs_data.data: %s",buf);
                fs_partition_table_main.used_sectors[i - FILE_SECTOR_BASE + 2+w] = f.file_data_lbas[w];
            }
            
            // memset(buf, 0, sizeof(buf));
            // memcpy(buf, &fs_data, sizeof(fs_data));
            // ide_write_sectors(0,1,LBA+1,(uint32)buf);
            //printf("\n fs_data.data: %s",buf);
             //fs_partition_table_main.used_sectors[i - FILE_SECTOR_BASE + 2] = LBA+1;
            //printf("\nYour file is stored in LBA %d", LBA);
            fs_partition_table_main_update();
            return 0;
        }
    }
    return ret;
}

// Function: read
// Description: Reads the contents of a file with the specified name in the current directory.
// Parameters:
//      - filename: The name of the file to read from (up to 8 characters).
// Return: Integer - Returns the Logical Block Address (LBA) of the file if successful, or -1 if the file is not found.
int read(char filename[8])
{
    for (uint32 i = FILE_SECTOR_BASE + 1; i < 900; i++)
    {
        uint32 LBA = i;
        char buf[512] = {0};
        memset(buf, 0, sizeof(buf));
        struct FILE_HEADER_V1 f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));

        // Check if the sector is used and the file belongs to the current directory
        if (isInArray(LBA, fs_partition_table_main.used_sectors, 128) == 0 &&
            strcmp(f.filename, filename) == 0 && strcmp(f.dictionary, current_dir) == 0)
        {
            // if(f.next_sector != 0)
            // {
            //     printf("here: %d",f.next_sector);
            // }
             memset(buf, 0, sizeof(buf));
             
             char data_out[MAX_FILE_SIZE] = {0};
             memset(data_out, 0, sizeof(data_out));
             //+printf("\n");
             for (size_t x = 0; x < f.num_sectors; x++)
             {
                struct BLOCK fs_data;
                if(f.file_data_lbas[x] != 0)
                {
                    //printf("Read: f.data[%d] = %d\n",x,f.data[x]);
                    char inner_buf[512] = {0};
                    memset(inner_buf, 0, sizeof(inner_buf));
                    ide_read_sectors(0,1,f.file_data_lbas[x],(uint32)inner_buf);
                    memcpy(&fs_data, inner_buf, sizeof(fs_data));
                    //printf("strlen(%d)",strlen(fs_data.data));
                    for (size_t q = 0; q < 512; q++)
                    {
                        if(fs_data.data[q] != 0 && strlen(data_out) <= f.file_size-1)
                        {
                            append(data_out,fs_data.data[q]);
                        }
                       
                       //printf("fs_data.data[%d] = %s ",q,fs_data.data[q]);
                    }
                    //printf("Data at: %d\n",f.data[x]);
                    //printf("%s\n",fs_data.data);
                   
                     //printf("\nFile data in loop\n%s", data_out);
                    
                    
                }
                else
                {
                    break;
                }
               

             }
             
            printf("\nFile data\n%s", data_out);
            return LBA;
        }
    }
    return -1;
}
int read_add(char filename[8],char* out)
{
    out = malloc(MAX_FILE_SIZE);
    for (uint32 i = FILE_SECTOR_BASE + 1; i < 900; i++)
    {
        uint32 LBA = i;
        char buf[512] = {0};
        memset(buf, 0, sizeof(buf));
        struct FILE_HEADER_V1 f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));

        // Check if the sector is used and the file belongs to the current directory
        if (isInArray(LBA, fs_partition_table_main.used_sectors, 128) == 0 &&
            strcmp(f.filename, filename) == 0 && strcmp(f.dictionary, current_dir) == 0)
        {
            // if(f.next_sector != 0)
            // {
            //     printf("here: %d",f.next_sector);
            // }
             memset(buf, 0, sizeof(buf));
             
             char data_out[MAX_FILE_SIZE] = {0};
             memset(data_out, 0, sizeof(data_out));
             //+printf("\n");
             for (size_t x = 0; x < f.num_sectors; x++)
             {
                struct BLOCK fs_data;
                if(f.file_data_lbas[x] != 0)
                {
                    //printf("Read: f.data[%d] = %d\n",x,f.data[x]);
                    char inner_buf[512] = {0};
                    memset(inner_buf, 0, sizeof(inner_buf));
                    ide_read_sectors(0,1,f.file_data_lbas[x],(uint32)inner_buf);
                    memcpy(&fs_data, inner_buf, sizeof(fs_data));
                    //printf("strlen(%d)",strlen(fs_data.data));
                    for (size_t q = 0; q < 512; q++)
                    {
                        if(fs_data.data[q] != 0 && strlen(data_out) <= f.file_size-1)
                        {
                            append(data_out,fs_data.data[q]);
                        }
                       
                       //printf("fs_data.data[%d] = %s ",q,fs_data.data[q]);
                    }
                    //printf("Data at: %d\n",f.data[x]);
                    //printf("%s\n",fs_data.data);
                   
                     //printf("\nFile data in loop\n%s", data_out);
                    
                    
                }
                else
                {
                    break;
                }
               

             }
             memcpy(out, data_out, sizeof(data_out));
            //printf("\nFile data\n%s", data_out);
            return LBA;
        }
    }
    return -1;
}
// Function: delete_file
// Description: Deletes a file with the specified name from the current directory.
// Parameters:
//      - filename: The name of the file to delete (up to 8 characters).
// Return: Integer - Returns the Logical Block Address (LBA) of the file if successful, or -1 if the file is not found.
int delete_file(char *filename[8])
{
    for (uint32 i = FILE_SECTOR_BASE + 1; i < 900; i++)
    {
        uint32 LBA = i;
        char buf[512] = {0};
        memset(buf, 0, sizeof(buf));
        struct FILE_HEADER_V1 f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));

        if (strcmp(f.filename, filename) == 0)
        {
            // if(f.num_sectors != 0)
            // {
            //     printf("here: %d",f.num_sectors);
            // }
            memset(buf, 0, sizeof(buf));
            ide_write_sectors(0, 1, i, (uint32)buf);

            clean_fs_partition_table_main(LBA);
            return LBA;
        }
    }
    return -1;
}

// Function: fs_partition_table_main_update
// Description: Updates the master table on the disk with the current in-memory master table.
// Return: None
int fs_partition_table_main_update()
{
    struct fs_partition_table min_tab;
    char buf[920];

    memset(buf, 0, sizeof(buf));
    memcpy(buf, &fs_partition_table_main, sizeof(fs_partition_table_main));
    ide_write_sectors(0, 1, KERNEL_SECTOR_BASE + 2, buf);
    return 0;
}





//# VERSION 2
// Function: read_superblock
// Description: Reads the superblock
// Parameters: drive - the drive to read from
// returns: none
int read_superblock(int drive)
{
    
    char buffer[512];// buffer to hold the superblock value
    memset(buffer, 0, sizeof(buffer)); // set the buffer to zero
    ide_read_sectors(drive,1,10,(uint32)buffer); // read the superblock int the buffer
    memcpy(&superblock,buffer,sizeof(superblock)); // copy the superblock data into the superblock struct
     // allocate memory for the free block list

}
// Function: write_superblock
// Description: writes the superblock
// Parameters: 
//  drive - the drive to write to
//  superblock - the superblock to write
// returns: none
int write_superblock(int drive, SUPERBLOCK superblock_to_write)
{
    char buffer[512]; // buffer to hold the superblock data
    memset(buffer, 0, sizeof(buffer)); // clear the buffer
    memcpy(buffer, &superblock_to_write, sizeof(buffer)); // copy the superblock data into the buffer
    ide_write_sectors(drive,1,10,(uint32)buffer); // write the superblock

}
int write_block(int drive, int pos, FREE_BLOCK_BITMAP block)
{
    char buffer[512] = { 0};
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, &block, sizeof(buffer));
    ide_write_sectors(drive,1,pos ,(uint32)buffer);

}
int read_block(int drive, int pos, FREE_BLOCK_BITMAP block)
{
    char buffer[512] = { 0};
    memset(buffer, 0, sizeof(buffer));
    
    ide_read_sectors(drive,1,pos ,(uint32)buffer);
    memcpy(&block,buffer, sizeof(block));

}
int format_drive(int drive)
{
    
    superblock.block_size = 512;
    superblock.disk_size = get_sectors(drive);
    strcpy(superblock.drive_name,"ALEGA");
    superblock.lba_storage_location_start = 30;
    superblock.num_blocks = (int)get_sectors(drive)/508;
    superblock.lba_storage_location_end = superblock.lba_storage_location_start+superblock.num_blocks ;
    superblock.magic1 = 111;
    superblock.magic2 = 111;
    superblock.magic3 = 111;
    superblock.magic4 = 111;
   
    superblock.sector_size = 512;
    superblock.version = 1;
    char buffer[512] = {0};
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, &superblock, sizeof(buffer));
    ide_write_sectors(drive,1,10,(uint32)buffer);
    char pre_allocated_block[superblock.num_blocks];
    printf("sizeof(pre_allocated_block) = %d\n", sizeof(pre_allocated_block));
    for (size_t i = 30; i < superblock.lba_storage_location_end; i++)
    {
        FREE_BLOCK_BITMAP block;
        block.block_num = i-30;
        for (size_t q = 0; q < 508; q++)
        {
            block.free_blocks[q] = '0';
            
        }
        write_block(drive,superblock.lba_storage_location_start+i,block);
        
    }
    printf("HEHE: %d\n",(int)(sizeof(pre_allocated_block))+10);
    int end = (sizeof(pre_allocated_block)/508)+1;
    for (size_t a = 0; a < end; a++)
    {
        printf("HERE");
        FREE_BLOCK_BITMAP block;
        block.block_num = a;

        for (size_t w = 0; w < 508; w++)
        {
            block.free_blocks[w] = '1';
        }
        write_block(drive,superblock.lba_storage_location_start+a,block);
    }
    
    
    
}    
int initialize_file_system(int drive)
{
    free_block_list = kmalloc(superblock.num_blocks*508);
    read_superblock(drive);
    
    if(strcmp(superblock.drive_name,"ALEGA") == 0)
    {
        printf("True");
        get_free_sectors();
    //printf("S\n%c",free_block_list);
    }
    else
    {
        printf("RUN format %d",drive);
    }
    
    

}










int get_free_sectors()
{
    int start = superblock.lba_storage_location_start+1;
    int end = superblock.lba_storage_location_end;
    
    for (size_t block = start; block < end; block++)
    {
        char buffer[512] ={0};
        memset(buffer, 0, sizeof(buffer));
        ide_read_sectors(0,1,block,(uint32)buffer);
        FREE_BLOCK_BITMAP free_blocks;
        memcpy(&free_blocks, buffer, sizeof(free_block_list));
        int pos_in_list = 0;
        for (size_t lba = 0; lba < 508; lba++)
        {
            free_block_list[pos_in_list] = free_blocks.free_blocks[lba];
            pos_in_list = pos_in_list + 1;
             printf("%c ", free_block_list[pos_in_list]);
        }
    }
        
}














int find_free_block()
{
    for (size_t pos_to_write_to = superblock.lba_storage_location_end+1; pos_to_write_to < get_sectors(0); pos_to_write_to++)
    {
        if(free_block_list[pos_to_write_to-(superblock.lba_storage_location_end+1)] == '0')
        {
            free_block_list[pos_to_write_to-(superblock.lba_storage_location_end+1)] = '1';
            return pos_to_write_to+superblock.lba_storage_location_end+1;
        }
    }
}
int write_inode(int pos,INODE inode)
{
    char buffer[512] = {0};
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, &inode, sizeof(buffer));
    ide_write_sectors(0,1,pos,(uint32)buffer);
    printf("inode pos: %d\n",pos);
}
void write_data_chunk(const char *input) {
    
}
int write_file(int drive, char *filename[20], char file_data[40*512])
{
    
    int inode_postion = find_free_block();
    INODE inode;
    strcpy(inode.filename, filename);
    inode.size = strlen(file_data)*8;
    size_t len = strlen(file_data);
    size_t chunk_size = 512;

    // for (size_t i = 0; i < len; i += chunk_size) {
        
    //     size_t remaining = len - i;
    //     size_t current_chunk_size = remaining < chunk_size ? remaining : chunk_size;

    //     // Create a buffer for the current chunk
    //     char chunk[current_chunk_size + 1];
    //     strncpy(chunk, file_data + i, current_chunk_size);
    //     chunk[current_chunk_size] = '\0';

    //     // Process the chunk (print it in this example)
    //     int data_pos = find_free_block();
        
    //     ide_write_sectors(drive,1,data_pos,(uint32)chunk);
    //     inode.position_of_file_blocks[i] = data_pos;
    // }
    write_inode(inode_postion,inode);
    

}

