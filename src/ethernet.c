#include "display.h"
#include "pci.h"
#include <stdint.h>
#include "stddef.h"
#include "ethernet.h"


// PCI configuration address and data ports (Replace with your actual ports)
// #define PCI_CONFIG_ADDRESS 0xCF8
// #define PCI_CONFIG_DATA    0xCFC

#define ee82540EM_BAR_MMIO   0 // Memory-mapped I/O BAR
#define ee82540EM_BAR_SIZE   0x1000 


// Define the base address for the Intel Ethernet e82540EM MMIO region
#define ee82540EM_BAR_MMIO   0 // Memory-mapped I/O BAR
#define e82540EM_MAC_ADDR_OFFSET 0x5400  // Offset for the MAC address registers (example value)


volatile uint8_t* physical_memory_base = (volatile uint8_t*)0x100000;

// Function to map a physical MMIO region to a virtual address
void* map_physical_mmio(uint64_t physical_addr, size_t size) {
    // Calculate the virtual address by adding the physical offset
    void* virtual_addr = (void*)(physical_memory_base + physical_addr);
    
    return virtual_addr;
}
// Function to initialize the PCI configuration for the Intel e82540EM Ethernet card
void init_e82540EM_pci_configuration() {
    // Find the Intel e82540EM Ethernet card by its vendor and device IDs
    uint8_t bus, device, function;
    printf_("e82540EM_pci_configuration");
    for (bus = 0; bus < 256; ++bus) {
       
        for (device = 0; device < 32; ++device) {
            for (function = 0; function < 8; ++function) {
                uint32_t vendorDeviceID = pci_read(bus, device, function, 0);
                if (vendorDeviceID != 0xFFFFFFFF &&
                    (vendorDeviceID & 0xFFFF) == INTEL_VENDOR_ID &&
                    ((vendorDeviceID >> 16) & 0xFFFF) == e82540EM_DEVICE_ID) {
                        printf_("initialized\n");
                    // Found the Intel e82540EM Ethernet card
                    // Perform PCI configuration initialization here
                    // This might include enabling bus mastering, memory-mapped I/O, etc.
                    return;
                }
                // else
                // {
                //     _printf("uninitialized: VendorDeviceID: 0x%x\n", vendorDeviceID);
                // }
            }
        }
    }
    print("END");
}

// Function to map memory regions for the Intel e82540EM Ethernet card
void map_e82540EM_memory_regions(uint8_t bus, uint8_t device, uint8_t function) {
    // Find the Intel 82540EM Ethernet card (you should already have done this)
    //uint8_t bus, device, function;
    // ...
    printf("Initializing Intel 82540EM Ethernet card");
    // Read the base address register for memory-mapped I/O
    uint32_t mmioBase = pci_read(bus, device, function, ee82540EM_BAR_MMIO);

    // Determine the type of BAR (memory-mapped I/O)
    uint32_t barType = mmioBase & 0x1;
    if (barType == 0) {
        // This is a memory-mapped I/O region

        // Map the physical MMIO address directly (implementation may vary)
        // Replace this with architecture-specific MMIO mapping code
        map_physical_mmio(mmioBase & 0xFFFFFFF0, ee82540EM_BAR_SIZE);

        // Now you can access the device's registers using the mapped physical MMIO address
    }
}

// Function to initialize the Intel e82540EM Ethernet card
void init_e82540EM_ethernet_card(uint8_t bus, uint8_t device, uint8_t function) {
    //init_e82540EM_pci_configuration();  // Step 2: Initialize PCI configuration
    map_e82540EM_memory_regions(bus,device,function);      // Step 3: Map memory regions
    // Step 4: Initialize the Ethernet card, set up MAC address, buffers, etc.
    // This step involves writing to the Ethernet card's registers.
    // Refer to the card's datasheet for specific details.
}

// Define a pointer to represent the base address of physical memory



// Function to read the MAC address from the Ethernet card

MacAddress read_mac_address() {
    MacAddress mac;
    
    // Calculate the virtual address of the MMIO region using map_physical_mmio
    void* mmio_base = map_physical_mmio(ee82540EM_BAR_MMIO, e82540EM_MAC_ADDR_OFFSET + 6); // 6 bytes for MAC address

    // Calculate the virtual address of the MAC address register
    volatile uint8_t* mac_reg = (volatile uint8_t*)((uintptr_t)mmio_base + e82540EM_MAC_ADDR_OFFSET);

    // Read the MAC address from the register (assuming little-endian byte order)
    for (int i = 0; i < 6; i++) {
        mac.bytes[i] = mac_reg[i];
    }

    // Unmap the MMIO region (if needed)

    return mac;
}

int compare_mac_addresses(const char* qemuMac, const MacAddress* osMac) {
    for (int i = 0; i < 6; i++) {
        if (qemuMac[i] != osMac->bytes[i]) {
            return 0; // MAC addresses are different
        }
    }
    return 1; // MAC addresses match
}



// In this code:

// compare_mac_addresses takes the MAC address obtained from QEMU (qemuMac) as a string and the MAC address obtained from your custom OS (osMac) as a MacAddress structure.

// It compares each byte of the MAC addresses and returns 1 if they match and 0 if they don't.

// In the main function, you can call read_mac_address() to get the MAC address from your custom OS, and then compare it to the QEMU-provided MAC address.

// This code will print a message indicating whether the MAC addresses match or not.





