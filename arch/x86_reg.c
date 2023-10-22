#include "stdint.h"
#include "../include/x86_reg.h"
#include "../include/isr.h"

// Function to get the value of EAX register
uint32_t get_EAX() {
    uint32_t value;
    asm volatile (
        "movl %%eax, %[value]"
        : [value] "=r" (value)
        :
        : "eax"
    );
    return value;
}

// Function to get the value of EBX register
uint32_t get_EBX() {
    uint32_t value;
    asm volatile (
        "movl %%ebx, %[value]"
        : [value] "=r" (value)
        :
        : "ebx"
    );
    return value;
}

// Function to get the value of ECX register
uint32_t get_ECX() {
    uint32_t value;
    asm volatile (
        "movl %%ecx, %[value]"
        : [value] "=r" (value)
        :
        : "ecx"
    );
    return value;
}

// Function to get the value of EDX register
uint32_t get_EDX() {
    uint32_t value;
    asm volatile (
        "movl %%edx, %[value]"
        : [value] "=r" (value)
        :
        : "edx"
    );
    return value;
}

// Function to get the value of ESI register
uint32_t get_ESI() {
    uint32_t value;
    asm volatile (
        "movl %%esi, %[value]"
        : [value] "=r" (value)
        :
        : "esi"
    );
    return value;
}
uint32_t get_ESP() {
    uint32_t value;
    asm volatile (
        "movl %%esp, %[value]"
        : [value] "=r" (value)
        :
        : "esi"
    );
    return value;
}
// Function to get the value of EDI register
uint32_t get_EDI() {
    uint32_t value;
    asm volatile (
        "movl %%edi, %[value]"
        : [value] "=r" (value)
        :
        : "edi"
    );
    return value;
}
void set_EAX(uint32_t value) {
    asm volatile (
        "movl %[value], %%eax"
        :
        : [value] "r" (value)
        : "eax"
    );
}

// Function to set the value of EBX register
void set_EBX(uint32_t value) {
    asm volatile (
        "movl %[value], %%ebx"
        :
        : [value] "r" (value)
        : "ebx"
    );
}

// Function to set the value of ECX register
void set_ECX(uint32_t value) {
    asm volatile (
        "movl %[value], %%ecx"
        :
        : [value] "r" (value)
        : "ecx"
    );
}

// Function to set the value of EDX register
void set_EDX(uint32_t value) {
    asm volatile (
        "movl %[value], %%edx"
        :
        : [value] "r" (value)
        : "edx"
    );
}

// Function to set the value of ESI register
void set_ESI(uint32_t value) {
    asm volatile (
        "movl %[value], %%esi"
        :
        : [value] "r" (value)
        : "esi"
    );
}

// Function to set the value of EDI register
void set_EDI(uint32_t value) {
    asm volatile (
        "movl %[value], %%edi"
        :
        : [value] "r" (value)
        : "edi"
    );
}
void set_regs(REGISTERS* regs) {
    // Inline assembly to load the first half of variables into registers
    asm volatile(
        "movl %0, %%eax\n\t"
        "movl %1, %%ebx\n\t"
        "movl %2, %%ecx\n\t"
        "movl %3, %%edx\n\t"
        :
        : "m" (regs->eax), "m" (regs->ebx), "m" (regs->ecx), "m" (regs->edx)
        
    );

    // Inline assembly to load the second half of variables into registers
    asm volatile(
        "movl %0, %%esi\n\t"
        "movl %1, %%edi\n\t"
        "movl %2, %%esp\n\t"
        "movl %3, %%ebp\n\t"
        :
        : "m" (regs->esi), "m" (regs->edi), "m" (regs->esp), "m" (regs->ebp)
        : "esi", "edi"
    );
}