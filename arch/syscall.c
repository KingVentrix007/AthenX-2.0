#include "types.h"
#include "../libc/string.h"
#include "../include/syscall.h"
#include <stdarg.h>
//filename syscall.c
volatile uint32_t syscall_ret[1000] = {0};

extern int system_call(uint32_t syscall_number, uint32_t arg1, uint32_t *results);






int my_system_call_user(int syscall_number, int arg1) {
    int result;

    asm volatile (
        "movl %[syscall_number], %%eax   \n"  // Set system call number in EAX
        "movl %[arg1], %%ebx              \n"  // Set argument 1 in EBX
        "int $42                          \n"  // Trigger the system call using int 42

        // Preserve the original value of EAX
        "pushl %%eax \n"
        "movl %%eax, %[result] \n"  // Store the result in 'result'
        "popl %%eax \n"
        : [result] "=r" (result)
        : [syscall_number] "r" (syscall_number), [arg1] "r" (arg1)
        : "ebx", "memory"
    );

    return result;
}



int system_call_2(uint32_t syscall_number, uint32_t* args, uint32_t* results, int num_args) {
    int result;
    
    asm volatile (
        "movl %[syscall_number], %%eax   \n"  // Set system call number in eax
        :
        : [syscall_number] "r" (syscall_number)
        : "eax"
    );

    // Set arguments in ebx, ecx, edx, etc.
    for (int i = 0; i < num_args; i++) {
        asm volatile (
            "movl %[arg], %%ebx"
            :
            : [arg] "r" (args[i])
            : "ebx"
        );
    }

    asm volatile (
        "int $42                         \n"  // Trigger the system call
        :
        :
        : "eax"
    );

    // Retrieve results from eax, ebx, ecx, etc.
    for (int i = 0; i < num_args; i++) {
        asm volatile (
            "movl %%ebx, %[result]"
            : [result] "=r" (results[i])
            :
            : "ebx"
        );
    }

    return 0; // Return 0 for success, handle error cases as needed.
}


uint32_t reverse_uint32(uint32_t value) {
    return ((value >> 24) & 0xFF) | ((value >> 8) & 0xFF00) | ((value << 8) & 0xFF0000) | ((value << 24) & 0xFF000000);
}

int set_ret(uint32_t ret)
{
    //memset(&syscall_ret,0,sizeof(syscall_ret));
    //syscall_ret = 6;
    *syscall_ret = ret;
}

void reverse_uint32_value(uint32_t* ptr) {
    if (ptr != NULL) {
        *ptr = ((*ptr >> 24) & 0xFF) | ((*ptr >> 8) & 0xFF00) | ((*ptr << 8) & 0xFF0000) | ((*ptr << 24) & 0xFF000000);
    }
}
uint32_t get_ret()
{
    //reverse_uint32_value(syscall_ret);
    return *syscall_ret;
}

// Declare the external syscall function
