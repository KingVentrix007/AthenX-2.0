/**
 * @file sys_handler.c
 * @brief System Call Handlers and Definitions
 * @author KingVentrix007
 * @date <date>
 * @license GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.en.html)
 *
 * @details This file contains the implementation of system call handlers and related functions.
 * It includes functions for handling specific system calls, tracking memory allocations, and managing file operations.
 *
 * @note If you choose to use the GNU GPLv3 license, please ensure that your project complies with the terms of the license.
 */

#include "../include/vesa.h"
#include "../include/isr.h"
#include "../include/printf.h"
#include "../include/keyboard.h"
#include "../userspace/libc/include/syscall.h"
#include "../include/fat_filelib.h"
#include "../include/kheap.h"
#include "../include/exe.h"
#include "../include/string.h"

uintptr_t memory_allocated[1000]; /**< Array to track memory allocations */
FILE files_opened[1000]; /**< Array to track opened files */

/**
 * @brief System call handler for the print system call.
 *
 * This function handles the print system call by displaying the message.
 *
 * @param reg Pointer to the register state at the time of the system call.
 */
void handle_print_system_call(REGISTERS *reg) {
    const char *message = (const char *)reg->ebx;
    printf(message);
}

/**
 * @brief System call handler for the fwrite system call.
 *
 * This function handles the fwrite system call by writing the buffer contents to the specified file.
 *
 * @param file Pointer to the file structure.
 * @param buffer Pointer to the buffer containing the data to be written.
 * @return 0 on success, 1 if there was an error opening the file, 2 if there was an error writing to the file.
 */
int handle_fwrite_system_call(FILE *file, char *buffer) {
    if (fputs(buffer, file) == EOF) {
        fclose(file);
        return 2;
    }
    fclose(file);
    return 0;
}

/**
 * @brief System call handler for the getchar system call.
 *
 * This function handles the getchar system call by waiting for user input and returning the character.
 *
 * @param reg Pointer to the register state at the time of the system call.
 * @return The character entered by the user.
 */
char *handler_get_char_system_call(REGISTERS *reg) {
    asm("sti");

    while (1) {
        char *c = get_chr();
        if (c == -1 || c == 0) {
            // No character available, continue waiting
        } else {
            return c;
        }
    }
}

/**
 * @brief System call handler for the screen control system call.
 *
 * This function handles the screen control system call by performing screen-related operations.
 *
 * @return 0 on success.
 */
int screen_ctrl() {
    // TODO: Implement screen control system call
    return 0;
}

// Global array to track memory allocations
#define MAX_ALLOCATIONS 100
void *allocated_memory[MAX_ALLOCATIONS] = {NULL};
size_t allocated_memory_sizes[MAX_ALLOCATIONS] = {0};
int num_allocations = 0; // Number of allocations currently tracked
size_t total_memory_used = 0; // Total memory used by tracked allocations
size_t total_memory_used_allocated = 0; // Total memory allocated by user program

/**
 * @brief Track memory allocation by storing the allocated pointer and size in the global arrays.
 *
 * This function tracks memory allocations by storing the allocated pointer in the global array and adding the size to the total memory used.
 *
 * @param ptr Pointer to the allocated memory.
 * @param size Size of the allocated memory.
 * @return 1 if memory is successfully tracked, 0 if the array is full.
 */
int track_memory_allocation(void *ptr, size_t size) {
    if (num_allocations < MAX_ALLOCATIONS) {
        allocated_memory[num_allocations] = ptr;
        allocated_memory_sizes[num_allocations] = size;
        num_allocations++;
        total_memory_used += size;
        total_memory_used_allocated += size;
        return 1; // Memory successfully tracked
    }
    return 0; // Array is full
}

/**
 * @brief Track memory deallocation by removing the pointer from the global array and subtracting the size from the total memory used.
 *
 * This function tracks memory deallocation by removing the pointer from the global array and subtracting the size from the total memory used.
 *
 * @param ptr Pointer to the deallocated memory.
 */
void track_memory_deallocation(void *ptr) {
    for (int i = 0; i < num_allocations; i++) {
        if (allocated_memory[i] == ptr) {
            // Subtract the deallocated memory size from the total
            total_memory_used -= allocated_memory_sizes[i];
            // Shift remaining entries to fill the gap
            for (int j = i; j < num_allocations - 1; j++) {
                allocated_memory[j] = allocated_memory[j + 1];
                allocated_memory_sizes[j] = allocated_memory_sizes[j + 1];
            }
            num_allocations--;
            return;
        }
    }
}

/**
 * @brief Deallocate any memory left in the global array that was not explicitly deallocated and update the total memory used.
 */
void deallocate_untracked_memory() {
    printf("Allocated memory size: %d\n", total_memory_used_allocated);
    printf("Total memory size not deallocated: %d\n", total_memory_used);
    for (int i = 0; i < num_allocations; i++) {
        total_memory_used -= allocated_memory_sizes[i];
        kfree(allocated_memory[i]);
        allocated_memory[i] = NULL;
        allocated_memory_sizes[i] = 0;
    }
    printf("Freed %d allocated memory units:\n", num_allocations);
    num_allocations = 0;
}

uint8_t process_stack_user[8192] __attribute__((aligned(16))); /**< User process stack */

int echo_mode = false; /**< Echo mode flag */
/**
 * @brief System call handler for handling various system calls.
 *
 * This function handles different system calls based on the syscall_number parameter and performs the corresponding operations.
 *
 * @param syscall_number The number representing the specific system call.
 * @param param1 The first parameter for the system call.
 * @param param2 The second parameter for the system call.
 * @return The result of the system call operation.
 *
 * @details The function performs the following steps:
 * 1. Initializes variables and structures used for specific system calls.
 * 2. Switches on the syscall_number to determine the specific system call to handle.
 * 3. For each system call, it performs the necessary operations and sets the result accordingly.
 * 4. The function handles system calls such as SYS_EXIT, SYS_FORK, SYS_READ, SYS_WRITE, SYS_OPEN, SYS_CLOSE, SYS_LSEEK, SYS_FSEEK, SYS_MMAP, SYS_MUNMAP, SYS_DUP, SYS_DUP2, SYS_EXECVE, SYS_WAITPID, SYS_GETPID, SYS_GETPPID, SYS_GETUID, SYS_GETGID, SYS_SETUID, SYS_SETGID, SYS_SLEEP, SYS_PRINT, SYS_GETCHAR, SYS_SCREEN_CTRL, SYS_PUTCHAR, SYS_RM, SYS_TELL, SYS_FTELL, SYS_LIST_DIR, SYS_IS_DIR, SYS_IS_PATH, SYS_CREATE, SYS_TERMINAL, SYS_GET_SCAN, SYS_CALLOC, SYS_REALLOC, SYS_LAZY_MAN, SYS_FWRITE, SYS_FREAD, SYS_FGETC, SYS_FPUTC, SYS_FPUTS, SYS_FGETS, SYS_SET_FONT_C, SYS_SET_BG_COLOR, SYS_SET_X, SYS_SET_Y, SYS_GET_X, SYS_GET_Y.
 * 5. After performing the necessary operations for the system call, it acknowledges the interrupt by sending an End of Interrupt (EOI) signal to the PIC8259.
 * 6. Returns the result of the system call operation.
 */
int system_call_handler_c(int syscall_number, int param1, int param2) {
    int result = 0;
    terminal_struct *term;
    FILE *fp;
    parameters *parmss;
    struct syscall *s;
    Entry dirs[MAX];
    Entry files[MAX];
    int dir_count = 0;
    int file_count = 0;
    char b[5] = "f";

    // Handle different system calls based on syscall_number
    switch (syscall_number) {
        case SYS_EXIT:
            printf("SYSCALL SYS_EXIT coming soon\n");
            break;
        case SYS_FORK:
            printf("SYSCALL SYS_FORK coming soon\n");
            break;
        case SYS_READ:
            parmss = (parameters *)param1;
            fp = (FILE *)param2;
            int count = 0;
            result = 0;
            char buffer[1001]; // Buffer to store the characters read, including the null-terminator
            int index = 0;
            int ch;
            while (index < 1000 && (ch = fgetc(fp)) != EOF) {
                buffer[index++] = (char)ch;
            }
            memcpy((char *)param1, buffer, sizeof(buffer));
            buffer[index] = '\0'; // Null-terminate the string
            break;
        case SYS_WRITE:
            result = handle_fwrite_system_call((FILE *)param1, (char *)param2);
            break;
        case SYS_OPEN:
            fp = fopen(param1, param2);
            result = (int)fp;
            break;
        case SYS_CLOSE:
            fp = (FILE *)param1;
            fclose(fp);
            break;
        case SYS_LSEEK:
            fp = (FILE *)param1;
            parmss = (parameters *)param2;
            fl_fseek(fp, parmss->param1, parmss->param2);
            break;
        // Handle other system calls...
        default:
            printf("Unknown syscall number\n");
            break;
    }

    pic8259_eoi(42); // Acknowledge interrupt

    return result;
}
/**
 * @brief Initialize the syscall module.
 *
 * This function initializes the syscall module by clearing the memory_allocated and files_opened arrays.
 *
 * @details The function performs the following steps:
 * 1. Uses the memset() function to set all elements of the memory_allocated array to 0.
 * 2. Uses the memset() function to set all elements of the files_opened array to 0.
 *
 * @note The memory_allocated and files_opened arrays are used to keep track of memory allocations and opened files in the system.
 */
int syscall_init() {
    memset(memory_allocated, 0, sizeof(memory_allocated));
    memset(files_opened, 0, sizeof(files_opened));
}
/**
 * @brief Destroy syscall resources.
 *
 * This function is responsible for destroying the resources used by the syscall. It clears the memory_allocated and files_opened arrays by setting all elements to 0.
 *
 * @return void
 *
 * @details The function uses the memset() function to set all elements of the memory_allocated and files_opened arrays to 0. This ensures that all resources are released and ready for reuse.
 */
void syscall_destroy()
{
    memset(memory_allocated, 0, sizeof(memory_allocated));
    memset(files_opened, 0, sizeof(files_opened));
}