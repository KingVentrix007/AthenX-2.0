/**
 * @file io_ports.h
 * @brief Input/Output Port Operations
 * @author KingVentrix007
 * @date <date>
 * @license GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.en.html)
 * 
 * @details This file contains functions for reading and writing data to input/output ports.
 * The functions provide operations for reading and writing bytes, shorts, and longs to specific port numbers.
 * The code includes inline assembly to perform the input/output operations.
 * 
 * @note If you choose to use the GNU GPLv3 license, please ensure that your project complies with the terms of the license.
 */

#include "../include/io_ports.h"
#include <stdint.h>

/**
 * @brief Read a byte from the given port number.
 *
 * @param port The port number to read from.
 * @return The byte read from the port.
 */
uint8_t inportb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * @brief Write a given byte to the given port number.
 *
 * @param port The port number to write to.
 * @param val The byte to write to the port.
 */
void outportb(uint16_t port, uint8_t val) {
    asm volatile ("outb %1, %0" :: "dN"(port), "a"(val));
}

/**
 * @brief Read 2 bytes (short) from the given port number.
 *
 * @param port The port number to read from.
 * @return The 2 bytes read from the port.
 */
uint16_t inports(uint16_t port) {
    uint16_t rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

/**
 * @brief Write given 2 bytes (short) to the given port number.
 *
 * @param port The port number to write to.
 * @param data The 2 bytes to write to the port.
 */
void outports(uint16_t port, uint16_t data) {
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (data));
}

/**
 * @brief Read 4 bytes (long) from the given port number.
 *
 * @param port The port number to read from.
 * @return The 4 bytes read from the port.
 */
uint32_t inportl(uint16_t port) {
    uint32_t rv;
    asm volatile ("inl %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

/**
 * @brief Write given 4 bytes (long) to the given port number.
 *
 * @param port The port number to write to.
 * @param data The 4 bytes to write to the port.
 */
void outportl(uint16_t port, uint32_t data) {
    asm volatile ("outl %1, %0" : : "dN" (port), "a" (data));
}

/**
 * @brief Read a byte from the given port number.
 *
 * @param port The port number to read from.
 * @return The byte read from the port.
 */
uint8_t input_bytes(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0"
                         : "=a"(ret)
                         : "Nd"(port));
    return ret;
}

/**
 * @brief Write a given byte to the given port number.
 *
 * @param port The port number to write to.
 * @param val The byte to write to the port.
 */
void out_bytes(uint16_t port, uint8_t val) {
    asm volatile ("outb %0,%1" : : "a"(val),"Nd"(port):"memory");
}

/**
 * @brief Scan for keyboard input.
 *
 * @return The scanned key or 0 if no key is pressed.
 */
uint8_t scan(void) {
    unsigned char brk;
    static uint8_t key = 0;
    uint8_t read_char = input_bytes(0x60); // keyboard port
    uint8_t old_char = read_char; 
    brk = read_char & 0x80;
    read_char = read_char & 0x7f;
    if (brk) {
        return key = 0;
    } else if (input_bytes(0x60) == 0x0f) {
        return "F1";
    } else if (read_char != key) {
        return key = read_char;
    } else {
        return 0;
    }
}