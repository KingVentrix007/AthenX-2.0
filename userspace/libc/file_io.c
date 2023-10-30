#include "fileio.h"
#include "printf.h"
 #include "syscall.h"
#include "stdio.h"
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







/**
 * Function Name: fgetc
 * Description: Reads the next character from the specified file.
 *
 * Parameters:
 *   stream (FILE*) - A pointer to the file stream from which to read.
 *
 * Return:
 *   On success, the character read is returned as an unsigned char cast to an int. If the end of the file is reached or an error occurs, it returns EOF.
 */
int fgetc(FILE* stream)
{
    return syscall(SYS_FGETC, stream,0);
}

/**
 * Function Name: fputc
 * Description: Writes a character to the specified file.
 *
 * Parameters:
 *   c (int) - The character to be written.
 *   stream (FILE*) - A pointer to the file stream where the character will be written.
 *
 * Return:
 *   On success, the character written is returned. If an error occurs, it returns EOF.
 */
int fputc(int c, FILE* stream)
{
    return syscall(SYS_FPUTC,stream,c);
}

/**
 * Function Name: fputs
 * Description: Writes a string to the specified file.
 *
 * Parameters:
 *   str (const char*) - The string to be written.
 *   stream (FILE*) - A pointer to the file stream where the string will be written.
 *
 * Return:
 *   On success, a non-negative value is returned. If an error occurs, it returns EOF.
 */
int fputs(const char* str, FILE* stream)
{
   return syscall(SYS_FPUTS,str,stream);
}

/**
 * Function Name: fgets
 * Description: Reads a line of text from the specified file and stores it in the provided buffer.
 *
 * Parameters:
 *   str (char*) - A pointer to the buffer where the read line is stored.
 *   size (int) - The maximum number of characters to be read, including the null-terminating character.
 *   stream (FILE*) - A pointer to the file stream from which to read.
 *
 * Return:
 *   On success, it returns the same `str` pointer. If an error occurs or the end of the file is reached, it returns NULL.
 */
char* fgets(char* str, int size, FILE* stream)
{
    struct syscall p;
    p.parameter1 = str;
    p.parameter2 = size;
    return syscall(SYS_FGETS, &p, stream);
}
/**
 * Function Name:fscanf
 * Description: Custom implementation of fscanf function using provided functions.
 *
 * Parameters:
 *   stream (FILE*) - The file pointer from which to read data.
 *   format (const char*) - The format string specifying the data to be read.
 *   ... - Variable number of pointers to variables where the read data will be stored.
 *
 * Return:
 *   Returns the number of input items successfully matched and assigned.
 */
int fscanf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int numItemsRead = 0;
    int c;
    char *buffer = malloc(get_file_size_from_pointer(stream));  // Temporary buffer for reading characters

    while (*format) {
        if (*format == '%') {
            format++;  // Move past '%'
            if (*format == 'd') {
                // Read an integer
                int *intPtr = va_arg(args, int*);
                int value = 0;
                int sign = 1;

                // Read characters until a non-digit character is encountered
                c = fgetc(stream);
                while (c != EOF && (c == ' ' || c == '\t' || c == '\n'))
                    c = fgetc(stream);
                if (c == '-') {
                    sign = -1;
                    c = fgetc(stream);
                }
                while (c != EOF && (c >= '0' && c <= '9')) {
                    value = value * 10 + (c - '0');
                    c = fgetc(stream);
                }
                *intPtr = value * sign;
                numItemsRead++;
            }
            else if (*format == 's') {
                // Read a string
                char *strPtr = va_arg(args, char*);
                int maxLen = 255;  // Maximum length of string
                int len = 0;
                c = fgetc(stream);
                while (c != EOF && (c == ' ' || c == '\t' || c == '\n'))
                    c = fgetc(stream);
                while (c != EOF && c != ' ' && c != '\t' && c != '\n') {
                    if (len < maxLen - 1) {
                        strPtr[len] = c;
                        len++;
                    }
                    c = fgetc(stream);
                }
                strPtr[len] = '\0';
                numItemsRead++;
            }
            else {
                // Unsupported format specifier
                break;
            }
        }
        else {
            // Read and compare a character from the file
            c = fgetc(stream);
            if (c == *format) {
                numItemsRead++;
            }
            else if (c == EOF) {
                break;
            }
        }
        format++;
    }

    va_end(args);
    free(buffer);
    return numItemsRead;
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

int is_dir(char *path)
{
    return syscall(SYS_IS_PATH, path,0);
}
#endif