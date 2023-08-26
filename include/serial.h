#ifndef _SERIAL_H
#define _SERIAL_H 1
#define PORT 0x3f8
#define DEFAULT_COM_DEBUG_PORT 0x3f8
void write_serial(char a,int port);
int is_transmit_empty();
int init_serial(int SERIAL_PORT);
void write_serial_i(int a);
char read_serial();



#endif