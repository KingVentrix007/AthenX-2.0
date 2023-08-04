#include "fs.h"
#include "display.h"
#include "ide.h"
#include "fs_format.h"
#include "string.h"
SUPER_BLOCK super_block;
PARTITION_TABLE partition_table;
int format_disk_main(int disk_num)
{
    

}

// int fwrite(int drive,char filename[8], char filetype[3],char data[MAX_FILE_SIZE])
// {
//     char data_to_write[ATA_SECTOR_SIZE] = {0}; // This will contain the data to write to the disk
//     memset(data_to_write, 0, sizeof(data_to_write)); // Make sure all values are 0 or NULL
//     int file_size = strlen(data); // Get the size of the data to be written
//     int required_sectors = file_size / ATA_SECTOR_SIZE; // Get the required number of sectors
//     char partition_info[ATA_SECTOR_SIZE] = {0}; // Buffer to hold the partition information
//     struct FILE_HEADER_V1 file; // File header
//     struct BLOCK file_block;
//     strcpy(file.filename,filename); // Define file name
//     strcpy(file.file_type,filetype); // Type of file
//     file.file_size = file_size; // Size of file
//     file.num_sectors = required_sectors; // number of sectors
//     //strcpy(file.dictionary,)
//     for (uint32 i = 0; i < 124; i++) // Loop through SUPER_BLOCKS partition list
//     {
//         int partition_lba = super_block.partition_start_lbas[i];
//         memset(partition_info,0,sizeof(partition_info)); // Make sure all values are 0 or NULL
//         ide_read_sectors(drive,1,partition_lba,(uint32_t)partition_info); // Read the partition information
//         memcpy(&partition_table, partition_info, sizeof(partition_table)); // Load the partition information in partition_table
//         if(partition_table.num_free_sectors >= required_sectors) // Checks to insure enough free space in partition table
//         {
//             int count = 0; // define count
//             int sectors[125] = {0}; // List of sectors to be written
//             for (uint32 q = 1; q < 124; q++) // loops through partition sectors
//             {
//                 int current_sector = partition_lba+q; // current sector number
                
//                 if(isInArray(current_sector, partition_table.used_blocks,125) != 0) // is sector free?
//                 {
//                     sectors[count] = current_sector; // If so store sector in array
//                     count = count + 1; // increment count
//                 }
//             }
//             int start = 0; // initialize a count 
//             for (size_t x = 0; x < (strlen(data)/ATA_SECTOR_SIZE)+1; x++)
//             {
               
//                 char data_out[512] = {0}; // data that will be written in this loop
//                 memset(data_out,0,sizeof(data_out)); // set all data to 0 or NULL
//                 for (size_t w = start; w < start + 511 ; w++)
//                 {
//                     append(data_out,data[w]);// append data to the end of data_out
//                 }
//                 append(data_out,'\0'); // Terminate sector
//                 start = start + 511; // increment start
//                 strcpy(file_block.data, data_out); // Put data into file block
//                 memset(data_to_write,0,sizeof(data_to_write)); // set data_out to zero
//                 memcpy(data_to_write, &file_block, sizeof(file_block)); // copy file_block into data_to_write
//                 ide_write_sectors(drive,1,sectors[1],(uint32_t)data_to_write); // write data to sector
//                 //printf("Count: %d\n",count);
                

//             }
            
//         }
//     }
    



// }