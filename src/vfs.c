
#include "ext2.h"
#include "fs.h"

#include "vfs.h"
#include "ide.h"
int init_disk(int disk)
{
     int init_fs_ret = 0;
        read_superblock();
        init_fs_ret = ext2_init();
        if(init_fs_ret != 0)
        {
            //printf("Disk %d is not formatted with Algea-fs, trying ext2\n",disk);
           
            // init_fs_ret =  
            // if(init_fs_ret != 0)
            // {
            //     //printf("Disk %d is not formatted with ext2, trying FAT32\n",disk);
            //     //fl_init();
            //     // if (fl_attach_media(ide_read_sectors_fat, ide_write_sectors_fat) != FAT_INIT_OK)
            //     // {
            //     //    init_fs_ret = -1;
            //     //    printf("Disk %d is not formatted with FAT\n",disk);
            //     //    return 3;


            //     // }
            //     // else
            //     // {
            //     //     printf("Disk %d is formatted with FAT\n",disk);
            //     //     printf("Type format('Desired format') to format the disk");
            //     //     
            //     // }
                return -1;
            }
            else
            {
                printf("Disk %d is formatted with ext2\n",disk);
                
                return 2;
            }
                
        }
        // else
        // {
             
        //     printf("Disk %d is formatted with Alega-fs\n",disk);
        //     return 1;
        // }

FILES_SYSTEM file_system_info;
int file_system_mode = -1;
int initialize_file_system(int disk)
{
    int mode = init_disk(disk);
    file_system_info.mode = mode;
    file_system_info.num_sectors = get_sectors(disk);
    file_system_mode = mode;
    return mode;


}

int get_fs_mode()
{
    return file_system_mode;
}