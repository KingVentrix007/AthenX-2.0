#include "../include/debug.h"
#include "../include/pci.h"
#include "../include/io_ports.h"
#include "../include/ethernet.h"
#include "../include/string.h"
#include "../include/vesa_display.h"
#include "../include/vesa.h"
#include "../include/serial.h"
#include "../include/speaker.h"
#include "../include/errno.h"
// typedef struct {
//     uint16_t device_id;
//     uint16_t vendor_id;
//     uint16_t class_code;
//     char name[MAX_DEVICE_NAME];
//     bool has_driver;
// } RegisteredPCIDeviceInfo;

// // Array to store a list of PCI devices
// RegisteredPCIDeviceInfo pciDevices[MAX_PCI_DEVICES];

//RegisteredPCIDeviceInfo x82540M_Ethernet_card; // PCI device information for the e82540EM Ethernet card
PCIDeviceList pci_list; // PCI device list (Invalid)
// PciDeviceInfoList pci_device_list; // PCI device list
RegisteredPCIDeviceInfo pci_registered_device_list[MAX_PCI_DEVICES];
RegisteredPCIDeviceInfo pci_registered_device_list_running[MAX_PCI_DEVICES];
int running_drivers = 0;
int num_devices_added_devices;

// pci_registered_device_list
// Array to store found device names
char found_devices[MAX_PCI_DEVICES][MAX_DEVICE_NAME];
int num_found_devices__registered = 0; // Number of found devices


// Counters for each type of device class
int Unclassified = 0;                    // Number of unclassified devices
int Mass_Storage_Controller = 0;          // Number of Mass Storage Controller devices
int Network_Controller = 0;               // Number of Network Controller devices
int Display_Controller = 0;               // Number of Display Controller devices
int Multimedia_Controller = 0;            // Number of Multimedia Controller devices
int Memory_Controller = 0;                // Number of Memory Controller devices
int Bridge = 0;                           // Number of Bridge devices
int Simple_Communication_Controller = 0;   // Number of Simple Communication Controller devices
int Base_System_Peripheral = 0;           // Number of Base System Peripheral devices
int Input_Device_Controller = 0;          // Number of Input Device Controller devices
int Docking_Station = 0;                  // Number of Docking Station devices
int Processor = 0;                        // Number of Processor devices
int Serial_Bus_Controller = 0;            // Number of Serial Bus Controller devices
int Wireless_Controller = 0;              // Number of Wireless Controller devices
int Intelligent_I_O_Controller = 0;       // Number of Intelligent I/O Controller devices
int Satellite_Communication_Controller = 0; // Number of Satellite Communication Controller devices
int Encryption_Decryption_Controller = 0;  // Number of Encryption/Decryption Controller devices
int Data_Acquisition_and_Signal_Processing_Controller = 0; // Number of Data Acquisition and Signal Processing Controller devices
int Processing_Accelerator = 0;           // Number of Processing Accelerator devices
// pciDevices[0] = 
// Function to read a 32-bit value from PCI configuration space
uint32_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    //write_serial("*",DEFAULT_COM_DEBUG_PORT);
    uint32_t address = (1U << 31) | ((uint32_t)bus << 16) | ((uint32_t)device << 11) | ((uint32_t)function << 8) | (offset & 0xFC);
    outportl(0xCF8, address);
    return inportl(0xCFC);
}
// uint32_t pci_read_config(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset) {
//     uint32_t address = (1U << 31) | ((uint32_t)bus << 16) | ((uint32_t)device << 11) | ((uint32_t)func << 8) | (offset & 0xFC);
//     outl(PCI_CONFIG_ADDRESS, address);
//     return inl(PCI_CONFIG_DATA);
// }


uint8_t read_bits_23_to_16(uint32_t value) {
    // Shift the bits to the right by 16 positions to align them to the least significant bits.
    // Then, mask with 0xFF to keep only the lower 8 bits.
    uint8_t result = (value >> 16) & 0xFF;
    return result;
}
// Function to enumerate PCI devices
void pci_scan() {
    //init_pci_device();
    //DEBUG("");    //write_serial("P",DEFAULT_COM_DEBUG_PORT);
    for (uint8_t bus = 0; bus < 256; ++bus) {
        
        // int old_pos_x = get_vesa_row();
        // int old_pos_y = get_vesa_col();
        // set_vesa_colum(400);
        // set_vesa_row(0);(vendorDeviceID >> 16) & 0xFFFF;
        // printf("Number of busses left: %d|",256-bus);
        // set_vesa_colum(old_pos_y);
        // set_vesa_row(old_pos_x); 
        for (uint8_t device = 0; device < 32; ++device) {
            // 
            for (uint8_t function = 0; function < 8; ++function) {
                uint32_t vendorDeviceID = pci_read(bus, device, function, 0);
                if (vendorDeviceID != 0xFFFF) {

                    // Found a PCI device, handle it
                    PCI_Device pciDevice_temp;
                    pciDevice_temp.vendor_id = vendorDeviceID & 0xFFFF;
                    pciDevice_temp.device_id = (vendorDeviceID >> 16) & 0xFFFF;
                    pciDevice_temp.class_code = (pci_read(bus, device, function, 0x08) >> 24) & 0xFF;
                    pciDevice_temp.sub_class = (pci_read(bus, device, function, 0x08) >> 16) & 0xFF;
                    if(pciDevice_temp.vendor_id != 0xFFFF)
                    {
                        RegisteredPCIDeviceInfo *device_s;
                        device_s = find_pci_device(pciDevice_temp.vendor_id,pciDevice_temp.device_id,pciDevice_temp.class_code,bus,function,device);
                        log_devices(pciDevice_temp.class_code);
                        PCIHeader header;
                        find_pci_device_header(bus,device,function,&header);
                        //printf("0x%04x",pciDevice_temp.vendor_id);
                        //printf("bus: %d, device: %d, function: %d\n",bus,device,function);
                        if(header.vendor_id != 0xFFFF)
                        {
                           
                            append_pci_device(&pci_list,&header);
                        }
                        
                    }
                    
             
                }
            }
        }
        if (bus <= 256 && bus >= 250)
        {
            //printf("BUS: %d\n", bus);
            break;
        }
    }
    //DEBUG("");
    int old_pos_row = get_vesa_row();
    int old_pos_col = get_vesa_col();
    set_vesa_row(0);
    set_vesa_colum(760);
          
           
    printf("----------------------------------------\n");
    set_vesa_colum(768);
    printf("PCI Device list:\n");
    set_vesa_colum(768);
    printf("Unclassified (0x0): %d\n", Unclassified);
    set_vesa_colum(768);
    printf("Mass Storage Controller (0x1): %d\n", Mass_Storage_Controller);
    set_vesa_colum(768);
    printf("Network Controller (0x2): %d\n", Network_Controller);
    set_vesa_colum(768);
    printf("Display Controller (0x3): %d\n", Display_Controller);
    set_vesa_colum(768);
    printf("Multimedia Controller (0x4): %d\n", Multimedia_Controller);
    set_vesa_colum(768);
    printf("Memory Controller (0x5): %d\n", Memory_Controller);
    set_vesa_colum(768);
    printf("Bridge (0x6): %d\n", Bridge);
    set_vesa_colum(768);
    printf("Simple Communication Controller (0x7): %d\n", Simple_Communication_Controller);
    set_vesa_colum(768);
    printf("Base System Peripheral (0x8): %d\n", Base_System_Peripheral);
    set_vesa_colum(768);
    printf("Input Device Controller (0x9): %d\n", Input_Device_Controller);
    set_vesa_colum(768);
    printf("Docking Station (0x10): %d\n", Docking_Station);
    set_vesa_colum(768);
    printf("Processor (0x11): %d\n", Processor);
    set_vesa_colum(768);
    printf("Serial Bus Controller (0x12): %d\n", Serial_Bus_Controller);
    set_vesa_colum(768);
    printf("Wireless Controller (0x13): %d\n", Wireless_Controller);
    set_vesa_colum(768);
    printf("Intelligent I/O Controller (0x14): %d\n", Intelligent_I_O_Controller);
    set_vesa_colum(768);
    printf("Satellite Communication Controller (0x15): %d\n", Satellite_Communication_Controller);
    set_vesa_colum(768);
    printf("Encryption/Decryption Controller (0x16): %d\n", Encryption_Decryption_Controller);
    set_vesa_colum(768);
    printf("Data Acquisition and Signal Processing Controller (0x17): %d\n", Data_Acquisition_and_Signal_Processing_Controller);
    set_vesa_colum(768);
    printf("Processing Accelerator (0x18): %d\n", Processing_Accelerator);
     set_vesa_colum(old_pos_col);
    set_vesa_row(old_pos_row); 
    //printf("--------------------------------------------------------------------\n");
    main_exit(SUCCESS,SUCCESS,__FUNCTION__,false,&dummy);

    
// }
// In this code, each counter variable is incremented when there's a match for the corresponding class code, and at the end of the switch statement, the counters are printed to show the counts for each class code.






}




// Function to read a 32-bit value from a PCI device's BAR
uint32_t pci_read_bar(uint8_t bus, uint8_t slot, uint8_t func, uint8_t bar_num) {
    // Calculate the address for the BAR register
    uint32_t address = (1U << 31) | ((uint32_t)bus << 16) | ((uint32_t)slot << 11) | ((uint32_t)func << 8) | (bar_num << 2);

    // Write the address to PCI_CONFIG_ADDRESS to select the BAR
    outportl(PCI_CONFIG_ADDRESS, address);

    // Read and return the value from the BAR
    return inportl(PCI_CONFIG_DATA);
}

// Function to write a 32-bit value to a PCI device's BAR
void pci_write_bar(uint8_t bus, uint8_t slot, uint8_t func, uint8_t bar_num, uint32_t value) {
    // Calculate the address for the BAR register
    uint32_t address = (1U << 31) | ((uint32_t)bus << 16) | ((uint32_t)slot << 11) | ((uint32_t)func << 8) | (bar_num << 2);

    // Write the address to PCI_CONFIG_ADDRESS to select the BAR
    outportl(PCI_CONFIG_ADDRESS, address);

    // Write the value to the BAR
    outportl(PCI_CONFIG_DATA, value);
}

// Function to get the PCI configuration space header of a device
// PCIHeader pci_get_header(uint8_t bus, uint8_t slot, uint8_t func) {
//     PCIHeader header;

//     // Read the vendor ID and device ID from the PCI configuration space
//     header.vendor_id = inw(PCI_CONFIG_DATA);
//     header.device_id = inw(PCI_CONFIG_DATA + 2);

//     // Read more fields from the configuration space header as needed

//     return header;
// }



// Function to calculate the slot value
uint32_t calculate_pci_slot(uint8_t bus, uint8_t device, uint8_t function) {
    // The upper 8 bits are the bus number, the next 5 bits are the device number,
    // and the lowest 3 bits are the function number.
    return ((uint32_t)bus << 8) | ((uint32_t)device << 3) | (uint32_t)function;
}

int dummy(uint8_t dummy1, uint8_t dummy2, uint8_t dummy3)
{
    printf("Dummy Call");
}

void init_pci_device()
{
    asm volatile("cli");
    num_devices_added_devices = 0;
    printf("Initializing PCI Device Headers\n");
    // RegisteredPCIDeviceInfo newDevice;

    int ret = create_device_list(0x100e,0x8086,0x2,true,"82540EM Gigabit Ethernet Controller","Intel Corporation",init_e82540EM_ethernet_card);
    if(ret != 0) return main_exit(PCI_ERROR,PCI_ERR_DEVICE_NOT_FOUND,__FUNCTION__,true,&dummy);
    ret = create_device_list(0x2415,0x8086,0x4,false,"82801AA AC'97 Audio Controller","Intel Corporation",dummy);
    if(ret != 0) return main_exit(PCI_ERROR,PCI_ERR_DEVICE_NOT_FOUND,__FUNCTION__,true,&dummy);
    ret = create_device_list(0x1229,0x8086,0x2,false,"82557/8/9/0/1 Ethernet Pro 100","Intel Corporation",dummy);
    if(ret != 0) return main_exit(PCI_ERROR,PCI_ERR_DEVICE_NOT_FOUND,__FUNCTION__,true,&dummy);
    
    printf("Initialized PCI Device headers\n");
    printf("Scanning PCI for devices\n");
    pci_scan();
    //printf("Scanned PCI.\n");
    printf("[PCI] Found %d devices\n",pci_list.num_devices);
     printf_("{/330:0,0,225}");
    printf("[PCI] Found %d registered devices\n",num_found_devices__registered);
     printf_("{/330:0,255,0}");
     //asm("sti");
    if(num_devices_added_devices >= 1)
    {
        initialize_registered_devices();
    }
    asm volatile("sti");
   
}

// Creates a pci device
int create_device_list(uint16_t device_id, uint16_t vendor_id, uint16_t class_code, bool has_driver, char device_name[200], char producer_name[200], DeviceDriverPtr init_func)
{
    RegisteredPCIDeviceInfo new_device_info;
    new_device_info.device_id = device_id;
    new_device_info.vendor_id = vendor_id;
    new_device_info.class_code = class_code;
    new_device_info.has_driver = has_driver;
    new_device_info.is_running = false;
    new_device_info.init_device = init_func;
    strcpy(new_device_info.device_name, device_name);
    strcpy(new_device_info.name, producer_name);
    pci_registered_device_list[num_devices_added_devices] = new_device_info;
    num_devices_added_devices += 1;
    return 0;

}

bool add_pci_device(uint16_t vendor_id, uint16_t device_id, uint16_t class_code, const char* name) {
    for (int i = 0; i < MAX_PCI_DEVICES; i++) {
        if (pci_registered_device_list[i].vendor_id == 0 && pci_registered_device_list[i].device_id == 0) {
            // Found an empty slot, append the new device
            pci_registered_device_list[i].vendor_id = vendor_id;
            pci_registered_device_list[i].device_id = device_id;
            pci_registered_device_list[i].class_code = class_code;
            strncpy(pci_registered_device_list[i].name, name, MAX_DEVICE_NAME - 1);
            pci_registered_device_list[i].name[MAX_DEVICE_NAME - 1] = '\0'; // Ensure null-terminated
            pci_registered_device_list[i].has_driver = false; // Assuming no driver initially
            return true; // Successfully appended
        }
    }
    return false; // List is full, cannot append
}




int log_devices(uint16_t class_code)
{
    switch (class_code) {
        case 0x00:
            //printf("Unclassified\n");
            Unclassified++;
            break;
        case 0x01:
            //printf("Mass Storage Controller\n");
            Mass_Storage_Controller++;
            break;
        case 0x02:
            //printf("Network Controller\n");
            Network_Controller++;
            break;
        case 0x03:
            //printf("Display Controller\n");
            Display_Controller++;
            break;
        case 0x04:
            //printf("Multimedia Controller\n");
            Multimedia_Controller++;
            break;
        case 0x05:
            //printf("Memory Controller\n");
            Memory_Controller++;
            break;
        case 0x06:
            //printf("Bridge\n");
            Bridge++;
            break;
        case 0x07:
            //printf("Simple Communication Controller\n");
            Simple_Communication_Controller++;
            break;
        case 0x08:
            //printf("Base System Peripheral\n");
            Base_System_Peripheral++;
            break;
        case 0x09:
            //printf("Input Device Controller\n");
            Input_Device_Controller++;
            break;
        case 0x0A:
            //printf("Docking Station\n");
            Docking_Station++;
            break;
        case 0x0B:
            //printf("Processor\n");
            Processor++;
            break;
        case 0x0C:
            //printf("Serial Bus Controller\n");
            Serial_Bus_Controller++;
            break;
        case 0x0D:
            //printf("Wireless Controller\n");
            Wireless_Controller++;
            break;
        case 0x0E:
            //printf("Intelligent I/O Controller\n");
            Intelligent_I_O_Controller++;
            break;
        case 0x0F:
            //printf("Satellite Communication Controller\n");
            Satellite_Communication_Controller++;
            break;
        case 0x10:
            //printf("Encryption/Decryption Controller\n");
            Encryption_Decryption_Controller++;
            break;
        case 0x11:
            //printf("Data Acquisition and Signal Processing Controller\n");
            Data_Acquisition_and_Signal_Processing_Controller++;
            break;
        case 0x12:
            //printf("Processing Accelerator\n");
            Processing_Accelerator++;
            break;
        default:
            //printf("Unknown class code\n");
            break;
    }
}

// Function to search for a PCI device based on vendor ID, device ID, and class code
RegisteredPCIDeviceInfo* find_pci_device(uint16_t vendor_id, uint16_t device_id, uint16_t class_code, uint8_t bus, uint8_t function, uint8_t device) {
    for (int i = 0; i < MAX_PCI_DEVICES; i++) {
        if (pci_registered_device_list[i].vendor_id == vendor_id &&
            pci_registered_device_list[i].device_id == device_id &&
            pci_registered_device_list[i].class_code == class_code) {
            // Check if the name is already in found_devices
            bool name_found = false;
            for (int j = 0; j < num_found_devices__registered; j++) {
                if (strcmp(pci_registered_device_list[i].device_name, found_devices[j]) == 0) {
                    name_found = true;
                    
                     
                        //sleep(1000);
                    break; // Name already found, no need to add it again
                }
            }
            if (!name_found) {
                //! DO NOT REMOVE
                // Add the name to found_devices
                // printf_("{/330:0,0,225}");
                // printf("Device Found:\n");
                // printf("Vendor ID: 0x%X\n", vendor_id);
                // printf("Device ID: 0x%X\n", device_id);
                // printf("Class Code: 0x%X\n", class_code);
                // printf("Developer Name: %s\n", pciDevices[i].name);
                // printf("Device Name: %s\n", pciDevices[i].device_name);
                // printf("Driver Available: %s\n", pciDevices[i].has_driver ? "Yes" : "No");
                // printf_("{/330:0,255,0}");
                find_pci_device_header(bus,device,function,&pci_registered_device_list[i].header);
                pci_registered_device_list[i].sub_class = pci_registered_device_list[i].header.subclass;
                // for (size_t x = 0; x < 5; x++)
                // {
                //     uint32_t bar_value = *(volatile uint32_t *)pciDevices[i].header.bar[x];
                //     uint32_t out = inportl(bar_value);
                //     printf("0x%x", out);
                // }
                //! CAN BE REMOVED
                // printf("\nD_ID: 0x%x\n",pciDevices[i].header.device_id);
                // printf("V_ID: 0x%x\n",pciDevices[i].header.vendor_id);
                
                      
                //beep();
                strcpy(found_devices[num_found_devices__registered], pci_registered_device_list[i].device_name);
                pci_registered_device_list[i].bus = bus;
                pci_registered_device_list[i].device = device;
                pci_registered_device_list[i].function = function;

                num_found_devices__registered++;
                // if(device_id == e82540EM_DEVICE_ID && vendor_id == 0x8086)
                // {
                //     init_e82540EM_ethernet_card(bus,device,function);
                // }
                
            }
            return &pci_registered_device_list[i]; // Return a pointer to the matching device
        }
    }
    return NULL; // Device not found
}

// Function to extract bits 31-24 from a uint32_t
uint8_t extractBits31to24(uint32_t value) {
    return (uint8_t)((value >> 24) & 0xFF);
}

// Function to extract bits 23-16 from a uint32_t
uint8_t extractBits23to16(uint32_t value) {
    return (uint8_t)((value >> 16) & 0xFF);
}

// Function to extract bits 15-8 from a uint32_t
uint8_t extractBits15to8(uint32_t value) {
    return (uint8_t)((value >> 8) & 0xFF);
}

// Function to extract bits 7-0 from a uint32_t
uint8_t extractBits7to0(uint32_t value) {
    return (uint8_t)(value & 0xFF);
}

// Function to find a PCI device and populate the PCIHeader struct
void find_pci_device_header(uint8_t bus, uint8_t device, uint8_t func, PCIHeader* header) {
    
    uint16_t read_vendor_id;
    uint16_t read_device_id;
    uint32_t vendorDeviceID = pci_read(bus, device, func, 0);
    read_vendor_id = vendorDeviceID & 0xFFFF;
    read_device_id = (vendorDeviceID >> 16) & 0xFFFF;

    if (read_vendor_id == 0xFFFF || read_device_id == 0xFFFF) {
        // Device not found, set appropriate values in the header
        //printf("WARN");
        header->vendor_id = 0xFFFF;
        header->device_id = 0xFFFF;
        // Set other fields as needed
        return;
    }

    //TODO THIS IS BROKEN, fix how the fields are filled in
    //Device found, populate the header struct
    header->vendor_id = read_vendor_id;
    header->device_id = read_device_id;
    header->command = pci_read(bus, device, func, 0x04);
    header->status = (pci_read(bus, device, func, 0x04) >> 24) & 0xFF;
    header->revision_id = pci_read(bus, device, func, 0x08);
    header->class_code = (pci_read(bus, device, func, 0x08) >> 24) & 0xFF;
    
    header->subclass = (pci_read(bus, device, func, 0x08) >> 16) & 0xFF;
    header->interface = pci_read(bus, device, func, 0x09);
    header->prog_if = pci_read(bus, device, func, 0x09);
    header->header_type = pci_read(bus, device, func, 0x0E);
    header->latency_timer = pci_read(bus, device, func, 0x0D);
    header->cache_line_size = pci_read(bus, device, func, 0x0C);

    // Read the BARs
    for (int i = 0; i < 6; i++) {
        header->bar[i] = pci_read(bus, device, func, 0x10 + i * 4);
    }

    header->cardbus_cis_ptr = pci_read(bus, device, func, 0x28);
    header->subsystem_vendor = pci_read(bus, device, func, 0x2C);
    header->subsystem_id = pci_read(bus, device, func, 0x2E);
    header->expansion_rom = pci_read(bus, device, func, 0x30);
    header->capabilities_ptr = pci_read(bus, device, func, 0x34);

    // Read the remaining fields
    header->interrupt_line = extractBits7to0(pci_read(bus, device, func, 0x3C));
    header->interrupt_pin = pci_read(bus, device, func, 0x3D);
    header->min_grant = pci_read(bus, device, func, 0x3E);
    header->max_latency = pci_read(bus, device, func, 0x3F);
}


// Function to append a PCI device to the list
void append_pci_device(PCIDeviceList* list, PCIHeader* header) {
     
    if (list->num_devices < MAX_PCI_DEVICES) {
        list->headers[list->num_devices] = *header;
        list->num_devices++;
    } else {
        printf("PCI device list is full. Cannot append more devices.\n");
    }
}

void list_pci_devices()
{
     
     printf("Number of devices: %d\n", pci_list.num_devices);
    //printf("Number of real found devices: %d\n",num_found_devices);
    for (int i = 0; i < pci_list.num_devices; i++) {
        printf("Device %d:\n", i);
        printf("Class code: 0x%x\n", pci_list.headers[i].class_code);
        printf("Vendor ID: 0x%x\n", pci_list.headers[i].vendor_id);
        printf("Device ID: 0x%x\n", pci_list.headers[i].device_id);
        printf("Device Sub class: 0x%x\n", pci_list.headers[i].subclass);
        
    }
}
void list_pic_class_code(int class_code, int sub_class_code)
{
    if(class_code != 0 && sub_class_code != 0)
    {

    }
    else if (class_code != 0 && sub_class_code == 0)
    {
        /* code */
    }
    else if (class_code == 0 && sub_class_code != 0)
    {
         for (int i = 0; i < pci_list.num_devices; i++)
         {
            if(pci_list.headers[i].subclass == sub_class_code)
            {
                 printf("Number of devices: %d\n", pci_list.num_devices);
                for (int i = 0; i < pci_list.num_devices; i++) 
                {
                    printf("Device %d:\n", i);
                    printf("Class code: 0x%x\n", pci_list.headers[i].class_code);
                    printf("Vendor ID: 0x%x\n", pci_list.headers[i].vendor_id);
                    printf("Device ID: 0x%x\n", pci_list.headers[i].device_id);
                    printf("Device Sub class: 0x%x\n", pci_list.headers[i].subclass);
                
                }
            }
         }
    }
    else
    {
         printf("Number of devices: %d\n", pci_list.num_devices);
    //printf("Number of real found devices: %d\n",num_found_devices);
        for (int i = 0; i < pci_list.num_devices; i++) {
            printf("Device %d:\n", i);
            printf("Class code: 0x%x\n", pci_list.headers[i].class_code);
            printf("Vendor ID: 0x%x\n", pci_list.headers[i].vendor_id);
            printf("Device ID: 0x%x\n", pci_list.headers[i].device_id);
            printf("Device Sub class: 0x%x\n", pci_list.headers[i].subclass);
        
        }
    }
    
    
}

void list_registered_device()
{
    for (size_t i = 0; i < num_found_devices__registered; i++)
    {

        printf_("{/330:0,0,225}");
        printf("Device Found:\n");
        printf("Class Code: 0x%X\n", pci_registered_device_list[i].class_code);
        printf("Sub class Code: 0x%X\n", pci_registered_device_list[i].sub_class);
        printf("Vendor ID: 0x%X\n", pci_registered_device_list[i].vendor_id);
        printf("Developer Name: %s\n", pci_registered_device_list[i].name);
        printf("Device ID: 0x%X\n", pci_registered_device_list[i].device_id);
        printf("Device Name: %s\n", pci_registered_device_list[i].device_name);
        printf("Driver Available: %s\n", pci_registered_device_list[i].has_driver ? "Yes" : "No");
        printf("Driver running: %s\n",pci_registered_device_list[i].is_running ? "Yes" : "No");
        printf_("{/330:0,255,0}");
        printf("\n");
    }
    
}

void list_running_devices()
{
    for (size_t i = 0; i < running_drivers; i++)
    {
        printf_("{/330:0,0,225}");
        printf("Device Found:\n");
        printf("Class Code: 0x%X\n", pci_registered_device_list_running[i].class_code);
        printf("Sub class Code: 0x%X\n", pci_registered_device_list_running[i].sub_class);
        printf("Vendor ID: 0x%X\n", pci_registered_device_list_running[i].vendor_id);
        printf("Developer Name: %s\n", pci_registered_device_list_running[i].name);
        printf("Device ID: 0x%X\n", pci_registered_device_list_running[i].device_id);
        printf("Device Name: %s\n", pci_registered_device_list_running[i].device_name);
        printf("Driver Available: %s\n", pci_registered_device_list_running[i].has_driver ? "Yes" : "No");
        printf_("{/330:0,255,0}");
        printf("\n");
    }
    
}

void initialize_registered_devices()
{
    printf("Initializing registered devices\n");
    for (size_t i = 0; i < num_found_devices__registered; i++)
    {
        if(pci_registered_device_list[i].has_driver == true && pci_registered_device_list[i].is_running == false)
        {
            printf("Calling %s initialization function \n", pci_registered_device_list[i].device_name);
            pci_registered_device_list[i].init_device(pci_registered_device_list[i].bus, pci_registered_device_list[i].device, pci_registered_device_list[i].function);
            pci_registered_device_list[i].is_running = true;
            pci_registered_device_list_running[running_drivers] = pci_registered_device_list[i];
            //printf("initialized");
        }
    }
    
}

RegisteredPCIDeviceInfo* return_registered_pci_device(uint8_t bus, uint8_t device, uint8_t func)
{
    uint16_t read_vendor_id;
    uint16_t read_device_id;
    uint32_t vendorDeviceID = pci_read(bus, device, func, 0);
    read_vendor_id = vendorDeviceID & 0xFFFF;
    read_device_id = (vendorDeviceID >> 16) & 0xFFFF;
    for (size_t i = 0; i < num_found_devices__registered; i++)
    {
        if(pci_registered_device_list[i].vendor_id == read_vendor_id && pci_registered_device_list[i].device_id == read_device_id)
        {
            return &pci_registered_device_list[i];
        }
    }
    
}

PCIDevice *PCI_search_device(uint16 vendor_id, uint16 device_id) {
    PCIDevice *device;
    RegisteredPCIDeviceInfo *temp_dev;
    for (size_t i = 0; i < num_found_devices__registered; i++)
    {
        if(pci_registered_device_list[i].vendor_id == vendor_id && pci_registered_device_list[i].device_id == device_id)
        {
            temp_dev = &pci_registered_device_list[i];
        }
    }
    device->BAR0 = temp_dev->header.bar[0];
    device->bus = temp_dev->bus;
    device->device_id = temp_dev->device_id;
    device->device_name = temp_dev->device_name;
    device->IRQ = temp_dev->header.interrupt_line;
    device->vendor_id = temp_dev->vendor_id;
    device->vendor_name = temp_dev->name;
    if(device->vendor_id == vendor_id && device->device_id == device_id)
    {
        return device;
    }
    // while (device) {
    //     if (device->vendor_id == vendor_id && device->device_id == device_id)
    //         r
    //     device = device->next;
    // }

    return 0;
}