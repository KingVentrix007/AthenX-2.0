/**
 * File: gdt.c
 * Author: KingVentrix007
 * Description: Global Descriptor Table(GDT) setup
 * Date: 2023-10-03
 * License: GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.en.html)
 *   - If you choose GPLv3, please ensure your project complies with the terms of the license.
 */


/**
 * Global Descriptor Table(GDT) setup
 */
#include "../include/gdt.h"
#include "../include/debug.h"
GDT g_gdt[NO_GDT_DESCRIPTORS];
GDT_PTR g_gdt_ptr;

/**
 * fill entries of GDT 
 */
void gdt_set_entry(int index, uint32 base, uint32 limit, uint8 access, uint8 gran) {
    //FUNC_ADDR_NAME(&gdt_set_entry);
    GDT *this = &g_gdt[index];

    this->segment_limit = limit & 0xFFFF;
    this->base_low = base & 0xFFFF;
    this->base_middle = (base >> 16) & 0xFF;
    this->access = access;

    this->granularity = (limit >> 16) & 0x0F;
    this->granularity = this->granularity | (gran & 0xF0);

    this->base_high = (base >> 24 & 0xFF);
}

// initialize GDT
void gdt_init() {
    //FUNC_ADDR_NAME(&gdt_init);
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
