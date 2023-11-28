/**
 * @file fpu.c
 * @brief Floating Point Unit Setup
 * @author KingVentrix007
 * @author OS
 * @date 2023-10-03
 * @copyright This project is released under the GNU Public License.
 *
 * @details This file contains the implementation of the floating point unit setup.
 * The purpose of this module is to configure the floating point unit for floating point operations.
 * Please ensure that your project complies with the terms of the GNU GPLv3 license if you choose to use it.
 */



#include "../include/io_ports.h"
#include "../include/debug.h"
/**
 * @brief Set the control word of the Floating Point Unit (FPU).
 *
 * This function sets the control word of the FPU to the specified value.
 *
 * @param cw The control word value to set.
 *
 * @details The function uses inline assembly to load the control word value into the FPU control register using the "fldcw" instruction.
 * The control word determines the rounding mode, precision, and other settings of the FPU.
 * This function is architecture-dependent and may require specific compiler flags or platform support to work correctly.
 * Please refer to the documentation of your target architecture and compiler for more information.
 */
void fpu_set_control_word(const uint16 cw) {
    asm volatile("fldcw %0" ::"m"(cw));
}

/**
 * @brief Enable the Floating Point Unit (FPU).
 *
 * This function enables the FPU by setting the necessary control registers and initializing the FPU control word.
 *
 * @details The function performs the following steps:
 * 1. Calls FUNC_ADDR_NAME() macro to log the function address and name for debugging purposes.
 * 2. Retrieves the current value of the CR4 control register using inline assembly and stores it in the local variable cr4.
 * 3. Sets the 9th bit to 1 in the cr4 variable to enable the FPU.
 * 4. Updates the CR4 control register with the modified value of cr4 using inline assembly.
 * 5. Calls fpu_set_control_word() function three times to set the FPU control word to specific values:
 *    - 0x37F: FPU initialization
 *    - 0x37E: Invalid operand exceptions
 *    - 0x37A: Divide by zero exceptions
 *
 * @note This function is architecture-dependent and uses inline assembly instructions to interact with the control registers and set the FPU control word.
 * The specific values used for the FPU control word may vary depending on the desired FPU behavior and the target architecture.
 * Please refer to the documentation of your target architecture and compiler for more information.
 */
void fpu_enable() {
    FUNC_ADDR_NAME(&fpu_enable, 0, " ");

    uint32 cr4;
    asm volatile("mov %%cr4, %0" :"=r"(cr4));

    // Set 9th bit to 1 in cr4
    cr4 |= 0x200;
    asm volatile("mov %0, %%cr4" ::"r"(cr4));

    fpu_set_control_word(0x37F); // FPU initialization
    fpu_set_control_word(0x37E); // Invalid operand exceptions
    fpu_set_control_word(0x37A); // Divide by zero exceptions
}