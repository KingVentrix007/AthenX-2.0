/**
 * File: 8259_pic.c
 * Author: KingVentrix007,araujo88(primus-os)
 * Description: 8259 Programmable Interrupt Controller(8259 PIC) setup
 * Date: 2023-10-03
 * License: GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.en.html)
 *   - If you choose GPLv3, please ensure your project complies with the terms of the license.
 */

/**
 * 8259 Programmable Interrupt Controller(8259 PIC) setup
 * for more, see https://wiki.osdev.org/8259_PIC
 */

#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/io_ports.h"
#include "../include/8259_pic.h"
#include "../include/serial.h"
#include "../include/debug.h"

/**
 * initialize 8259 PIC with default IRQ's defined in isr.h
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
 * send end of interrupt command to PIC 8259
 */
void pic8259_eoi(uint8 irq) {
    if(irq >= 0x28)
        outportb(PIC2, PIC_EOI);
        write_serial(irq,DEFAULT_COM_DEBUG_PORT);
        
    outportb(PIC1, PIC_EOI);
}
