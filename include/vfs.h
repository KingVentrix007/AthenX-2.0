#ifndef VFS_H
#define VFS_H

typedef struct
{
    int mode;
    unsigned int long long num_sectors;
    char mode_char[10];
}FILES_SYSTEM;



int initialize_file_system(int disk);
#endif