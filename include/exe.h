#ifndef __EXE__H
#define __EXE__H
#include <stdint.h>
#include "elf.h"
#include <stddef.h>
#define EXECUTABLE_BASE_ADDRESS 0x90000
typedef uintptr_t addr;
typedef enum executable_type
{
  ELF32_EXECUTABLE,
  FLAT_EXECUTABLE
} executable_type;

typedef struct executable
{
  addr entry_point;
  addr program_break;
  executable_type type;
} executable;

void load_elf_file(const char* filename);

#endif