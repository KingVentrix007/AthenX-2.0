// reboot.h

#ifndef __QEMU_H
#define __QEMU_H

/**
 * Function Name: reboot_qemu
 * Description: Reboots QEMU using I/O ports.
 *
 * Parameters:
 *   port (unsigned short) - The I/O port to write to for reboot.
 *
 * Return:
 *   None
 */
void reboot_qemu(unsigned short port);

#endif // REBOOT_H
