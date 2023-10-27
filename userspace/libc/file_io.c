#include "fileio.h"
#include "printf.h"
#include "../../include/syscall.h"
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
int fwrite(FILE *fp,char* buffer)
{
    syscall(SYS_WRITE, fp, buffer);
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
int fprintf(FILE *fp, const char* format, ...) {
    char buffer[4096]; // You can adjust the buffer size as needed.
    memset(buffer, 0, sizeof(buffer));
    va_list args;
    va_start(args, format);
    
    // Format the string using _vsnprintf_
    int result = vsnprintf_(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (result >= 0) {
        // Write the formatted string to the file using fwrite
        if (fwrite(fp, buffer) == 0) {
            return 0; // Success
        }
    }

    return -1; // Error
}




/**
 * Function Name: ftell
 * Description: Returns the current file position indicator of the given stream.
 *
 * Parameters:
 *   stream (FILE*) - A pointer to the FILE object representing the file.
 *
 * Return:
 *   long - The current file position indicator if successful, or -1L if an error occurs.
 */
long ftell(FILE *fp)
{
    syscall(SYS_FTELL,fp,0);
}

/**
 * Function Name: fseek
 * Description: Repositions the file pointer associated with the given stream.
 *
 * Parameters:
 *   stream (FILE*) - A pointer to the FILE object representing the file.
 *   offset (long) - The number of bytes to move the file pointer.
 *   whence (int) - The reference point for the offset (SEEK_SET, SEEK_CUR, or SEEK_END).
 *
 * Return:
 *   int - 0 if successful, or a non-zero value if an error occurs.
 */
int fseek(FILE *f, long offset, int whence)
{
    parameters *parms;
    parms->param3 = offset;
    parms->param2 = whence;
    syscall(SYS_FSEEK, f, parms);
}