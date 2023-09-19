#include "installer.h"
#include "types.h"
#include <stddef.h>
#include "string.h"
#include "ide.h"

/**
 * Function Name: install
 * Description: installs AthenX from one disk to another
 *
 * Parameters:
 *   disk_1 (int) - the disk to install from
 *   disk_2 (int) - the disk to install to
 *   size (uint32) - the size of the install
 * 
 *
 * Return:
 *   none
 */
int install(int,int,uint16_t) {
    int total_sectors_on_source_disk = 26010; 
    // Define variables for source and destination drives
    uint8 source_drive = 0; // Replace with the appropriate source drive number
    uint8 dest_drive = 1;   // Replace with the appropriate destination drive number

    // Specify the number of sectors to copy in one go
    uint8 num_sectors_to_copy = 1;

    // Define a buffer to hold data from the source disk
    uint32 buffer[512]; // Assuming a 512-byte sector size

    // Loop through all sectors on the source disk and copy to the destination disk
    uint32 sector;
    for (sector = 0; sector < total_sectors_on_source_disk; sector++) {
        // Read a sector from the source disk
        int read_result = ide_read_sectors(source_drive, num_sectors_to_copy, sector, buffer);
        if (read_result != 0) {
            // Handle read error
            // You can add error handling code here.
        }

        // Write the sector data to the destination disk
        int write_result = ide_write_sectors(dest_drive, num_sectors_to_copy, sector, buffer);
        if (write_result != 0) {
            // Handle write error
            // You can add error handling code here.
        }
    }

    // Close both disks if needed
    // You can add code here to close the disks if your operating system requires it.

    return 0;
}