#include "fileio.h"
#include "printf.h"
#include "../../include/syscall.h"
// fileio.c
#define ATHENX_LIBC
int athenx_fread(char *buffer,FILE *fp,size_t size)
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
int athenx_fwrite(FILE *fp,char* buffer)
{
    syscall(SYS_WRITE, fp, buffer);
}

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream) {

    struct file_io file;
    file.ptr = ptr;
    file.size = size;
    file.count = count;
    return syscall(SYS_FWRITE,stream,&file);
    
}
size_t fread(void *ptr, size_t size, size_t count, FILE *stream) {
    struct file_io file;
    file.ptr = ptr;
    file.size = size;
    file.count = count;

    size_t items_read = syscall(SYS_FREAD, stream, &file);

    // Check if items were read successfully and handle errors if necessary
    if (items_read < 0) {
        // Handle the error here, return an error code, or perform error handling as needed.
        printf("zero bytes read\n");
        return items_read;
    }

    return items_read; // Return the number of items read.
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
    
    // Format the string using vsnprintf
    int result = vsnprintf_(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (result >= 0) {
        // Write the formatted string to the file using fwrite
        size_t len = strlen(buffer);
        size_t bytes_written = fwrite(buffer, sizeof(char), len, fp);
        
        if (bytes_written == len) {
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
#ifdef ATHENX_LIBC
long get_file_size_from_pointer(FILE *file) {
    if (file == NULL) {
        return -1; // Return -1 to indicate that the file pointer is invalid
    }

    long current_position = ftell(file); // Get the current position
    fseek(file, 0, SEEK_END); // Move the file pointer to the end of the file
    long size = ftell(file); // Get the current position, which is the size
    fseek(file, current_position, SEEK_SET); // Return the file pointer to its original position

    return size;
}
#endif