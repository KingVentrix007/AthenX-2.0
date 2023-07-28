#ifndef FS_H
#define FS_H
#include "stdint.h"
#include "types.h"

int write(char filename[8], char data[512-8]);

int read(char filename[8]);
#endif
