#include "fileio.h"
#include "../include/syscall.h"

int fread(char *buffer,FILE *fp,size_t size)
{
   
    parameters *parms;
    // parms->param1 = buffer;
    // parms->param2 = size;
    // printf("1\n");
    char c;
    int count = 0;
    int ret = syscall(SYS_READ,buffer,fp);
    // printf("ret %d\n",ret);
    return ret;
    // while(c != -1 && count <= size*1)
    // {
    //     c = (char)syscall(SYS_READ, 0, fp);
    //     count++;
    //     printf("CC%c",c);
        
     

    // }

}

FILE* fopen(const char *filename,char* mode)
{
    FILE *fp;
    fp = syscall(SYS_OPEN, filename, mode);
    return fp;

}
int fwrite(const char *filename,char* buffer)
{
    syscall(SYS_WRITE, filename, buffer);
}
void fclose(FILE *fp)
{
    syscall(SYS_CLOSE,fp,0);
}