#include "fs.h"
#include "display.h"
#include "ide.h"
#include "fs_format.h"
#include "string.h"
SUPER_BLOCK super_block;
PARTITION_TABLE partition_table;
int format_disk_main(int disk_num)
{
    int num_sectors = get_sectors(disk_num);
    int num_partitions = (num_sectors-FILE_SECTOR_BASE) / 124;
    printf("\nPARTITION_TABLE size is %d\n", sizeof(PARTITION_TABLE)); // DEBUG info
    //printf("Number of sectors %d, number of partitions %d\n", num_sectors, num_partitions);
    super_block.num_partitions = 124; // Number of partitions, Currently capped at 124 (max values that can be stored in in the array)
    super_block.first_partition = FILE_SECTOR_BASE+1; // The first partition
    strcpy(super_block.is_formated,"true"); // Value set so os dons't over right partition
    int partition_lba = FILE_SECTOR_BASE+1; // Counter to set LBA of consecutive partitions
    for (uint32 part = 0; part < 124; part++) // Loop and write partitions
    {
        char table[512] = {0}; // buffer for partition table
        memset(table, 0, sizeof(table)); // insure all elements are 0 or NULL
        super_block.partition_start_lbas[part] = partition_lba; // Store partition in SUPER_BLOCK list of partition table LBA's
        partition_table.partition_number = part; // Stets the partitions partition number
        partition_table.last_block = partition_lba+126; // Sets the Last sector in the current partition
        memcpy(table, &partition_table, sizeof(partition_table)); // Copy partition table into buffer
        ide_write_sectors(disk_num,1,partition_lba,(uint32_t)table); // Write partition to disk
        partition_lba = partition_lba + 127; // Increment partition counter
        //printf("%d|",partition_lba); // Print partition counter for DEBUG purposes
        

    }
    char super_block_buffer[ATA_SECTOR_SIZE] = {0}; // SUPER_BLOCK buffer
    memset(super_block_buffer, 0, sizeof(super_block_buffer)); // insures all elements are 0 or NULL
    memcpy(super_block_buffer, &super_block, sizeof(super_block));// copy SUPER_BLOCK into buffer
    ide_write_sectors(disk_num,1,FILE_SECTOR_BASE,(uint32_t)(super_block_buffer)); // Write superblock to disk
    printf("Wrote super block to sector %d\n",FILE_SECTOR_BASE);// print SUPER_BLOCK pos for debug purposes
    
    // super_block.num_partitions = 429496729;
    //printf("Sieze of superblock --> %d\n%d\n", sizeof(999999999),super_block.num_partitions);

}

int fwrite(int drive,char filename[8], char filetype[3],char data[MAX_FILE_SIZE])
{
    char data_to_write[ATA_SECTOR_SIZE] = {0}; // This will contain the data to write to the disk
    memset(data_to_write, 0, sizeof(data_to_write)); // Make sure all values are 0 or NULL
    int file_size = strlen(data); // Get the size of the data to be written
    int required_sectors = file_size / ATA_SECTOR_SIZE; // Get the required number of sectors
    char partition_info[ATA_SECTOR_SIZE] = {0}; // Buffer to hold the partition information
    struct FILE_HEADER_V1 file; // File header
    struct BLOCK file_block;
    strcpy(file.filename,filename); // Define file name
    strcpy(file.file_type,filetype); // Type of file
    file.file_size = file_size; // Size of file
    file.num_sectors = required_sectors; // number of sectors
    //strcpy(file.dictionary,)
    for (uint32 i = 0; i < 124; i++) // Loop through SUPER_BLOCKS partition list
    {
        int partition_lba = super_block.partition_start_lbas[i];
        memset(partition_info,0,sizeof(partition_info)); // Make sure all values are 0 or NULL
        ide_read_sectors(drive,1,partition_lba,(uint32_t)partition_info); // Read the partition information
        memcpy(&partition_table, partition_info, sizeof(partition_table)); // Load the partition information in partition_table
        if(partition_table.num_free_sectors >= required_sectors) // Checks to insure enough free space in partition table
        {
            int count = 0; // define count
            int sectors[125] = {0}; // List of sectors to be written
            for (uint32 q = 1; q < 124; q++) // loops through partition sectors
            {
                int current_sector = partition_lba+q; // current sector number
                
                if(isInArray(current_sector, partition_table.used_blocks,125) != 0) // is sector free?
                {
                    sectors[count] = current_sector; // If so store sector in array
                    count = count + 1; // increment count
                }
            }
            int start = 0; // initialize a count 
            for (size_t x = 0; x < (strlen(data)/ATA_SECTOR_SIZE)+1; x++)
            {
               
                char data_out[512] = {0}; // data that will be written in this loop
                memset(data_out,0,sizeof(data_out)); // set all data to 0 or NULL
                for (size_t w = start; w < start + 511 ; w++)
                {
                    append(data_out,data[w]);// append data to the end of data_out
                }
                append(data_out,'\0'); // Terminate sector
                start = start + 511; // increment start
                strcpy(file_block.data, data_out); // Put data into file block
                memset(data_to_write,0,sizeof(data_to_write)); // set data_out to zero
                memcpy(data_to_write, &file_block, sizeof(file_block)); // copy file_block into data_to_write
                ide_write_sectors(drive,1,sectors[1],(uint32_t)data_to_write); // write data to sector
                //printf("Count: %d\n",count);
                

            }
            
        }
    }
    



}