#ifndef FS_H
#define FS_H
#include "stdint.h"
#include "types.h"
typedef struct FILE
{
    char filename[8];
    char data[ATA_SECTOR_SIZE-8-5-8];
    char is_file[5];
    int next_sector;
};
typedef struct fs_table
{
    int used_sectors[900];

};

typedef struct format_table
{
    char is_format[4];
    char format[20];
    int sector_size;



};
typedef struct FILE_V2
{
    char filename[8];
    char data[ATA_SECTOR_SIZE-8-5-8-8-3];
    char is_file[5];
    char dictionary[8];
    char file_type[3];
    int next_sector;
};
typedef struct DICTIONARY
{
    char dict_name[8];
    char flies[20][8];
};
int write(char filename[8],char file_type[3],char data[512-8]);
int run_once();
int read(char filename[8]);
int init_fs();
int update_table();
int list_files();
int fs_master_table_update();
int fs_master_table_p();
int format_disk();
delete_file(char *filename[8]);
int clean_fs_master_table(int num);
int make_dir(char *dir_name[8]);
#endif
