#ifndef __SYSCALL__H
#define __SYSCALL__H
#include "types.h"
#include "stddef.h"
#define SYS_EXIT 1
#define SYS_FORK 2
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 7
#define SYS_MMAP 8
#define SYS_MUNMAP 9
#define SYS_DUP 10
#define SYS_DUP2 11
#define SYS_EXECVE 12
#define SYS_WAITPID 13
#define SYS_GETPID 14
#define SYS_GETPPID 15
#define SYS_GETUID 16
#define SYS_GETGID 17
#define SYS_SETUID 18
#define SYS_SETGID 19
#define SYS_SLEEP 20
#define SYS_PRINT 21
#define SYS_GETCHAR 22
#define SYS_SCREEN_CTRL 23
#define SYS_PUTCHAR 24
#define SYS_LIST_DIR 25
#define SYS_IS_DIR 26
#define SYS_CREATE 27
#define SYS_RM 28
#define SYS_TERMINAL 29
#define SYS_TELL 30
#define SYS_GET_SCAN 31
#define SYS_REALLOC 32
#define SYS_CALLOC 33
#define SYS_ALLIGEND_MEM 34
#define SYS_FTELL 35
#define SYS_FSEEK 36
#define SYS_LAZY_MAN 37
#define SYS_FWRITE 38
#define SYS_FREAD 39
#define SYS_IS_PATH 40
#define SYS_FGETC 41
#define SYS_FGETS 42
#define SYS_FPUTS 43
#define SYS_FPUTC 44
#define SYS_SET_FONT_C 45
#define SYS_SET_BG_COLOR 46
#define SYS_SET_X 47
#define SYS_SET_Y 48
#define SYS_GET_X 49
#define SYS_GET_Y 50
typedef struct param_struct
{
    char* param1;
    int param2;
    long param3;
    
    // int param3;
    // int param4;
}parameters;
typedef struct term_struct
{
    /* data */
    int x;
    int y;
    uint32_t color;
}terminal_struct;

struct file_io {
    const void *ptr;
    size_t size;
    size_t count;
};
struct syscall
{
    int num_paraters; // the total number of parameters
    int all_one_type; // all the parameters are one type
    uint32 parameter1; // the first parameter
    uint32 parameter2; // the second parameter
    uint32 parameter3; // the third parameter
    uint32 pointer; // Pointer to anther structure
};

typedef struct {
    uint32_t font_color;  // Font color (RGB value)
    uint32_t bg_color;    // Background color (RGB value)
    int echo_mode;        // Echo mode (1 for on, 0 for off)
    int auto_scroll;      // Auto-scroll mode (1 for on, 0 for off)
    // Add any other parameters you need here
} AthenXTerminal;

int syscall(int syscall_number, int param1, int param2);
#endif