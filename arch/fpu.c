/**
 * File: fpu.c
 * Author: KingVentrix007, OS(see contributions)
 * Description: floating point unit setup
 * Date: 2023-10-03
 * License: GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.en.html)
 *   - If you choose GPLv3, please ensure your project complies with the terms of the license.
 */


#include "../include/io_ports.h"
#include "../include/debug.h"
void fpu_set_control_word(const uint16 cw) {
    asm volatile("fldcw %0" ::"m"(cw));
}

void fpu_enable() {
    FUNC_ADDR_NAME(&fpu_enable,0," ");
    uint32 cr4;
    asm volatile("mov %%cr4, %0" :"=r"(cr4));
    // set 9th bit to 1 in cr4
    cr4 |= 0x200;
    asm volatile("mov %0, %%cr4" ::"r"(cr4));
    fpu_set_control_word(0x37F); // fpu init
    fpu_set_control_word(0x37E); // invalid operand exceptions
    fpu_set_control_word(0x37A); // divide by zero
}
