#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include <stdbool.h>

// Define constants for maximum values
#define MAX_PCI_DEVICES 500
#define MAX_DEVICE_NAME 200

// Define PCI configuration space addresses
#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

//Function pointer for device driver initialization
typedef int (*DeviceDriverPtr)(uint8_t, uint8_t,uint8_t);


// Structure for a PCI device
typedef struct {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_code;
    uint16_t sub_class;
    // Add more fields as needed
} PCI_Device;


// Struct to represent the PCI configuration space header
typedef struct {
    uint16_t vendor_id;         // Vendor ID
    uint16_t device_id;         // Device ID
    uint16_t command;           // Command register
    uint16_t status;            // Status register
    uint8_t  revision_id;       // Revision ID
    uint8_t  class_code;        // Class code
    uint8_t  subclass;          // Subclass
    uint8_t  interface;         // Interface
    uint8_t  prog_if;           // Programming Interface
    uint8_t  header_type;       // Header Type
    uint8_t  latency_timer;     // Latency Timer
    uint8_t  cache_line_size;   // Cache Line Size
    uint32_t bar[6];            // Base Address Registers (BARs)
    uint32_t cardbus_cis_ptr;   // CardBus CIS Pointer
    uint16_t subsystem_vendor;  // Subsystem Vendor ID
    uint16_t subsystem_id;      // Subsystem ID
    uint32_t expansion_rom;     // Expansion ROM Base Address
    uint8_t  capabilities_ptr;  // Capabilities Pointer
    uint8_t  reserved[7];       // Reserved for future use
    uint8_t  interrupt_line;    // Interrupt Line
    uint8_t  interrupt_pin;     // Interrupt Pin
    uint8_t  min_grant;         // Minimum Grant
    uint8_t  max_latency;       // Maximum Latency
} PCIHeader;

typedef struct{
    PCIHeader headers[MAX_PCI_DEVICES]; // Actual PCI device Headers
    int num_devices; // Number of devices in the PCI device list
}PCIDeviceList;



// Struct to store information about a PCI device
typedef struct {
    uint16_t device_id;
    uint16_t vendor_id;
    uint16_t class_code;
    uint16_t sub_class;
    char name[MAX_DEVICE_NAME];
    char device_name[MAX_DEVICE_NAME];
    bool has_driver;
    uint8_t bus;
    uint8_t device;
    uint8_t function;
    //This information is not presented to the user, but is for internal use
    PCIHeader header;
    DeviceDriverPtr init_device;
}RegisteredPCIDeviceInfo;

// Array to store a list of PCI devices

// Function to read a 32-bit value from PCI configuration space
uint32_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

bool add_pci_device(uint16_t vendor_id, uint16_t device_id, uint16_t class_code, const char* name);
// Function to enumerate PCI devices
void pci_scan();

// Function to log the number of PCI devices by class code
int log_devices(uint16_t class_code);
// Function to search for a PCI device based on vendor ID, device ID, and class code
RegisteredPCIDeviceInfo* find_pci_device(uint16_t vendor_id, uint16_t device_id, uint16_t class_code, uint8_t bus, uint8_t function, uint8_t device);

// Function to append a device to the list of devices
void append_pci_device(PCIDeviceList* list, PCIHeader* header);

// Function to verify that the device exists and retrieves its header info;
void verify_pci_device(uint16_t vendor_id, uint16_t device_id, PCIHeader* header);

// Function to initialize the PCI
void init_pci_device();

//Function to list the PCI devices available
void list_pci_devices();

// Function to list registered the PCI devices available
void list_registered_device();

// Function to register a PCI device
int create_device_list(uint16_t device_id, uint16_t vendor_id, uint16_t class_code, bool has_driver, char device_name[200], char producer_name[200], DeviceDriverPtr init_func);



#endif /* PCI_H */