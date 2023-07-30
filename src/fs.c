#include "cmdhandler.h"
#include "ide.h"
#include "display.h"
#include "string.h"
#include "types.h"
#include "fs.h"
#include <stddef.h>

#define MAX_SIZE 100

// Global Variables
struct fs_table fs_master_table;
struct format_table fs_format_table;
char current_dir[8] = "root";

// Function: init_fs
// Description: Initializes the file system. It reads the format table and the master table from the disk.
//              If the disk is not formatted, it formats the disk, creates a new master table, and writes it to the disk.
// Return: Integer - Returns 0 if successful.
int init_fs()
{
    // Read format table from disk
    uint32 LBA = 5;
    char table[512];
    memset(table, 0, sizeof(table));
    ide_read_sectors(0, 1, 20, (uint32)table);
    memcpy(&fs_format_table, table, sizeof(fs_format_table));

    // Check if the disk is formatted
    if (strcmp(fs_format_table.is_format, "True") == 0)
    {
        // Read the master table from disk
        char buf[900] = {0};
        memset(buf, 0, sizeof(buf));
        ide_read_sectors(0, 1, KERNEL_SECTOR_BASE + 2, (uint32)buf);
        memcpy(&fs_master_table, buf, sizeof(fs_master_table));

        printf("File format: %s\n", fs_format_table.format);

        // Debug: Print used sectors in the master table
        #define FILE_OUT 0
        #if FILE_OUT
            for (size_t i = 0; i < 512; i++)
            {
                if (fs_master_table.used_sectors[i] != 0)
                {
                    printf("\nUsed sectors: %d", fs_master_table.used_sectors[i]);
                }
            }
        #endif
    }
    else
    {
        // If the disk is not formatted, format the disk and create a new master table
        printf("Disk not formatted\n");
        format_disk();
        run_once();
        printf("Formatted disk\n");
        char buf[900] = {0};
        memset(buf, 0, sizeof(buf));
        ide_read_sectors(0, 1, KERNEL_SECTOR_BASE + 2, (uint32)buf);
        memcpy(&fs_master_table, buf, sizeof(fs_master_table));

        // Debug: Print used sectors in the master table
        #define FILE_OUT 0
        #if FILE_OUT
            for (size_t i = 0; i < 512; i++)
            {
                if (fs_master_table.used_sectors[i] != 0)
                {
                    printf("\nUsed sectors: %d", fs_master_table.used_sectors[i]);
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
    struct fs_table min_tab;
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
    struct fs_table min_tab;
    memcmp(&min_tab.used_sectors, fs_master_table.used_sectors, sizeof(fs_master_table.used_sectors)); //min_tab.used_sectors = fs_master_table.used_sectors;
    char buf[920];
    memset(buf, 0, sizeof(buf));
    memcpy(buf, &min_tab, sizeof(min_tab));
    ide_write_sectors(0, 8, 5, buf);
    return 0;
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
            printf("%d : ", fs_master_table.used_sectors[i - FILE_SECTOR_BASE + 1]);
        #endif

        // Check if the sector is available for use
        if (isInArray(LBA, fs_master_table.used_sectors, 900) != 0)
        {
            struct DICTIONARY d;
            memset(buf, 0, sizeof(buf));
            ide_read_sectors(0, 1, i, (uint32)buf);
            memcpy(&d, buf, sizeof(d));
            fs_master_table.used_sectors[i - FILE_SECTOR_BASE + 1] = LBA;
            strcpy(d.dict_name, dir_name);

            memset(buf, 0, sizeof(buf));
            memcpy(buf, &d, sizeof(d));
            ide_write_sectors(0, 1, LBA, buf);
            printf("\nYour directory is stored in LBA %d", LBA);
            fs_master_table_update();
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

// Function: clean_fs_master_table
// Description: Cleans the master table by removing a given number from the used sectors array.
// Parameters:
//      - num: The number to remove from the used sectors array.
// Return: None
void clean_fs_master_table(int num)
{
    int *array[900];
    for (size_t i = 0; i < 900; i++)
    {
        if (fs_master_table.used_sectors[i] == num)
        {
            fs_master_table.used_sectors[i] = NULL;
        }
    }
    update_table();
}

// Function: list_files
// Description: Lists all files in the current directory.
// Return: None
void list_files()
{
    for (uint32 i = FILE_SECTOR_BASE + 1; i < 900; i++)
    {
        uint32 LBA = i;
        char buf[512] = {0};
        memset(buf, 0, sizeof(buf));
        struct FILE_V2 f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));

        // Check if the sector is used and the file belongs to the current directory
        if (isInArray(LBA, fs_master_table.used_sectors, 900) == 0 &&
            strcmp(f.is_file, "True") == 0 && strcmp(f.dictionary, current_dir) == 0)
        {
            printf("\nFile name: %s", f.filename);
            printf("\nDirectory name: %s", f.dictionary);
            printf("\nLBA %d", LBA);

            // Clean up master table if necessary
            if (strcmp(f.is_file, "True") != 0 && isInArray(LBA, fs_master_table.used_sectors, sizeof(fs_master_table.used_sectors) / 8) == 0)
            {
                clean_fs_master_table(LBA);
            }
        }
    }
}

// Function: format_disk
// Description: Formats the disk by writing the format table and initializing the master table.
// Return: Integer - Returns 0 if successful.
int format_disk()
{
    struct format_table format;
    char buf[512];

    // Set format table
    strcpy(format.format, "SSFS_V1");
    strcpy(format.is_format, "True");
    format.sector_size = 512;

    // Clear used sectors in the master table
    for (size_t i = 0; i < 900; i++)
    {
        fs_master_table.used_sectors[i] = 0;
    }

    memset(buf, 0, sizeof(buf));
    memcpy(buf, &format, sizeof(format));
    ide_write_sectors(0, 1, 20, buf);
    cmd_handler("set-xy");
    return 0;
}

// Function: fs_master_table_p
// Description: Prints the master table for debugging purposes.
// Return: None
int fs_master_table_p()
{
    printf("\n");
    clear_screen();
    printf("%d\n", sizeof(fs_master_table));
    for (size_t i = 0; i < 900; i++)
    {
        printf("%d|", fs_master_table.used_sectors[i]);
    }
    return 0;
}

// Function: write
// Description: Writes data to a file with the specified name in the current directory.
// Parameters:
//      - filename: The name of the file to write to (up to 8 characters).
//      - file_type: The type of the file (up to 3 characters).
//      - data: The data to be written to the file.
// Return: Integer - Returns the Logical Block Address (LBA) of the file if successful, or -1 if failed.
int write(char filename[8], char file_type[3], char data[512 - 10])
{
    char buf[512] = {0};
    memset(buf, 0, sizeof(buf));
    for (uint32 i = FILE_SECTOR_BASE + 1; i < 900; i++)
    {
        uint32 LBA = i;
        #define DEBUG
        #ifdef DEBUG
            printf("%d : ", fs_master_table.used_sectors[i - FILE_SECTOR_BASE + 1]);
        #endif

        // Check if the sector is available for use
        if (isInArray(LBA, fs_master_table.used_sectors, 900) != 0)
        {
            struct FILE_V2 f;
            memset(buf, 0, sizeof(buf));
            ide_read_sectors(0, 1, i, (uint32)buf);
            memcpy(&f, buf, sizeof(f));
            fs_master_table.used_sectors[i - FILE_SECTOR_BASE + 1] = LBA;
            strcpy(f.filename, filename);
            if (sizeof(data) >= sizeof(f.data))
            {
                printf("Warning: Data size exceeds maximum file size.\n");
            }
            else
            {
                f.next_sector = 0;
            }
            strcpy(f.data, data);
            strcpy(f.is_file, "True");
            strcpy(f.dictionary, current_dir);

            memset(buf, 0, sizeof(buf));
            memcpy(buf, &f, sizeof(f));
            ide_write_sectors(0, 1, LBA, buf);
            printf("\nYour file is stored in LBA %d", LBA);
            fs_master_table_update();
            return LBA;
        }
    }
    return -1;
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
        struct FILE_V2 f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));

        // Check if the sector is used and the file belongs to the current directory
        if (isInArray(LBA, fs_master_table.used_sectors, 900) == 0 &&
            strcmp(f.filename, filename) == 0 && strcmp(f.dictionary, current_dir) == 0)
        {
            // if(f.next_sector != 0)
            // {
            //     printf("here: %d",f.next_sector);
            // }
            printf("\nFile data\n%s", f.data);
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
        struct FILE_V2 f;
        ide_read_sectors(0, 1, i, (uint32)buf);
        memcpy(&f, buf, sizeof(f));

        if (strcmp(f.filename, filename) == 0)
        {
            // if(f.next_sector != 0)
            // {
            //     printf("here: %d",f.next_sector);
            // }
            memset(buf, 0, sizeof(buf));
            ide_write_sectors(0, 1, i, (uint32)buf);

            clean_fs_master_table(LBA);
            return LBA;
        }
    }
    return -1;
}

// Function: fs_master_table_update
// Description: Updates the master table on the disk with the current in-memory master table.
// Return: None
int fs_master_table_update()
{
    struct fs_table min_tab;
    char buf[920];

    memset(buf, 0, sizeof(buf));
    memcpy(buf, &fs_master_table, sizeof(fs_master_table));
    ide_write_sectors(0, 1, KERNEL_SECTOR_BASE + 2, buf);
    return 0;
}