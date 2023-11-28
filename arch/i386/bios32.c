/** 
 * @file bios32.c 
 * @brief Bios calls in 32 bit mode 
 * @author KingVentrix007 
 * @date 2023-10-03 
 * @copyright GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.en.html) 
 * 
 * @details This file contains the implementation of bios calls in 32-bit mode. 
 * The bios32.c module provides functions and utilities for interacting with the BIOS in a 32-bit protected mode environment. 
 * It is authored by KingVentrix007 and contributions from other contributors. 
 * The file is licensed under the GNU General Public License version 3 (GPLv3). 
 * Please ensure that your project complies with the terms of the license if you choose to use this code. 
 */

#include "../include/bios32.h"
#include "../include/console.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/string.h"

/**
 * @brief Global variables for the real mode GDT and IDT.
 *
 * These global variables, g_real_mode_gdt and g_real_mode_idt, store the pointers to the real mode Global Descriptor Table (GDT) and Interrupt Descriptor Table (IDT) respectively.
 * The GDT and IDT are data structures used by the processor to manage memory segments and interrupt handling in real mode.
 * These variables are used to access and manipulate the real mode GDT and IDT in the code.
 *
 * @note The exact algorithms and logic used to initialize and manipulate the GDT and IDT are not provided in this code snippet.
 * It is recommended to refer to the relevant documentation or code implementation for detailed information on the algorithms and logic used.
 */
IDT_PTR g_real_mode_gdt;
IDT_PTR g_real_mode_idt;

/**
 * @brief External global variable for the GDT (Global Descriptor Table).
 *
 * The external global variable g_gdt is an array of GDT structures representing the Global Descriptor Table.
 * The GDT is a data structure used by the processor to manage memory segments and their attributes.
 * This variable is declared as external, indicating that it is defined in another source file, and is used to access and manipulate the GDT in the code.
 *
 * @note The exact algorithms and logic used to initialize and manipulate the GDT are not provided in this code snippet.
 * It is recommended to refer to the relevant documentation or code implementation for detailed information on the algorithms and logic used.
 */
extern GDT g_gdt[NO_GDT_DESCRIPTORS];

/**
 * @brief Pointer to the BIOS32 function.
 *
 * The variable exec_bios32_function is a function pointer that points to the BIOS32 function located at memory address 0x7c00.
 * The BIOS32 function is a 32-bit interface provided by the BIOS for accessing system services.
 * This pointer can be used to call the BIOS32 function and invoke the corresponding functionality.
 *
 * @note The exact logic and algorithms used within the BIOS32 function are not provided in this code snippet.
 * It is recommended to refer to the relevant documentation or code implementation for detailed information on the logic and algorithms used.
 */
void (*exec_bios32_function)() = (void *)0x7c00;

/**
 * @brief Initialize the BIOS32 routine.
 *
 * This function initializes the BIOS32 routine by setting the entries 6 and 7 in the Global Descriptor Table (GDT).
 * These entries will be copied when the bios32_service() function is called.
 *
 * @details The function performs the following steps:
 * 1. Sets the 16-bit code segment entry (entry 6) in the GDT using the gdt_set_entry() function.
 *    The parameters for the entry include the base address, limit, access byte, and granularity.
 * 2. Sets the 16-bit data segment entry (entry 7) in the GDT using the gdt_set_entry() function.
 *    The parameters for the entry include the base address, limit, access byte, and granularity.
 * 3. Initializes the real mode Global Descriptor Table (g_real_mode_gdt) by setting the base address and limit.
 * 4. Initializes the real mode Interrupt Descriptor Table (g_real_mode_idt) by setting the base address and limit.
 *
 * @note The gdt_set_entry() function and the exact details of the GDT, real mode GDT pointer, and real mode IDT pointer are not provided in this code snippet.
 * It is recommended to refer to the relevant documentation or code implementation for detailed information on the logic and algorithms used.
 */
void bios32_init() {
    // 16-bit code segment
    gdt_set_entry(6, 0, 0xffffffff, 0x9A, 0x0f);
    // 16-bit data segment
    gdt_set_entry(7, 0, 0xffffffff, 0x92, 0x0f);
    // real mode gdt ptr
    g_real_mode_gdt.base_address = (uint32)g_gdt;
    g_real_mode_gdt.limit = sizeof(g_gdt) - 1;
    // real mode idt ptr
    g_real_mode_idt.base_address = 0;
    g_real_mode_idt.limit = 0x3ff;
}
/**
 * @brief BIOS32 service function.
 *
 * This function copies data to the assembly file bios32_call.asm and executes code from the 0x7c00 address.
 *
 * @param interrupt The BIOS interrupt number.
 * @param in Pointer to the input registers structure.
 * @param out Pointer to the output registers structure.
 *
 * @details The function performs the following steps:
 * 1. Defines a new_code_base variable to hold the address 0x7c00.
 * 2. Copies the GDT entries from g_gdt to bios32_gdt_entries in bios32_call.asm.
 * 3. Sets the base address of bios32_gdt_entries to 0x7c00.
 * 4. Copies the g_real_mode_gdt structure to bios32_gdt_ptr in bios32_call.asm.
 * 5. Copies the g_real_mode_idt structure to bios32_idt_ptr in bios32_call.asm.
 * 6. Copies the 16-bit registers from in to bios32_in_reg16_ptr in bios32_call.asm.
 * 7. Copies the BIOS interrupt number to bios32_int_number_ptr in bios32_call.asm.
 * 8. Copies the code from bios32_call.asm to the new_code_base address.
 * 9. Executes the code from the 0x7c00 address.
 * 10. Copies the output registers from bios32_out_reg16_ptr in bios32_call.asm to the out structure.
 * 11. Re-initializes the GDT and IDT.
 */
void bios32_service(uint8 interrupt, REGISTERS16 *in, REGISTERS16 *out) {
    void *new_code_base = (void *)0x7c00;
    // Copy our GDT entries g_gdt to bios32_gdt_entries(bios32_call.asm)
    memcpy(&bios32_gdt_entries, g_gdt, sizeof(g_gdt));
    // Set base_address of bios32_gdt_entries(bios32_call.asm) starting from 0x7c00
    g_real_mode_gdt.base_address = (uint32)REBASE_ADDRESS((&bios32_gdt_entries));
    // Copy g_real_mode_gdt to bios32_gdt_ptr(bios32_call.asm)
    memcpy(&bios32_gdt_ptr, &g_real_mode_gdt, sizeof(IDT_PTR));
    // Copy g_real_mode_idt to bios32_idt_ptr(bios32_call.asm)
    memcpy(&bios32_idt_ptr, &g_real_mode_idt, sizeof(IDT_PTR));
    // Copy all 16-bit registers to bios32_in_reg16_ptr(bios32_call.asm)
    memcpy(&bios32_in_reg16_ptr, in, sizeof(REGISTERS16));
    // Get out registers address defined in bios32_call.asm starting from 0x7c00
    void *in_reg16_address = REBASE_ADDRESS(&bios32_in_reg16_ptr);
    // Copy BIOS interrupt number to bios32_int_number_ptr(bios32_call.asm)
    memcpy(&bios32_int_number_ptr, &interrupt, sizeof(uint8));
    // Copy bios32_call.asm code to new_code_base address
    uint32 size = (uint32)BIOS32_END - (uint32)BIOS32_START;
    memcpy(new_code_base, BIOS32_START, size);
    // Execute the code from 0x7c00
    exec_bios32_function();
    // Copy output registers to out
    in_reg16_address = REBASE_ADDRESS(&bios32_out_reg16_ptr);
    memcpy(out, in_reg16_address, sizeof(REGISTERS16));
    // Re-initialize the GDT and IDT
    gdt_init();
    idt_init();
}

/**
 * @brief BIOS interrupt call function.
 *
 * This function calls the bios32_service() function to handle the BIOS interrupt.
 *
 * @param interrupt The BIOS interrupt number.
 * @param in Pointer to the input registers structure.
 * @param out Pointer to the output registers structure.
 *
 * @details The function simply calls the bios32_service() function, passing the interrupt number, input registers, and output registers as arguments.
 */
void int86(uint8 interrupt, REGISTERS16 *in, REGISTERS16 *out) {
    bios32_service(interrupt, in, out);
}