#ifndef FS_H
#define FS_H
#include "stdint.h"
#include "types.h"
#include "ide.h"

#define MAGIC1 0x11
#define MAGIC2 0x22
#define MAGIC3 0x33
#define MAGIC4 0x44


typedef struct FILE
{
    char filename[8];
    char data[ATA_SECTOR_SIZE-8-5-8];
    char is_file[5];
    int next_sector;
};
typedef struct fs_partition_table
{
    int used_sectors[128];

};

typedef struct format_table
{
    char is_format[4];
    char format[20];
    int sector_size;



};
// typedef struct FILE_V2
// {
//     char filename[8];
//     char data[ATA_SECTOR_SIZE-8-5-8-8-3];
//     char is_file[5];
//     char dictionary[8];
//     char file_type[3];
//     int next_sector;
// };
// typedef struct FILE_HEADER_V1
// {
//     char filename[8];
//     int data[(ATA_SECTOR_SIZE-8-5-8-8-3)/8]; //stores a list of the LBA where the files data is stored
//     char is_file[5];
//     char dictionary[8];
//     char file_type[3];
//     int next_sector;
// };
#define MAX_FILE_SIZE 41*512 //max file size in bytes
typedef struct FILE_HEADER_V1
{
    char filename[8];
    int file_data_lbas[(ATA_SECTOR_SIZE-8-5-8-8-3-8)/8]; //stores a list of the LBA where the files data is stored
    char is_file[5];
    char dictionary[8];
    char file_type[3];
    int num_sectors;
    int file_size; //number of sectors in the file
};
typedef struct BLOCK
{
    char data[ATA_SECTOR_SIZE];
    
};
typedef struct DICTIONARY
{
    char dict_name[8];
    char flies[20][8];
};
int write(char filename[8],char file_type[3],char data[MAX_FILE_SIZE]);
int run_once();
int read(char filename[8]);
int init_fs();
int update_table();
void list_files();
int fs_partition_table_main_update();
int fs_partition_table_main_p();
int format_disk(int disk);
delete_file(char *filename[8]);
void clean_fs_partition_table_main(int num);
int make_dir(char *dir_name[8]);
int read_add(char filename[8],char* out);


//# VERSION 2 
typedef struct
{
    int version;
    int magic1;
    int disk_info_sector;
    int disk_info_size; //size in bytes
    int file_data_start;
    int sector_size;
    int magic2;
    int root_dir_inode;
    int block_size;
    int disk_size;
    int num_blocks; // Lba_storage_location_end - Lba_storage_location_start
    int num_sectors; 
    int magic3;
    char drive_name[10];
    int supports_readonly;
    int magic4;
   
    
}SUPERBLOCK;

typedef struct
{
    char filename[20];
    int size;
    int position_of_file_blocks[40];
    

}INODE;
typedef struct
{
    char file_data[512]
}FILE_DATA;

typedef struct
{
    int block_num;
    char free_blocks[508];
    
}FREE_BLOCK_BITMAP;


// typedef struct 
// {
//     char filename[50]
// }INODE;


int format_drive(int drive);
//int write_file(int drive, char *filename[20], char file_data[40*512]);
int initialize_file_system(int drive);
int write_fs(char filename[20],int size, char file_contents[512-8-20]);
#endif
