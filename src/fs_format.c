#include "fs.h"
#include "display.h"
#include "ide.h"
#include "fs_format.h"
SUPER_BLOCK super_block;
int format_disk_main(int disk_num)
{
    int num_sectors = get_sectors(disk_num);
    int num_partitions = (num_sectors-FILE_SECTOR_BASE) / 124;
    printf("Number of sectors %d, number of partitions %d\n", num_sectors, num_partitions);
    super_block.num_partitions = num_partitions;
    super_block.first_partition = FILE_SECTOR_BASE+1;
    for (uint32 part = 0; part < num_partitions; part++)
    {
        /* code */
    }
    
    // super_block.num_partitions = 429496729;
    //printf("Sieze of superblock --> %d\n%d\n", sizeof(999999999),super_block.num_partitions);

}