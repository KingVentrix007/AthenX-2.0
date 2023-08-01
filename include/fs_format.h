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
    unsigned int used_blocks[120];
    unsigned int next_partition;
    unsigned int partition_number;



}PARTITION_TABLE;


int format_disk_main(int disk_num); // formats the given disk
#endif