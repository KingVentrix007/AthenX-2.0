#ifndef __UNISTD__H
#define __UNISTD__H
#include "stdint.h"
#include "sys/types.h"
/**
 * Description: Constant for checking file existence.
 */
#define F_OK 0

/**
 * Description: Constant for checking read permission.
 */
#define R_OK 4

/**
 * Description: Constant for checking write permission.
 */
#define W_OK 2

/**
 * Description: Constant for checking execute permission.
 */
#define X_OK 1

intptr_t fork(void);
int execv(const char* path, char* const argv[]);
int execve(const char* path, char* const argv[], char* const envp[]);
int execvp(const char* file, char* const argv[]);
pid_t getpid(void);
int access(const char *path, int mode);
#endif