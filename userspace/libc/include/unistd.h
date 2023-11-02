#ifndef __UNISTD__H
#define __UNISTD__H
#include "stdint.h"
#include "sys/types.h"
intptr_t fork(void);
int execv(const char* path, char* const argv[]);
int execve(const char* path, char* const argv[], char* const envp[]);
int execvp(const char* file, char* const argv[]);
pid_t getpid(void);
#endif