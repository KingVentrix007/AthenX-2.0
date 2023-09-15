#ifndef __SSFS__H
#define __SSFS__H
#include "types.h"
// Define the size of a sector (adjust according to your needs)
typedef struct
{
    char filename[100];
    int magic;
    unsigned int data_blocks[sizeof(int)/2+sizeof(char)*100];
}SSFS_FILE;
typedef struct
{
    char name[100];
    int magic;
    unsigned int free_sectors_list[(1024-100-8)/4];

    
}SSFS_TABLE;

typedef struct 
{
    int magic;
    unsigned int used_sectors[(512-sizeof(int))/sizeof(int)];
}SECTOR_LIST;



int init_ssfs();



int write_file(int check);
#endif