#ifndef __SYSCALL__H
#define __SYSCALL__H
#include "types.h"
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
typedef struct param_struct
{
    char* param1;
    int param2;
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

int syscall(int syscall_number, int param1, int param2);
#endif