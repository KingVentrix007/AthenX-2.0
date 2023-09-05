#ifndef e82540EM_ETHERNET_H
#define e82540EM_ETHERNET_H

#include <stdint.h>

// Vendor and Device IDs for Intel Ethernet e82540EM
#define INTEL_VENDOR_ID 0x8086  
#define e82540EM_DEVICE_ID  0x100e    

// // PCI configuration address and data ports (Replace with your actual ports)
// #define PCI_CONFIG_ADDRESS 0xCF8
// #define PCI_CONFIG_DATA    0xCFC

// Structure to represent a MAC address
typedef struct {
    uint8_t bytes[6];
} MacAddress;

// Function to map memory regions for the Intel e82540EM Ethernet card
void map_e82540EM_memory_regions(uint8_t bus, uint8_t device, uint8_t function);

// Function to initialize the PCI configuration for the Intel e82540EM Ethernet card
void init_e82540EM_pci_configuration();

// Function to initialize the Intel e82540EM Ethernet card
void init_e82540EM_ethernet_card(uint8_t bus, uint8_t device, uint8_t function);

// Function to read the MAC address from the Ethernet card
MacAddress read_mac_address();

int compare_mac_addresses(const char* qemuMac, const MacAddress* osMac);

#endif /* e82540EM_ETHERNET_H */
