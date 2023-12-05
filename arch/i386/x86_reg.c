#include "stdint.h"
#include "../include/x86_reg.h"
#include "../include/isr.h"
//TODO Re-do the comments for doxygen
// Function to get the value of EAX register
/**
 * @brief Get the value of the EAX register.
 *
 * @return The value of the EAX register.
 *
 * @details This function uses inline assembly to retrieve the value of the EAX register.
 * It performs the following steps:
 * 1. Declare a variable `value` of type uint32_t to store the value of the EAX register.
 * 2. Use the `asm volatile` statement to write inline assembly code.
 * 3. The assembly code moves the value of the EAX register into the variable `value`.
 * 4. The output constraint `[value] "=r" (value)` specifies that the output operand is the variable `value`, and it is assigned a register.
 * 5. The input constraint `"eax"` specifies that the EAX register is used as an input operand.
 * 6. Return the value of the EAX register stored in the variable `value`.
 */
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

/**
 * @brief Get the value of the EBX register.
 *
 * @return The value of the EBX register.
 *
 * @details This function uses inline assembly to retrieve the value of the EBX register.
 * It performs the following steps:
 * 1. Declare a variable `value` of type uint32_t to store the value of the EBX register.
 * 2. Use the `asm volatile` statement to write inline assembly code.
 * 3. The assembly code moves the value of the EBX register into the variable `value`.
 * 4. The output constraint `[value] "=r" (value)` specifies that the output operand is the variable `value`, and it is assigned a register.
 * 5. The input constraint `"ebx"` specifies that the EBX register is used as an input operand.
 * 6. Return the value of the EBX register stored in the variable `value`.
 */
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
/**
 * @brief Get the value of the ECX register.
 *
 * @return The value of the ECX register.
 *
 * @details This function uses inline assembly to retrieve the value of the ECX register.
 * It performs the following steps:
 * 1. Declare a variable `value` of type uint32_t to store the value of the ECX register.
 * 2. Use the `asm volatile` statement to write inline assembly code.
 * 3. The assembly code moves the value of the ECX register into the variable `value`.
 * 4. The output constraint `[value] "=r" (value)` specifies that the output operand is the variable `value`, and it is assigned a register.
 * 5. The input constraint `"ecx"` specifies that the ECX register is used as an input operand.
 * 6. Return the value of the ECX register stored in the variable `value`.
 */
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

/**
 * @brief Get the value of the EDX register.
 *
 * @return The value of the EDX register.
 *
 * @details This function uses inline assembly to retrieve the value of the EDX register.
 * It performs the following steps:
 * 1. Declare a variable `value` of type uint32_t to store the value of the EDX register.
 * 2. Use the `asm volatile` statement to write inline assembly code.
 * 3. The assembly code moves the value of the EDX register into the variable `value`.
 * 4. The output constraint `[value] "=r" (value)` specifies that the output operand is the variable `value`, and it is assigned a register.
 * 5. The input constraint `"edx"` specifies that the EDX register is used as an input operand.
 * 6. Return the value of the EDX register stored in the variable `value`.
 */
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

/**
 * @brief Get the value of the ESI register.
 *
 * @return The value of the ESI register.
 *
 * @details This function uses inline assembly to retrieve the value of the ESI register.
 * It performs the following steps:
 * 1. Declare a variable `value` of type uint32_t to store the value of the ESI register.
 * 2. Use the `asm volatile` statement to write inline assembly code.
 * 3. The assembly code moves the value of the ESI register into the variable `value`.
 * 4. The output constraint `[value] "=r" (value)` specifies that the output operand is the variable `value`, and it is assigned a register.
 * 5. The input constraint `"esi"` specifies that the ESI register is used as an input operand.
 * 6. Return the value of the ESI register stored in the variable `value`.
 */
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
/**
 * @brief Get the value of the ESP register.
 *
 * @return The value of the ESP register.
 *
 * @details This function uses inline assembly to retrieve the value of the ESP register.
 * It performs the following steps:
 * 1. Declare a variable `value` of type uint32_t to store the value of the ESP register.
 * 2. Use the `asm volatile` statement to write inline assembly code.
 * 3. The assembly code moves the value of the ESP register into the variable `value`.
 * 4. The output constraint `[value] "=r" (value)` specifies that the output operand is the variable `value`, and it is assigned a register.
 * 5. The input constraint `"esp"` specifies that the ESP register is used as an input operand.
 * 6. Return the value of the ESP register stored in the variable `value`.
 */
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
/**
 * @brief Get the value of the EDI register.
 *
 * @return The value of the EDI register.
 *
 * @details This function uses inline assembly to retrieve the value of the EDI register.
 * It performs the following steps:
 * 1. Declare a variable `value` of type uint32_t to store the value of the EDI register.
 * 2. Use the `asm volatile` statement to write inline assembly code.
 * 3. The assembly code moves the value of the EDI register into the variable `value`.
 * 4. The output constraint `[value] "=r" (value)` specifies that the output operand is the variable `value`, and it is assigned a register.
 * 5. The input constraint `"edi"` specifies that the EDI register is used as an input operand.
 * 6. Return the value of the EDI register stored in the variable `value`.
 */
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
/**
 * @brief set the value of the EAX register.
 *
 *
 * @details This function uses inline assembly to set the value of the EAX register.
 * It performs the following steps:
 * 1. Use the `asm volatile` statement to write inline assembly code.
 * 2. The assembly code moves the value of the `"value"` parameter into the EAX register.
 */

void set_EAX(uint32_t value) {
    asm volatile (
        "movl %[value], %%eax"
        :
        : [value] "r" (value)
        : "eax"
    );
}

/**
 * @brief set the value of the EBX register.
 *
 *
 * @details This function uses inline assembly to set the value of the EBX register.
 * It performs the following steps:
 * 1. Use the `asm volatile` statement to write inline assembly code.
 * 2. The assembly code moves the value of the `"value"` parameter into the EAX register.
 */
void set_EBX(uint32_t value) {
    asm volatile (
        "movl %[value], %%ebx"
        :
        : [value] "r" (value)
        : "ebx"
    );
}

/**
 * @brief Get the value of the ECX register.
 *
 * @return The value of the ECX register.
 *
 * @details This function uses inline assembly to retrieve the value of the ECX register.
 * It performs the following steps:
 * 1. Declare a variable `value` of type uint32_t to store the value of the ECX register.
 * 2. Use the `asm volatile` statement to write inline assembly code.
 * 3. The assembly code moves the value of the ECX register into the variable `value`.
 * 4. The output constraint `[value] "=r" (value)` specifies that the output operand is the variable `value`, and it is assigned a register.
 * 5. The input constraint `"ecx"` specifies that the ECX register is used as an input operand.
 * 6. Return the value of the ECX register stored in the variable `value`.
 */
void set_ECX(uint32_t value) {
    asm volatile (
        "movl %[value], %%ecx"
        :
        : [value] "r" (value)
        : "ecx"
    );
}

/**
 * @brief Get the value of the EDX register.
 *
 * @return The value of the EDX register.
 *
 * @details This function uses inline assembly to retrieve the value of the EDX register.
 * It performs the following steps:
 * 1. Declare a variable `value` of type uint32_t to store the value of the EDX register.
 * 2. Use the `asm volatile` statement to write inline assembly code.
 * 3. The assembly code moves the value of the EDX register into the variable `value`.
 * 4. The output constraint `[value] "=r" (value)` specifies that the output operand is the variable `value`, and it is assigned a register.
 * 5. The input constraint `"edx"` specifies that the EDX register is used as an input operand.
 * 6. Return the value of the EDX register stored in the variable `value`.
 */
void set_EDX(uint32_t value) {
    asm volatile (
        "movl %[value], %%edx"
        :
        : [value] "r" (value)
        : "edx"
    );
}

/**
 * @brief Get the value of the ESI register.
 *
 * @return The value of the ESI register.
 *
 * @details This function uses inline assembly to retrieve the value of the ESI register.
 * It performs the following steps:
 * 1. Declare a variable `value` of type uint32_t to store the value of the ESI register.
 * 2. Use the `asm volatile` statement to write inline assembly code.
 * 3. The assembly code moves the value of the ESI register into the variable `value`.
 * 4. The output constraint `[value] "=r" (value)` specifies that the output operand is the variable `value`, and it is assigned a register.
 * 5. The input constraint `"esi"` specifies that the ESI register is used as an input operand.
 * 6. Return the value of the ESI register stored in the variable `value`.
 */
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