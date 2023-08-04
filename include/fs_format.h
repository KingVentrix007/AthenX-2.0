#ifndef FS_FORMAT
#define FS_FORMAT

typedef struct
{
    unsigned long int partition_start_lbas[124]; //496 bytes
    unsigned long int num_partitions; //4 bytes
    unsigned long int first_partition; // 4 bytes
    char is_formated[5]; 


}SUPER_BLOCK;

typedef struct
{
    unsigned int used_blocks[125]; //  500 bytes
    unsigned int last_block; // 4 bytes
    unsigned int partition_number; // 4 bytes
    unsigned int num_free_sectors; // 4 bytes



}PARTITION_TABLE;


int format_disk_main(int disk_num); // formats the given disk
#endif