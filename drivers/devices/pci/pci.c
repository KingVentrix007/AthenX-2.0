#include "printf.h"
#include "pci.h"
#include "pci2.h"
#include "io_ports.h"


uint32_t pci_read_config_space(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    // Construct the PCI configuration address
    uint32_t address = (1U << 31) | ((uint32_t)bus << 16) | ((uint32_t)device << 11) | ((uint32_t)function << 8) | (offset & 0xFC);

    // Write the address to the PCI configuration address port
    outportl(PCI_CONFIG_ADDRESS, address);

    // Read and return the data from the PCI configuration data port
    return inportl(PCI_CONFIG_DATA);
}

void pci_write_config_space(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint32_t data) {
    // Construct the PCI configuration address
    uint32_t address = (1U << 31) | ((uint32_t)bus << 16) | ((uint32_t)device << 11) | ((uint32_t)function << 8) | (offset & 0xFC);

    // Write the address to the PCI configuration address port
    outportl(PCI_CONFIG_ADDRESS, address);

    // Write the data to the PCI configuration data port
    outportl(PCI_CONFIG_DATA, data);
}

/*

*/

int scan_for_pci_device()
{
    int total_devices = 0;
    uint16_t bus;
    uint8_t device;
    for (bus = 0; bus < 256; bus++) {
         for (device = 0; device < 32; device++) {
             for (size_t function = 0; function < 8; function++)
             {
                uint32_t vendorDeviceID = pci_read_config_space(bus, device, function, 0);
                if(vendorDeviceID != 0xFFFF)
                {
                    
                }
             }
             
         }
     }
}                                                                                                                               

