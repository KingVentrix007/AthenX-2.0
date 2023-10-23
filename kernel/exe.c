#include "fat_filelib.h"
#include "elf.h"
#include "exe.h"

#include "exe.h"
#include "string.h"
#include "../include/maths.h"
#include "../include/kernel.h"
#include "../include/x86_reg.h"
#include "printf.h"
#include "../include/kheap.h"
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





// Define a structure to represent the user process context
typedef struct {
    uint32_t esp;
    uint32_t eip;
} UserProcessContext_exe;

typedef struct {
    uint32_t esp;
} KernelContext_exe;

struct exe_file {
    uint8_t* exe_start;
    int priority;
};





void run_exe(struct exe_file exe, uint8_t* stack) {
    // Check if the stack is 16-byte aligned
    if ((uintptr_t)stack % 16 != 0) {
        printf("Error: Stack is not 16-byte aligned.\n");
        return;
    }

    // Set up the stack pointer (ESP) to point to the provided stack
    asm volatile (
        "mov %0, %%esp"
        :
        : "r"(stack)
    );

    void (*exe_entry)(int, char*[]) = (void (*)(int, char*[]))(exe.exe_start);
    
    int myArgc = 3;
    char* myArgv[] = {"Tristan", "/root/", "arg2"};

    exe_entry(myArgc, myArgv);
    int ret = 0;
    asm volatile(
        "movl %%eax, %0"
        : "=r" (ret)
    );

    printf("\nComplete EXE execution with exit code %d\n", ret);

    asm volatile (
        "mov %0, %%esp"
        :
        : "r"(stack)
    );
}


















void load_exe_file(const char* filename, uint8_t* stack) {
    // Open the EXE file
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: Failed to open the EXE file.\n");
        return;
    }

    // Read the EXE file into memory
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t* exe_data = (uint8_t*)malloc(file_size);
    if (exe_data == NULL) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return;
    }

    fread(exe_data, sizeof(uint8_t), file_size, file);
    fclose(file);

    // Check if the stack is 16-byte aligned
    if ((uintptr_t)stack % 16 != 0) {
        printf("Error: Stack is not 16-byte aligned.\n");
        free(exe_data);
        return;
    }

    // Create an exe_file structure with the EXE data and priority (if needed)
    struct exe_file my_exe;
    my_exe.exe_start = exe_data;
    my_exe.priority = 0;  // You can set the priority as needed

    // Run the EXE file
    run_exe(my_exe, stack);

    // Free the allocated memory for the EXE data
    free(exe_data);
}