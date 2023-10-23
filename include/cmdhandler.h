#ifndef CMDHANDLER_H
#define CMDHANDLER_H

#include "../include/types.h"
void cmd_handler(char *buffer[512],char programs[100][20]);
void kernel_command_handler(char *buffer[512]);
char *get_cwd();
#endif
