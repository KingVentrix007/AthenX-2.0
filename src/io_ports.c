#include "io_ports.h"
#include <stdint.h>
/**
 * read a byte from given port number
 */
uint8 inportb(uint16 port) {
    uint8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * write a given byte to given port number
 */
void outportb(uint16 port, uint8 val) {
    asm volatile("outb %1, %0" :: "dN"(port), "a"(val));
}

/**
 * read 2 bytes(short) from given port number
 */
uint16 inports(uint16 port) {
    uint16 rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

/**
 * write given 2 bytes(short) to given port number
 */
void outports(uint16 port, uint16 data) {
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (data));
}

/**
 * read 4 bytes(long) from given port number
 */
uint32 inportl(uint16 port) {
    uint32 rv;
    asm volatile ("inl %%dx, %%eax" : "=a" (rv) : "dN" (port));
    return rv;
}

/**
 * write given 4 bytes(long) to given port number
 */
void outportl(uint16 port, uint32 data) {
    asm volatile ("outl %%eax, %%dx" : : "dN" (port), "a" (data));
}
uint8_t input_bytes(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__("inb %1, %0"
                         : "= a"(ret)
                         : "Nd"(port));
    return ret;
}

void out_bytes(uint16_t port, uint8_t val)
{
    asm volatile ("outb %0,%1" : : "a"(val),"Nd"(port):"memory");
}
uint8_t scan(void)
{
    unsigned char brk;
    static uint8_t key = 0;
    uint8_t read_char = input_bytes(0x60); // keyboard port
    uint8_t old_char = read_char; 
    brk = read_char & 0x80;
    read_char = read_char & 0x7f;
    if (brk)
    {
        return key = 0;
    }
    else if(input_bytes(0x60) == 0x0f)
    {
        return "F1";
    }
    else if (read_char != key)
    {
        return key = read_char;
    }
    else
    {
        return 0;
    }
}