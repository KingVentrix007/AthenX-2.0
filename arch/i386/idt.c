/**
 * @file idt.c
 * @brief Interrupt Descriptor Table (IDT) setup
 * @author KingVentrix007
 * @date <date>
 * @license GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.en.html)
 * 
 * @details This file contains the implementation of the Interrupt Descriptor Table (IDT) setup.
 * The IDT is responsible for handling interrupts in an x86-based system.
 * 
 * @note If you choose to use the GNU GPLv3 license, please ensure that your project complies with the terms of the license.
 */

#include "../include/idt.h"
#include "../include/isr.h"
#include "../include/8259_pic.h"
#include "../include/debug.h"
#include "../include/sys_handler.h"

IDT g_idt[NO_IDT_DESCRIPTORS]; /**< Array to hold IDT entries */
IDT_PTR g_idt_ptr; /**< Pointer to the IDT */

/**
 * @brief Set an entry in the IDT.
 *
 * @param index The index of the IDT entry.
 * @param base The base address of the interrupt handler.
 * @param seg_sel The segment selector for the interrupt handler.
 * @param flags The flags byte for the interrupt handler.
 *
 * @details This function fills the specified IDT entry with the provided values for base, seg_sel, and flags.
 */
void idt_set_entry(int index, uint32 base, uint16 seg_sel, uint8 flags) {
    IDT *this = &g_idt[index];
    this->base_low = base & 0xFFFF;
    this->segment_selector = seg_sel;
    this->zero = 0;
    this->type = flags | 0x60;
    this->base_high = (base >> 16) & 0xFFFF;
}

/**
 * @brief Initialize the IDT.
 *
 * @details This function initializes the IDT by setting up the necessary entries.
 * It sets the base address and limit of the IDT, initializes the Programmable Interrupt Controller (PIC),
 * and fills the IDT entries for the exception handlers, IRQ handlers, system call handler, and additional interrupt handlers.
 * Finally, it loads the IDT using the load_idt() function and enables interrupts using the "sti" assembly instruction.
 */
void idt_init() {
    g_idt_ptr.base_address = (uint32)g_idt;
    g_idt_ptr.limit = sizeof(g_idt) - 1;

    pic8259_init();

    // Fill IDT entries for exception handlers
    idt_set_entry(0, (uint32)exception_0, 0x08, 0x8E);
    idt_set_entry(1, (uint32)exception_1, 0x08, 0x8E);
    // ...
    // Fill IDT entries for IRQ handlers
    idt_set_entry(32, (uint32)irq_0, 0x08, 0x8E);
    idt_set_entry(33, (uint32)irq_1, 0x08, 0x8E);
    // ...
    // Fill IDT entry for system call handler
    idt_set_entry(42, (uint32)system_call_handler, 0x08, 0x8E);
    // ...
    // Fill additional IDT entries

    load_idt((uint32)&g_idt_ptr);
    asm volatile("sti");
}