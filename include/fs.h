#ifndef FS_H
#define FS_H
#include "stdint.h"
#include "types.h"
#include "ide.h"
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
#define MAX_FILE_SIZE 1024 //max file size in bytes
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
    int lba_storage_location_start;
    int lba_storage_location_end;
    int sector_size;
    int magic2;
    int root_dir_inode;
    int block_size;
    int disk_size;
    int num_blocks; // Lba_storage_location_end - Lba_storage_location_start
    int magic3;
    char drive_name[10];
    int supports_readonly;
    int secondary_partition_table_start_lba;
    //char padding[512-60];
    int magic4;
}SUPERBLOCK;

typedef struct
{
    int magic1;
    char filename[10];
    char filetype[3];
    char dir[10];
    int dir_inode_lba;
    int extended_filename_lba;
    int file_bock_lba_list[118];
    int magic2;

}INODE_FILE;
typedef struct
{
    int magic1;
    char dirname[10];
    int lba_list_of_files;
    int magic2;

}INODE_DIR;

typedef struct
{
   
    char data[512];

}DATA_BLOCK;
typedef struct
{
    int magic1;
    int file_lba_list[127];
    int magic2;
    

}MAIN_INODE;

typedef struct
{
    int magic1;
    int file_lba_list[127];

}SECONDARY_MAIN_INODE;
typedef struct
{
    int magic1;
    int size[127];
    int magic2;
    int size2[127];
}SIZE;

typedef struct
{
    int magic1;
    int lba_list[120];
}DISK_BLOCK;

int write_superblock(int superblock_pos,int num_sectors);
int read_superblock_al(int superblock_pos);
int init_alega_fs(int disk);
int format_disk_v2(int disk);
int write_file_2(char filename[10],char data[1024+1024]);

#endif
