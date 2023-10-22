#ifndef __FILE__IO__H
#define __FILE__IO__H
#include <stdarg.h>
#include <stddef.h>
#include "string.h"
struct fat_node
{
    struct fat_node    *previous;
    struct fat_node    *next;
};
struct cluster_lookup
{
    uint32 ClusterIdx;
    uint32 CurrentCluster;
};
typedef struct FL_FILE
{
    uint32                  parentcluster;
    uint32                  startcluster;
    uint32                  bytenum;
    uint32                  filelength;
    int                     filelength_changed;
    char                    path[260];
    char                    filename[260];
    uint8                   shortfilename[11];

#ifdef FAT_CLUSTER_CACHE_ENTRIES
    uint32                  cluster_cache_idx[FAT_CLUSTER_CACHE_ENTRIES];
    uint32                  cluster_cache_data[FAT_CLUSTER_CACHE_ENTRIES];
#endif

    // Cluster Lookup
    struct cluster_lookup   last_fat_lookup;

    // Read/Write sector buffer
    uint8                   file_data_sector[50];
    uint32                  file_data_address;
    int                     file_data_dirty;

    // File fopen flags
    uint8                   flags;
#define FILE_READ           (1 << 0)
#define FILE_WRITE          (1 << 1)
#define FILE_APPEND         (1 << 2)
#define FILE_BINARY         (1 << 3)
#define FILE_ERASE          (1 << 4)
#define FILE_CREATE         (1 << 5)

    struct fat_node         list_node;
} FILE;
int fread(char *buffer,FILE *fp,size_t size);
FILE* fopen(const char *filename,char* mode);
int fwrite(const char *filename,char* buffer);
void fclose(FILE *fp);

#endif

