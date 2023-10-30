#ifndef LIB_STDIO_H
#define LIB_STDIO_H

// Include necessary standard headers
#include <stdarg.h>
#include "stddef.h"
#include "stdint.h"
#include "fileio.h"
#include "memory.h"
#include "printf.h"

#define SCAN_CODE_KEY_UP         0x48
#define SCAN_CODE_KEY_DOWN         0x50
#define SCAN_CODE_KEY_ESC         0x01

// Declare the custom printf_lib function
// int printf(const char* format, ...);




#ifndef MY_CUSTOM_LIBC_H
#define MY_CUSTOM_LIBC_H



// Console I/O Functions
int puts(char *string);
int vsnprintf(char* buffer, size_t size, const char* format, va_list ap);
int test_ret(int in);
int reverse_int(int value);
char *get_char();
int clear_screen();
int get_scan_code();

// File I/O Functions (POSIX-Compliant)
int athenx_fread(char *buffer, FILE *fp, size_t size);
FILE* fopen(const char *filename, char* mode);
int athenx_fwrite(FILE *fp, char* buffer);
void fclose(FILE *fp);
int create_file(const char *path);
int fl_is_dir(const char *path);
int rm_file(const char *path);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *fp);
size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
size_t fread(void *ptr, size_t size, size_t count, FILE *stream);

// Directory Function (Non-POSIX)
int ls(char *path);

// Functions from the second header (POSIX-Compliant)
size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
size_t fread(void *ptr, size_t size, size_t count, FILE *stream);
void fclose(FILE *stream);
long ftell(FILE *stream);
int fseek(FILE *stream, long offset, int whence);
int fgetc(FILE *stream);
int fputc(int c, FILE *stream);
int fputs(const char *str, FILE *stream);
char* fgets(char *str, int size, FILE *stream);
int fprintf(FILE *stream, const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);

void perror(const char *s);

#endif


// Add other declarations for custom functions or types if needed








#define ATHENX_LIBC
#ifdef ATHENX_LIBC //these are fuction native to the ATHENX library 
long get_file_size_from_pointer(FILE *file);
int is_dir(char *path);
#endif

FILE *stdio;
FILE *stderr;
FILE *stdlog; // NOT POSIX specific, native to AthenX-2.0
init_stdio();
int init_libc();
#endif /* LIB_STDIO_H */
