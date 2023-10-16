#ifndef X86_REGISTER_UTILS_H
#define X86_REGISTER_UTILS_H

#include <stdint.h>

// Function to set the value of EAX register
void set_EAX(uint32_t value);

// Function to set the value of EBX register
void set_EBX(uint32_t value);

// Function to set the value of ECX register
void set_ECX(uint32_t value);

// Function to set the value of EDX register
void set_EDX(uint32_t value);

// Function to set the value of ESI register
void set_ESI(uint32_t value);

// Function to set the value of EDI register
void set_EDI(uint32_t value);

// Function to get the value of EAX register
uint32_t get_EAX();

// Function to get the value of EBX register
uint32_t get_EBX();

// Function to get the value of ECX register
uint32_t get_ECX();

// Function to get the value of EDX register
uint32_t get_EDX();

// Function to get the value of ESI register
uint32_t get_ESI();

// Function to get the value of EDI register
uint32_t get_EDI();

#endif // X86_REGISTER_UTILS_H
