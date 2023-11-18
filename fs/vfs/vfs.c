
// #include "../include/ext2.h"
#include "../include/fs.h"

#include "../include/vfs.h"
#include "../include/ide.h"
int fs_type;
int init_disk(int disk)
{
     int init_fs_ret = 0;
        //read_superblock();
        //init_fs_ret = ext2_init();
        if(init_fs_ret != 0)
        {
            //printf_("Disk %d is not formatted with Algea-fs, trying ext2\n",disk);
           
            // init_fs_ret =  
            // if(init_fs_ret != 0)
            // {
            //     //printf_("Disk %d is not formatted with ext2, trying FAT32\n",disk);
            //     //fl_init();
            //     // if (fl_attach_media(ide_read_sectors_fat, ide_write_sectors_fat) != FAT_INIT_OK)
            //     // {
            //     //    init_fs_ret = -1;
            //     //    printf_("Disk %d is not formatted with FAT\n",disk);
            //     //    return 3;


            //     // }
            //     // else
            //     // {
            //     //     printf_("Disk %d is formatted with FAT\n",disk);
            //     //     printf_("Type format('Desired format') to format the disk");
            //     //     
            //     // }
                return -1;
            }
            else
            {
                printf_("Disk %d is formatted with ext2\n",disk);
                
                return 2;
            }
                
}
        // else
        // {
             
        //     printf_("Disk %d is formatted with Alega-fs\n",disk);
        //     return 1;
        // }

