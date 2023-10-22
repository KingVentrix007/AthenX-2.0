#include "fat_filelib.h"
#include "elf.h"
#include "exe.h"


int is_elf(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return 0;
    }

    Elf32_Ehdr elf_header;  // For 32-bit ELF files, change to Elf64_Ehdr for 64-bit files
    fread(&elf_header, 1, sizeof(elf_header), file);

    // Check if the file starts with the ELF magic number
    if (memcmp(elf_header.e_ident, ELFMAG, SELFMAG) == 0) {
        fclose(file);
        return 1;  // It's a valid ELF file
    }

    fclose(file);
    return 0;  // It's not an ELF file
}

void append_program(const char *path, ProgramEntry programs[MAX_PROGRAMS], int *program_count) {
    if (*program_count < MAX_PROGRAMS) {
        strncpy(programs[*program_count].name, path, sizeof(programs[*program_count].name) - 1);
        programs[*program_count].name[sizeof(programs[*program_count].name) - 1] = '\0'; // Ensure null-terminated
        (*program_count)++;
    }
}


void find_programs(const char *path,ProgramEntry programs[MAX_PROGRAMS], int *program_count)
{
    Entry dirs[MAX];
    Entry files[MAX];

    int dir_count = 0;
    int file_count = 0;
    fl_listdirectory(path, dirs, files, &dir_count, &file_count);
    int count = 0;
    program_count = 0;
    // int count = 0;
     for (int i = 0; i < file_count; i++) {
        char tmp[1024] = "";
        strcat(tmp,path);
        strcat(tmp,"/");
        strcat(tmp,files[i].name);
        if(is_elf(tmp) == 0)
        {
            append_program(tmp,programs,&count);
            printf("Program %d %s\n",&count,programs[count--].name);
            
        }
    }
}