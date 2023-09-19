#include "kheap.h"
#include "string.h"
#include "math.h"
#include "ide.h"
#include "ssfs.h"
#include "string.h"
#include "debug.h"
#include "errno.h"
int free_sectors[227];
int start = 0;
partition_table tab;
void padString(char *str, int desiredLength, char paddingChar) {
    int currentLength = strlen(str);

    // Check if the string is shorter than the desired length
    if (currentLength < desiredLength) {
        int paddingAmount = desiredLength - currentLength;
        if (paddingAmount > 0) {
            // Add padding characters at the end of the string
            for (int i = 0; i < paddingAmount; i++) {
                str[currentLength + i] = paddingChar;
            }
            str[currentLength + paddingAmount] = '\0';  // Null-terminate the new length
        }
    }
}
int ssfs_format_disk(int disk_num, char disk_name[100]) {
    if(strlen(disk_name) < 100)
    {
        padString(disk_name,100,' ');
    }
    FUNC_ADDR_NAME(&ssfs_format_disk,2,"is");
    partition_table table;
    printf("%d",sizeof(table));
    char partion_buffer[1024] = {0};
    table.disk_size = get_sectors(disk_num);

    table.file_start = 3;
    table.magic1 = 9;
    table.partition_size = 1024;
    strcpy(table.diskname,disk_name);
    for (size_t i = 3; i < 3+227 ; i++)
    {
        table.free_sectors[i-3] = i;
        if(i > 101)
        {
            //printf("%d\n",i);
        }
    }
    for (size_t q = 0; q < 227 ; q++)
    {
        //table.free_sectors[i-3] = i;
         //printf("%d\n",table.free_sectors[q]);
    }
   
    memset(partion_buffer,2,sizeof(partion_buffer));
    memcpy(partion_buffer,&table,sizeof(partion_buffer));
    int ret = ide_write_sectors(disk_num,2,0,(uint32)partion_buffer);
    
    return main_exit(SUCCESS,SUCCESS,__FUNCTION__,true,decode_error(0));
    
}

int read_ssfs_partion(partition_table part_table,int location,int drive)
{
    char buffer[1024] = {0};
    memset(buffer,0,1024);
    ide_read_sectors(drive,2,location,(uint32)buffer);
    memcpy(&part_table,buffer,sizeof(part_table));
    printf("%s\n",part_table.diskname);
    printf("%d\n",part_table.magic1);
    for (size_t i = 0; i < sizeof(part_table.free_sectors)/sizeof(int); i++)
    {
        //printf("%d\n",part_table.free_sectors[i]);
    }
    memcpy(free_sectors,part_table.free_sectors,sizeof(free_sectors));
    for (size_t i = 0; i < sizeof(part_table.free_sectors)/sizeof(int); i++)
    {
        //printf("%d\n",free_sectors[i]);
    }
    start = part_table.file_start;
    memcpy(&tab,&padString,sizeof(tab));
    
}

int mount(int part_num)
{

}
int find_partion(int drive)
{

}
int is_sector_free(int lba)
{
     for(size_t pos_in_list = 0; pos_in_list < 227; pos_in_list++)
     {
        if(free_sectors[pos_in_list] == lba)
        {
            free_sectors[pos_in_list] = 0;
            return 0;
        }
     }
     return 1;
        

}
int ssfs_write_file(file_header *header, char *data)
{
    FUNC_ADDR_NAME(&ssfs_write_file,2,"us");
    //printf("ERROR");
    int num_sectors_needed = (sizeof(data)/512)+1;
    if(num_sectors_needed <=10)
    {
        int file_header_pos = 0;
         for (size_t lba = start; lba < start+227; lba++)
        {
               int ret = is_sector_free(lba);
               if(ret == 0)
               {
                file_header_pos = lba;
                break;
               }
               else
               {

               }
        }
        int sectors[10];
        for (size_t i = 0; i < num_sectors_needed; i++)
        {
            for (size_t lba = start; lba < start+227; lba++)
            {
               int ret = is_sector_free(lba);
               if(ret == 0)
               {
                sectors[i] = lba;
                break;
               }
               else
               {

               }
            }
            //printf("sector %d\n",sectors[i]);
        }
        memcpy(header->lba_entries,sectors,sizeof(sectors));
       splitStringIntoChunks(data,sectors);
       DEBUG("HERE");
       char header_buf[512] = {0};
       memset(header_buf,0,sizeof(header_buf));
       memcpy(header_buf,header,sizeof(header_buf));
       ide_write_sectors(0,1,file_header_pos,(uint32)header_buf);
       printf("%d",file_header_pos);
        

        
        
    }
    
    
    
    
}
int ssfs_update_table()
{
    char buffer[1024] = {0};
    memset(buffer,0,1024);
    for (size_t i = 0; i < 227; i++)
    {
         tab.free_sectors[i] = free_sectors[i];
    
   
    }
    memcpy(buffer,&tab,sizeof(buffer));
    ide_write_sectors(0,2,0,(uint32)buffer);
}
int read_file_data(int lbas[10],char out[512*10])
{
    for (size_t i = 0; i < 10; i++)
    {
        if(lbas[i] >= 3)
        {
            //printf("\nLBA -- >%d",lbas[i]);
             char temp[512] = {0};
            memset(temp,0,512);
            ide_read_sectors(0,1,lbas[i],(uint32)temp);
            strcat(out,temp);
        }
       
    }
    
}
int ssfs_read(char file_name[100], char out[512*10])
{
    //padString(file_name,100,' ');
    for (size_t i = 3; i < 227+3; i++)
    {
        printf("%d ",i);
        file_header head;
        char buffer[512] = {0};
        memset(buffer,0,sizeof(buffer));
        ide_read_sectors(0,1,i,(uint32)buffer);
        memcpy(&head,buffer,sizeof(head));
        if(strcmp(head.file_name,file_name) == 0)
        {
            //printf("i --> %d",i);
            read_file_data(head.lba_entries,out);
            break;
        }
    }
    
}
#define CHUNK_SIZE 512
void splitStringIntoChunks(const char* inputString, int lab_list[10]) {
    int length = strlen(inputString);
    int currentIndex = 0;
    int count = 0;
    while (currentIndex < length) {
        // Calculate the size of the chunk
        int chunkSize = (length - currentIndex) < CHUNK_SIZE ? (length - currentIndex) : CHUNK_SIZE;

        // Create a buffer for the chunk
        char chunk[CHUNK_SIZE + 1]; // +1 for null-terminator

        // Copy the chunk from the input string
        strncpy(chunk, inputString + currentIndex, chunkSize);

        // Null-terminate the chunk
        chunk[chunkSize] = '\0';

        // Print or process the chunk (here, we just print it)
        printf("Chunk: %s\n", chunk);
        if(lab_list[count] >= 3)
        {
            ide_write_sectors(0,1,lab_list[count],(uint32)chunk);
            count++;
        }
        
        // Move to the next chunk
        currentIndex += chunkSize;
    }
}