#include "qemu.h"

void reboot_qemu(unsigned short port) {
    // Using inline assembly to perform the outb instruction
    asm volatile (
        "outb %0, %1"
        :
        : "a" (0x64), "Nd" (port)
    );

    // Insert a delay to ensure QEMU has processed the command
    for (volatile int i = 0; i < 500000; ++i) {
        asm volatile ("nop");
    }

    // Send the reset command
    asm volatile (
        "outb %0, %1"
        :
        : "a" (0xFE), "Nd" (0x64)
    );

    // Infinite loop to ensure the reset occurs
    for (;;) {
        asm volatile ("nop");
    }
}