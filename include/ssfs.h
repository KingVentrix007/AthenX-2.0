#ifndef __SSFS__H
#define __SSFS__H
#include "../include/types.h"
// Define the size of a sector (adjust according to your needs)
// Define the maximum size for the disk name
#define MAX_DISK_NAME_SIZE 100

// Define the size of the struct (1024 bytes)
#define PARTITION_TABLE_SIZE 1024

// Define the maximum size for the file name
#define MAX_FILE_NAME_SIZE 100

// Define the maximum number of LBA entries in the array
#define MAX_LBA_ENTRIES 10

// Define the size of the struct (512 bytes)
#define FILE_HEADER_SIZE 512

// Define the maximum size for the file data
#define MAX_FILE_DATA_SIZE 500  // 500 bytes to leave some space for other information

// Define the size of the struct (512 bytes)
#define FILE_CONTENTS_SIZE 512
// Define the partition table struct
typedef struct {
    char diskname[MAX_DISK_NAME_SIZE];  // Disk name (up to 100 characters)
    int magic1;               // Magic number 1 (1 byte)
    int disk_size;                      // Disk size in bytes (4 bytes)
    int file_start;                     // File start position (4 bytes)
    int partition_size;                 // Partition size in bytes (4 bytes)
    int free_sectors[(PARTITION_TABLE_SIZE - MAX_DISK_NAME_SIZE - sizeof(int) - 3 * sizeof(int)) / sizeof(int)];
    // Remaining space as an array of ints to fill up the remaining bytes
} partition_table;

typedef struct {
    char file_name[MAX_FILE_NAME_SIZE];  // File name (up to 100 characters)
    int file_size;                       // File size in bytes (4 bytes)
    int num_lba_entries;                // Number of LBA entries (4 bytes)
    int lba_entries[MAX_LBA_ENTRIES];   // Array to store LBA entries (up to 10 entries)
    char reserved[FILE_HEADER_SIZE - MAX_FILE_NAME_SIZE - 2 * sizeof(int) - MAX_LBA_ENTRIES * sizeof(int)];
    // Reserved space to fill up the remaining bytes
} file_header;
// Define the file contents struct
typedef struct {
    char data[MAX_FILE_DATA_SIZE];  // Actual file data (up to 500 bytes)
    char reserved[FILE_CONTENTS_SIZE - MAX_FILE_DATA_SIZE]; // Reserved space to fill up the remaining bytes
} file_contents;

int ssfs_format_disk(int disk_num, char *disk_name);

int read_ssfs_partion(partition_table part_table,int location,int drive);

int ssfs_write_file(file_header *header, char *data);
void splitStringIntoChunks(const char* inputString,int lba_list[10]);
void padString(char *str, int desiredLength, char paddingChar);
int ssfs_read(char *file_name, char out[512*10]);
int ssfs_update_table();
#endif