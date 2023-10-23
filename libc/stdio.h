#ifndef LIB_STDIO_H
#define LIB_STDIO_H

// Include necessary standard headers
#include <stdarg.h>
#include "stddef.h"
#include "stdint.h"
#include "fileio.h"
// Declare the custom printf_lib function
// int printf(const char* format, ...);

int puts(char *string);
int vsnprintf(char* buffer, size_t size, const char* format, va_list ap) ;
int  test_ret(int in);
int reverse_int(int value);
char *get_char();
int clear_screen();
int ls(char *path);
int fread(char *buffer,FILE *fp,size_t size);
FILE* fopen(const char *filename,char* mode);
int fwrite(const char *filename,char* buffer);
void fclose(FILE *fp);
int create_file(const char * path);
int fl_is_dir(char *path);
int rm_file(const char * path);
int fprintf(const char* filename, const char* format, ...) ;
// Add other declarations for custom functions or types if needed

#endif /* LIB_STDIO_H */
