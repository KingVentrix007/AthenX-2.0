#include "kheap.h"
#include "string.h"
#include "math.h"
#include "ide.h"
#include "ssfs.h"
#include "string.h"
SSFS_TABLE master;
SECTOR_LIST check_list;
int write_file(int check)
{
    // struct cat
    // {
    //     char data[512];
    // };
    // char buffer[512] = {0};
    // memset(buffer,0,sizeof(buffer));
    // struct cat cc;
    // strcpy(cc.data,"I like cats");
    // memcpy(buffer,&cc,sizeof(buffer));
    // ide_write_sectors(0,1,check,(uint32)buffer);
    
}

int init_ssfs()
{
    char buffer[1024];
    memset(buffer,0,sizeof(buffer));
    ide_read_sectors(0,2,0,(uint32)buffer);
    memcpy(&master,buffer,sizeof(master));
    if(master.magic != 1001)
    {
        master.magic = 1001;
        strcpy(master.name,"TRISTAN'S DISK");
        SECTOR_LIST first;
        first.magic == 2002;
        master.free_sectors_list[0] = 3;
        for (size_t i = 0; i < 127; i++)
        {
            first.used_sectors[i] = 0;
        }
        memset(buffer,0,sizeof(buffer));
        memcpy(buffer,&master,sizeof(buffer));
        ide_write_sectors(0,2,0,(uint32)buffer);
        char fb[512];
        memset(fb,0,sizeof(fb));
        memcpy(fb,&first,sizeof(fb));
        ide_write_sectors(0,1,3,(uint32)fb);
    }
}