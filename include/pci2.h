#ifndef __PCI2__H
#define __PCI2__H
#include <stdbool.h>
#include <stdint.h>


/**
 * Enum: Architecture
 * Description: Enum to indicate the architecture of the PCI device.
 */
typedef enum {
    ARCH_x86,
    ARCH_ARM,
    ARCH_OTHER,
    ARCH_i386
} Architecture;

/**
 * Struct Name: PciDevice
 * Description: Represents a PCI device with additional information.
 */
typedef struct {
    // PCI Configuration Space Header Fields
    uint8_t bus;
    uint8_t device;
    uint8_t function;
    uint16_t vendorId;
    char vendorIdChar[64];
    uint16_t deviceId;
    char deviceIdChar[64];
    uint16_t command;
    uint16_t status;
    uint32_t classCode;
    uint8_t subclass;
    uint8_t progInterface;
    uint8_t bist;
    uint8_t headerType;
    uint32_t baseAddressRegisters[6];
    uint16_t subsystemVendorId;
    uint16_t subsystemId;
    uint8_t interruptLine;
    uint8_t interruptPin;
    uint8_t minGnt;
    uint8_t maxLat;

    // Additional Information
    char deviceName[64];  // Adjust size as needed
    void (*initFunction)();  // Pointer to initialization function
    bool hasDriver;  // Indicates whether a driver is available
    bool BuiltInDriver; // Indicates whether a driver is loaded as a module or built in 
    bool irqRegistered;  // Indicates whether the IRQ is registered
    uint8_t deviceSpecificInfo[256];  // Field of 256 bytes for device-specific info
    Architecture architecture;  // Indicates the architecture of the PCI device
    // time_t startTime;  // Startup time

    // Additional Fields
    uint32_t capabilitiesPointer;  // Pointer to capabilities list
    uint16_t secondaryBus;  // Secondary bus number
    uint8_t pciLatencyTimer;  // PCI Latency Timer
    uint8_t cacheLineSize;  // Cache Line Size
    // Add any other fields you may need
} PciDevice;

struct pci_header
{

};
#endif