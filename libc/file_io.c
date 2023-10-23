#include "fileio.h"
#include "printf.h"
#include "../include/syscall.h"
// fileio.c
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

int create_file(const char * path)
{
    syscall(SYS_CREATE, path,0);
}
int rm_file(const char * path)
{
    syscall(SYS_RM,path,0);
}
int fprintf(const char* filename, const char* format, ...) {
    char buffer[4096]; // You can adjust the buffer size as needed.
    va_list args;
    va_start(args, format);
    
    // Format the string using _vsnprintf_
    int result = vsnprintf_(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (result >= 0) {
        // Write the formatted string to the file using fwrite
        if (fwrite(filename, buffer) == 0) {
            return 0; // Success
        }
    }

    return -1; // Error
}