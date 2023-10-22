#include "elf.h"
#include "../include/fat_filelib.h"
#include "../include/kheap.h"
#include "string.h"
#include "../include/maths.h"
#include "../include/exe.h"
#include "../include/i386.h"
#include "../include/kernel.h"
#include "../include/debug.h"
#include "../include/isr.h"
#include "../include/x86_reg.h"
#include "printf.h"
addr original_esp;
addr original_ebp;

uint8_t process_stack[8192] __attribute__((aligned(16)));
// Define a structure to represent the user process context
typedef struct {
    uint32_t esp;
    uint32_t eip;
} UserProcessContext;
typedef struct {
    uint32_t esp;
} KernelContext;
// Define a structure to store register values
typedef struct {
    uint32_t ebp;
    uint32_t esp;
} RegisterState;



struct elf_exe {
    uint8_t* elf_start; // Pointer to the start address of the ELF file
    int priority;       // Priority number (not used in this code)
    // Add any other necessary members here
};








/**
 * Function Name: run_elf
 * Description: Load and run an ELF executable.
 *
 * Parameters:
 *   elf - A struct containing the ELF file's start address and other data.
 *
 * Return:
 *   [Description of the return value, if applicable]
 */
void run_elf(struct elf_exe elf) {
    // Define a stack aligned to 16 bytes
    uint8_t stack[4096] __attribute__((aligned(16)));
    memset(stack, 0, sizeof(stack));
    // Set up the stack pointer (ESP) to point to the top of the stack
    asm volatile (
        "mov %0, %%esp" // Set the stack pointer
        :
        : "r"(&stack[4096])
    );

    // Load and run the ELF executable
    void (*elf_entry)(void) = (void (*)(void))(elf.elf_start); // Assuming ELF entry point is at offset 24
    elf_entry();
    int ret = 0;
    asm volatile(
        "movl %%eax, %0" // Pop the top value from the stack and store it in ret
        : "=r" (ret) // Output operand
    );

    printf("\nComplete ELF execution with exit code %d\n",ret);
    // Restore the stack pointer (ESP)
    asm volatile (
        "mov %0, %%esp"
        :
        : "r"(&stack[4096])
    );
}















bool check_overlap(uintptr_t kernel_start, uintptr_t kernel_end, Elf32_Ehdr* elf_header) {
    uintptr_t elf_start = (uintptr_t)elf_header;
    uintptr_t elf_end = elf_start + elf_header->e_ehsize; // Adjust this as needed based on ELF structure.

    // Check for overlap by comparing the addresses.
    if (elf_end < kernel_start || elf_start > kernel_end) {
        // No overlap, return false.
        return false;
    } else {
        // Overlap detected, return true.
        return true;
    }
}

void save_register_state(RegisterState* state) {
    __asm__ volatile (
        "movl %%ebp, %0\n\t"  // Move the value of ebp into state->ebp
        "movl %%esp, %1\n\t"  // Move the value of esp into state->esp
        : "=r" (state->ebp), "=r" (state->esp)  // Output operands
        :
        : "memory"
    );
}
void restore_register_state(RegisterState* state) {
    __asm__ volatile (
        "movl %0, %%ebp\n\t"  // Move the value in state->ebp into ebp
        "movl %1, %%esp\n\t"  // Move the value in state->esp into esp
        :
        : "r" (state->ebp), "r" (state->esp)  // Input operands
        : "memory"
    );
}
// Load ELF executable into memory.
// Define memory layout constants
#define KERNEL_START_ADDR   0x1000  // Adjust as needed
#define KERNEL_END_ADDR     0x5000  // Adjust as needed

// Function to check if a segment overlaps with kernel memory
// Define memory layout constants for the ELF file
#define ELF_LOAD_OFFSET   0x9000  // Adjust this offset as needed

// Function to check if a segment overlaps with kernel memory
int is_kernel_memory_overlap(Elf32_Phdr* segment) {
    uint32_t segment_start = segment->p_vaddr;
    uint32_t segment_end = segment_start + segment->p_memsz;

    // Calculate the load address based on ELF_LOAD_OFFSET
    uint8_t* load_address = (uint8_t*)((uint32_t)&__kernel_section_end + ELF_LOAD_OFFSET);
    //printf("load->%p\n",load_address);
    // Check for overlap with kernel memory region
    uint8_t* kernel_start = &__kernel_section_start;
    uint8_t* kernel_end = &__kernel_section_end;

    if (segment_start < (uint32_t)kernel_end && segment_end > (uint32_t)kernel_start) {
        // Handle overlap error (e.g., reject the load)
        printf("Error: ELF segment overlaps with kernel memory\n");
        printf("Segment Start: %p\n", (void*)segment_start);
        printf("Segment End: %p\n", (void*)segment_end);
        printf("Kernel Start: %p\n", (void*)kernel_start);
        printf("Kernel End: %p\n", (void*)kernel_end);
        return 1;  // Overlap detected
    }

    return 0;  // No overlap
}
void switch_to_user_mode(UserProcessContext *user_context, KernelContext *kernel_context) {
    FUNC_ADDR_NAME(&switch_to_user_mode,2,"uu");
    // Save the kernel context
    asm volatile (
        "movl %%esp, %0"
        : "=r" (kernel_context->esp)
    );
     printf("here");
    // Load the user process context
    asm volatile (
        "mov %0, %%esp \n"  // Load ESP
       
        "call *%1"          // Call the entry point
        :
        : "r" (user_context->esp), "r" (user_context->eip)
    );
    printf("\nhere2");
    // Restore the kernel context
    asm volatile (
        "mov %0, %%esp"
        :
        : "r" (kernel_context->esp)
    );
}
// Function to load and execute an ELF executable
void load_elf_executable(uint8_t* elf_data) {
    FUNC_ADDR_NAME(&load_elf_executable,1,"u");
    // Verify ELF magic number.
    Elf32_Ehdr* elf_header = (Elf32_Ehdr*)elf_data;
    
    if (elf_header->e_ident[EI_MAG0] != ELFMAG0 ||
        elf_header->e_ident[EI_MAG1] != ELFMAG1 ||
        elf_header->e_ident[EI_MAG2] != ELFMAG2 ||
        elf_header->e_ident[EI_MAG3] != ELFMAG3) {
        // Invalid ELF format.
        // Handle error.
        return;
    }
    
    // Read program headers.
    Elf32_Phdr* program_headers = (Elf32_Phdr*)(elf_data + elf_header->e_phoff);
    // uint8_t* user_stack = &process_stack[8192]; // Set ESP to the top of the stack
    // uint32_t marker_value = 0xDEADBEEF;
    // // Calculate the initial stack pointer
    // uint32_t* esp = (uint32_t*)(user_stack + 8192);
    
    // Push dummy values for argc, argv, and envp
    // *(--esp) = 0;  // envp (NULL-terminated)
    // *(--esp) = 0;  // argv (NULL-terminated)
    // *(--esp) = 0;  // argc
    bool over = check_overlap(__kernel_section_start,__kernel_section_end,elf_header);
    if(over)
    {
        printf("Overlap detected\n");
        return -1;
    }
    else
    {
        // printf("Continuing execution\n");
    }
    // Iterate through program headers and load loadable segments.
    for (int i = 0; i < elf_header->e_phnum; i++) {
        if (program_headers[i].p_type == PT_LOAD) {
            // Calculate the load address by adding p_vaddr to __kernel_section_end
            uint8_t* load_address = (uint8_t*)(uint32_t)(program_headers[i].p_vaddr);

            // Allocate memory and copy segment data.
            uint8_t* file_data = elf_data + program_headers[i].p_offset;
            uint32_t file_size = program_headers[i].p_filesz;

            // Zero-fill any padding.
            for (uint32_t j = program_headers[i].p_filesz; j < program_headers[i].p_memsz; j++) {
                load_address[j] = 0;
            }

            // Copy data.
            memcpy(load_address, file_data, file_size);
        }
    }
     struct elf_exe my_elf;
    my_elf.elf_start = (uint32_t*)(elf_header->e_entry);
    run_elf(my_elf);
    //(uint32_t)&__kernel_section_end + 
    //(uint32_t)&__kernel_section_end+
    // Read entry point and jump to it.
    // void (*entry_point)() = (void (*)())(elf_header->e_entry);
    // uint8_t* user_stack = &process_stack[8192]; // Set ESP to the top of the stack
    // sizeof(user_stack);
//     uintptr_t original_esp;
//     uintptr_t original_ebp;
//     unsigned int flags;
//     // asm volatile("pushf; pop %0" : "=g"(flags));
//     REGISTERS *regs;
//     regs = get_regs();
    
//     print_registers(regs,"");
//     //  asm volatile("pusha;");

//   // Save 'esp' and 'ebp' so stack can be restored after process exits
//   asm volatile(
//       "movl %%esp, %0;"
//       "movl %%ebp, %1;"
//       : "=m"(original_esp),
//         "=m"(original_ebp));

//   // Set up a new stack for the process (userspace stack)
//   asm volatile("movl %0, %%esp" ::"r"(process_stack + sizeof(process_stack)));
// //   asm volatile("movl %%esp, %%ebp;");
//     // Read entry point and jump to it.
//     // void (*entry_point)() = (void (*)())(elf_header->e_entry);
//     // entry_point();
//     asm volatile("movl %0, %%eax;"    // Save entry_point before switching stack frames
//                "movl %%esp, %%ebp;" // Set up the 'ebp' for the process' stack
//                "jmp *%%eax;"        // Jump to the binary to start execution
//                :
//                : "m"(elf_header->e_entry));
//     asm volatile(".global process_kernel_return;"
//                "process_kernel_return:");
//     // set_regs(regs);
//     printf("exit succefull 1/5\n");
//     set_regs(regs);
//     REGISTERS *end_r = get_regs();
//     print_registers(end_r,"");
//     //! CAUSE OF HANG(1)
//     // asm volatile(
//     //   "movl %0, %%esp;"
//     //   "movl %1, %%ebp;"
//     //   :
//     //   : "m"(original_esp),
//     //     "m"(original_ebp));
//     // //  ! CAUSE OF HANG(1) END
//     printf("exit succefull 2/5\n");
    
//     // asm volatile("push %0; popf" :: "g"(flags));
//     // asm volatile("popa");
//     // printf("Exited\n");
// //     uint32_t stack_marker;
// //    asm volatile (
// //         "movl %%esp, %0"    // Move ESP into esp_value
// //         : "=r" ()  // Output: esp_value
// //     );

// //     if (stack_marker != marker_value) {
// //         printf("Error: Stack corrupted or not returning to terminal code.\n");
// //     }

//     printf("Finished exectuin of elf file\n");
//     REGISTERS *end_regs;
//     end_regs = get_regs();
//     print_registers(end_regs,"");
    
    

    // Restore the saved register values
    // asm volatile(
    //     "popl %0\n" // Pop EFLAGS into EAX
    //     "popfl"
    //     : "=m"(eflags)
    // );
    // asm volatile(
    //     "popl %0\n"
    //     "popl %1\n"
    //     "popl %2\n"
    //     "popl %3\n"
    //     "popl %4\n"
    //     "popl %5\n"
    //     "popl %6\n"
    //     "popl %7\n"
    //     : "=m"(eax), "=m"(ebx), "=m"(ecx), "=m"(edx), "=m"(ebp), "=m"(esi), "=m"(edi), "=m"(eflags)
    // );
    // printf("exiting the loader script\n");
    // restore_register_state(state);
    // printf("exited");
    // exit_elf(kernel_context);

    // Clear loaded memory and resources
    // memset(elf_data, 0, sizeof(elf_data));
    // printf("returning\n");
    return 0;
    
    // Reset loaded kernel memory if needed
}  // Reset loaded kernel memory if needed
void exit_elf(KernelContext* location)
{
     asm volatile (
        "mov %0, %%esp"
        :
        : "r" (location->esp)
    );
    terminal_main();
}
void load_elf_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        // Handle file opening error.
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t* elf_data = (uint8_t*)malloc(file_size);
    if (elf_data == NULL) {
        printf("Malloc error\n");
        // Handle memory allocation error.
        fclose(file);
        return;
    }
    //printf("%d\n",__LINE__);
    fread(elf_data, sizeof(uint8_t), file_size, file);
    fclose(file);
    //printf("%d\n",__LINE__);
    load_elf_executable(elf_data);
    // printf("Exited to prime\n");
    //printf("HERE");
    // Now, you can parse the ELF data and load it into memory as described in the previous responses.

    // Don't forget to free the allocated memory when you're done.
    kfree(elf_data);
}


// Function to set up the stack for the ELF program
// void setup_stack(void* stack, void* entry_point) {
//     // Initialize the stack pointer (ESP or RSP)
//     // Set it to the top of the allocated stack memory
//     asm volatile ("mov %0, %%esp" :: "r"(stack));  // For x86
//     // asm volatile ("mov %0, %%rsp" :: "r"(stack)); // For x86_64

//     // Push the entry point address onto the stack
//     asm volatile ("push %0" :: "r"(entry_point));
// }

// // Function to load and execute an ELF program
// void load_elf_file(const char* elf_file) {
//     // Open and read the ELF file
//     FILE* file = fopen(elf_file, "rb");
//     if (!file) {
//         perror("Failed to open file");
//         return 1;
//     }

//     // Read and verify the ELF header
//     Elf32_Ehdr elf_header;
//     if (fread(&elf_header, sizeof(Elf32_Ehdr), 1, file) != 1) {
//         perror("Failed to read ELF header");
//         fclose(file);
//         return 1;
//     }

//     // Check if it's an ELF file (verify the magic number)
//     if (memcmp(elf_header.e_ident, ELFMAG, SELFMAG) != 0) {
//         printf("Not an ELF file\n");
//         fclose(file);
//         return 1;
//     }

//     // Specify the entry point for the ELF program
//     void* entry_point = (void*)elf_header.e_entry;

//     // Allocate memory for the stack
//     void* stack = malloc(STACK_SIZE);
//     if (!stack) {
//         perror("Failed to allocate stack memory");
//         fclose(file);
//         return 1;
//     }

//     // Set up the stack for the ELF program
//     setup_stack(stack + STACK_SIZE, entry_point);

//     // Close the ELF file
//     fclose(file);

//     // Execute the ELF program (load it and start execution)

//     // Free the allocated stack memory when it's no longer needed
//     free(stack);

//     return 0;
// }