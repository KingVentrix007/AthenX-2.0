/**
 * @file gdt.c
 * @brief Global Descriptor Table (GDT) setup
 * @author KingVentrix007
 * @date 2023-10-03
 * @license GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.en.html)
 * 
 * @details This file contains the implementation of the GDT setup for the Global Descriptor Table.
 * The GDT is used for segment descriptor management in x86-based systems.
 * 
 * @note If you choose to use the GNU GPLv3 license, please ensure that your project complies with the terms of the license.
 */

#include "../include/gdt.h"
#include "../include/debug.h"

GDT g_gdt[NO_GDT_DESCRIPTORS]; /**< Array to hold GDT entries */
GDT_PTR g_gdt_ptr; /**< Pointer to the GDT */

/**
 * @brief Set an entry in the GDT.
 *
 * @param index The index of the GDT entry.
 * @param base The base address of the segment.
 * @param limit The limit of the segment.
 * @param access The access byte for the segment.
 * @param gran The granularity byte for the segment.
 *
 * @details This function fills the specified GDT entry with the provided values for base, limit, access, and granularity.
 */
void gdt_set_entry(int index, uint32 base, uint32 limit, uint8 access, uint8 gran) {
    GDT *this = &g_gdt[index];
    this->segment_limit = limit & 0xFFFF;
    this->base_low = base & 0xFFFF;
    this->base_middle = (base >> 16) & 0xFF;
    this->access = access;
    this->granularity = (limit >> 16) & 0x0F;
    this->granularity = this->granularity | (gran & 0xF0);
    this->base_high = (base >> 24 & 0xFF);
}

/**
 * @brief Initialize the GDT.
 *
 * @details This function initializes the GDT by setting up the necessary entries.
 * It sets the limit and base address of the GDT, and then fills the GDT entries for the NULL segment, code segment, data segment,
 * user code segment, and user data segment.
 * Finally, it loads the GDT using the load_gdt() function.
 */
void gdt_init() {
    g_gdt_ptr.limit = sizeof(g_gdt) - 1;
    g_gdt_ptr.base_address = (uint32)g_gdt;

    // NULL segment
    gdt_set_entry(0, 0, 0, 0, 0);

    // code segment
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // data segment
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // user code segment
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // user data segment
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    load_gdt((uint32)&g_gdt_ptr);
}