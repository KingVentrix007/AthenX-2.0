#include "serial.h"
#include "io_ports.h"
#include "string.h"
#define COM1_BASE_ADDRESS PORT
#define COM1_DATA_REGISTER (COM1_BASE_ADDRESS + 0)

int is_transmit_empty() {
   return inportb(PORT + 5) & 0x20;
}
 
void write_serial(char a,int port) {
   while (is_transmit_empty() == 0);
 
   outportb(port,a);
   //write_info(a);
}
void write_serial_i(int a) {
   while (is_transmit_empty() == 0);
 
   outportb(PORT,a);
}
int init_serial(int SERIAL_PORT) {
   output_bytes(PORT + 1, 0x00);    // Disable all interrupts
   output_bytes(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   output_bytes(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   output_bytes(PORT + 1, 0x00);    //                  (hi byte)
   output_bytes(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   output_bytes(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   output_bytes(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   output_bytes(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   output_bytes(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(input_bytes(PORT + 0) != 0xAE) {
      return 1;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   output_bytes(PORT + 4, 0x0F);
   return 0;
}

void write_string_serial(char *string, int port)
{
   init_serial(port);
   for(int i = 0; i < strlen(string); i++)
   {
      write_serial(string[i], port);
   }
}

int serial_received() {
   return inportb(PORT + 5) & 1;
}
 
char read_serial() {
   //while (serial_received() == 0) printf("1");
 
   return input_bytes(PORT);
}