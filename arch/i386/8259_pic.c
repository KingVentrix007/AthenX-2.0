/**
 * @file 8259_pic.c
 * @brief 8259 Programmable Interrupt Controller (8259 PIC) setup
 * @author KingVentrix007
 * @author araujo88
 * @date 2023-10-03
 * @license GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.en.html)
 *
 * @details This file contains the setup for the 8259 Programmable Interrupt Controller (8259 PIC).
 *          For more information, please refer to the following link: https://wiki.osdev.org/8259_PIC
 */

#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/io_ports.h"
#include "../include/8259_pic.h"
#include "../include/serial.h"
#include "../include/debug.h"

/**
 * @brief Initialize the 8259 Programmable Interrupt Controller (8259 PIC) with default IRQs defined in isr.h.
 *
 * This function initializes the 8259 PIC by sending commands and configuring various registers.
 * The default IRQs defined in isr.h are mapped to the vector offsets in the master and slave PICs.
 *
 * @details The function performs the following steps:
 * 1. Saves the mask registers of both master and slave PICs.
 * 2. Sends initialization commands (ICW1) to both master and slave PICs.
 * 3. Maps the vector offset of all default IRQs from 0x20 to 0x27 in the master PIC (ICW2).
 * 4. Maps the vector offset of all default IRQs from 0x28 to 0x2F in the slave PIC (ICW2).
 * 5. Sets ICW3 to inform the master PIC about the presence of a slave PIC at IRQ2 (0000 0100).
 * 6. Sets ICW3 to inform the slave PIC about its cascade identity (0000 0010).
 * 7. Sets ICW4 to configure the PICs in x86 mode.
 * 8. Restores the saved mask registers of both master and slave PICs.
 *
 * @note The function uses the inportb() and outportb() functions to read from and write to the PIC's data and command registers.
 * The specific values used for ICW1, ICW2, ICW3, and ICW4 are defined in the code.
 * The function also assumes that the PIC1_COMMAND, PIC1_DATA, PIC2_COMMAND, and PIC2_DATA constants are defined and represent the respective port addresses.
 */
void pic8259_init() {
    //FUNC_ADDR_NAME(&pic8259_init);
    uint8 a1, a2;

    // save mask registers
    a1 = inportb(PIC1_DATA);
    a2 = inportb(PIC2_DATA);

    // send commands to pic to initialize both master & slave
    outportb(PIC1_COMMAND, ICW1);
    outportb(PIC2_COMMAND, ICW1);

    // map vector offset of all default IRQ's from 0x20 to 0x27 in master(ICW2)
    outportb(PIC1_DATA, 0x20);

    // map vector offset of all default IRQ's from 0x28 to 0x2F in slave(ICW2)
    outportb(PIC2_DATA, 0x28);

    // ICW3: tell master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outportb(PIC1_DATA, 4);

    // ICW3: tell slave PIC its cascade identity (0000 0010)
    outportb(PIC2_DATA, 2);

    // ICW4, set x86 mode
    outportb(PIC1_DATA, ICW4_8086);
    outportb(PIC2_DATA, ICW4_8086);

    // restore the mask registers
    outportb(PIC1_DATA, a1);
    outportb(PIC2_DATA, a2);
}
/**
 * @brief Send the End of Interrupt (EOI) command to the PIC 8259.
 *
 * This function sends the End of Interrupt (EOI) command to the PIC 8259 to acknowledge the completion of an interrupt request.
 *
 * @param irq The IRQ number for which the EOI command is sent.
 *
 * @details The function performs the following steps:
 * 1. Checks if the IRQ number is greater than or equal to 0x28, which indicates that it is a slave PIC IRQ.
 * 2. If it is a slave PIC IRQ, sends the EOI command to the PIC2 and writes the IRQ number to the serial port for debugging purposes.
 * 3. Sends the EOI command to the PIC1.
 *
 * @note The function uses the outportb() function to write to the PIC's command register.
 * The specific values used for PIC2, PIC_EOI, PIC1, and DEFAULT_COM_DEBUG_PORT are assumed to be defined elsewhere in the code.
 */
void pic8259_eoi(uint8 irq) {
    if (irq >= 0x28) {
        outportb(PIC2, PIC_EOI);
        write_serial(irq, DEFAULT_COM_DEBUG_PORT);
    }
    outportb(PIC1, PIC_EOI);
}