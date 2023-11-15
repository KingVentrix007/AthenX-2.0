#include "../include/debug.h"
#include "../include/vesa.h"
#include "../include/vesa_display.h"
#include "../include/isr.h"
#include "../include/printf.h"
#include "../include/io_ports.h"
#include "../include/display.h"
#include "../include/pci.h"
#include <stdint.h>
#include "../include/stddef.h"
#include "../include/e1000.h"
#include "../include/pci.h"
#include "../include/string.h"
//#include "../include/memory.h"
#include "../include/speaker.h"
#include "../include/errno.h"
#include "../include/kheap.h"
#include "../include/DHCP.h"

const char* eth_error_to_string(uint16_t error_code) {
    //printf("HERE");
    switch (error_code) {
        
        case ETH_ERR_NO_DRIVER:
            return "No Ethernet driver found";
        case ETH_ERR_INVALID_MAC:
            return "Invalid MAC address detected";
        case ETH_ERR_BUFFER_OVERFLOW:
            return "Ethernet receive buffer overflow";
        case ETH_ERR_PACKET_TOO_LARGE:
            return "Received packet is too large to handle";
        case ETH_ERR_PACKET_TOO_SMALL:
            return "Received packet is too small to process";
        case ETH_ERR_LINK_DOWN:
            return "Ethernet link is down";
        case ETH_ERR_INVALID_IP_HEADER:
            return "Invalid IP header detected in an incoming packet";
        case ETH_ERR_INVALID_ARP_PACKET:
            return "Invalid ARP packet received";
        case ETH_ERR_INVALID_UDP_PACKET:
            return "Invalid UDP packet received";
        case ETH_ERR_INVALID_TCP_PACKET:
            return "Invalid TCP packet received";
        case ETH_ERR_IP_CONFLICT:
            return "IP address conflict detected";
        case ETH_ERR_DHCP_FAILURE:
            return "DHCP configuration failed";
        case ETH_ERR_TCP_TIMEOUT:
            return "Timeout while waiting for a TCP connection";
        case ETH_ERR_UDP_PORT_IN_USE:
            return "UDP port already in use";
        case ETH_ERR_INVALID_PACKET_TYPE:
            return "Invalid Ethernet packet type";
        case ETH_ERR_BUFFER_ALLOCATION_FAILED:
            return "Failed to allocate memory for Ethernet buffer";
        case ETH_ERR_INVALID_DESTINATION_MAC:
            return "Destination MAC address is not recognized";
        case ETH_ERR_INVALID_PACKET_LENGTH:
            return "Invalid Ethernet packet length";
        case ETH_ERR_RX_FIFO_OVERFLOW:
            return "Receive FIFO overflow";
        case ETH_ERR_TX_FIFO_OVERFLOW:
            return "Transmit FIFO overflow";
        case ETH_ERR_PHY_FAILURE:
            return "Ethernet PHY hardware failure";
        case ETH_ERR_INVALID_VLAN_TAG:
            return "Invalid VLAN tag detected";
        case ETH_ERR_MULTICAST_FILTER_FULL:
            return "Multicast filter table is full";
        case ETH_ERR_INVALID_IPV6_PACKET:
            return "Invalid IPv6 packet received";
        case ETH_ERR_PACKET_DROPPED:
            return "Packet dropped due to congestion";
        case ETH_ERR_INVALID_MTU:
            return "Invalid Maximum Transmission Unit (MTU) value";
        case ETH_ERR_TRANSMIT_ERROR:
            return "Error while transmitting data";
        case ETH_ERR_INVALID_PACKET_HEADER:
            return "Invalid Ethernet packet header";
        case ETH_ERR_PROMISCUOUS_MODE:
            return "Inappropriate use of promiscuous mode";
        case ETH_ERR_INVALID_LINK_SPEED:
            return "Invalid Ethernet link speed detected";
        default:
            return "Unknown Ethernet error";
    }
}



typedef uint32_t uint;
uint64_t mem_base;
// PCI configuration address and data ports (Replace with your actual ports)
// #define PCI_CONFIG_ADDRESS 0xCF8
// #define PCI_CONFIG_DATA    0xCFC

//#define ee82540EM_BAR_MMIO   0 // Memory-mapped I/O BAR
#define ee82540EM_BAR_SIZE   0x1000 
#define REG_EEPROM      0x0014
uint16_t ee82540EM_BAR_MMIO;
NicCardInfo card;
struct e1000_tx_desc *tx_descs[E1000_NUM_TX_DESC];
struct e1000_rx_desc *rx_descs[E1000_NUM_RX_DESC];
// Define the base address for the Intel Ethernet e82540EM MMIO region
//#define ee82540EM_BAR_MMIO   0 // Memory-mapped I/O BAR
#define e82540EM_MAC_ADDR_OFFSET 0x5400  // Offset for the MAC address registers (example value)
uint8_t bar_type;     // Type of BAR0
uint16_t io_base;     // IO Base Address
uint64_t  mem_base;   // MMIO Base Address
bool eerprom_exists;  // A flag indicating if eeprom exists
uint8_t mac [6];      // A buffer for storing the mack address
struct e1000_rx_desc *rx_descs[E1000_NUM_RX_DESC]; // Receive Descriptor Buffers
struct e1000_tx_desc *tx_descs[E1000_NUM_TX_DESC]; // Transmit Descriptor Buffers
uint16_t rx_cur;      // Current Receive Descriptor Buffer
uint16_t tx_cur;      // Current Transmit Descriptor Buffer



volatile uint8_t* physical_memory_base = (volatile uint8_t*)0x100000;

// Function to map a physical MMIO region to a virtual address
void* map_physical_mmio(uint64_t physical_addr, size_t size) {
    // Calculate the virtual address by adding the physical offset
    void* virtual_addr = (void*)(physical_memory_base + physical_addr);
    
    return virtual_addr;
}


// Function to map memory regions for the Intel e82540EM Ethernet card
void map_e82540EM_memory_regions(uint8_t bus, uint8_t device, uint8_t function) {
    // Find the Intel 82540EM Ethernet card (you should already have done this)
    //uint8_t bus, device, function;
    // ...
    //printf("[Ethernet]Initializing Intel 82540EM Ethernet card");
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

void print_mac_address(const MacAddress* mac) {
    printf("MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
        mac->bytes[0], mac->bytes[1], mac->bytes[2],
        mac->bytes[3], mac->bytes[4], mac->bytes[5]);
}
// Function to initialize the Intel e82540EM Ethernet card
int init_e82540EM_ethernet_card(uint8_t bus, uint8_t device, uint8_t function) {
     FUNC_ADDR_NAME(&init_e82540EM_ethernet_card,3,"iii");
    //printf("HERERE");
    //init_e82540EM_pci_configuration();  // Step 2: Initialize PCI configuration
    //init_E1000();
    RegisteredPCIDeviceInfo *device_info;
    device_info = return_registered_pci_device(bus,device,function);
    PCI_Device *device_info_add = PCI_search_device(device_info->vendor_id,device_info->device_id);
    int offset = get_bar_offset(0);
    bar_type = get_bar_type(bus,device,function,0);
    printf("bar type = %d\n",bar_type);
    //map_e82540EM_memory_regions(bus,device,function);      // Step 3: Map memory regions
    uint32_t bar0 = device_info->header.bar[0]; // Step 4:
    // uint32_t bar1 = pci_read(bus,device,function,0x14);
    // //uint32_t bar2 = pci_read(bus,device,function,0x18);
    // // printf("bar0 = 0x%x\n",bar0);
    // // printf("bar1 = 0x%x\n",bar1);
    // // printf("bar2 = 0x%x\n",bar2);
    uint16_t STATUS = inportl(bar0>>0x00008);
    //printf("82540EM status register: 0x%x\n",(STATUS));
    ee82540EM_BAR_MMIO = device_info->header.bar[0];
    mem_base = bar0  & ~3;
    //printf("EEPROM exists: %d",detectEEProm());
    detectEEProm();
    card.mac_address = read_eep_mac();
    printf("\nmac->");
    for (size_t i = 0; i < sizeof(mac)/sizeof(mac[0]); i++)
    {
       mac[i] = card.mac_address.bytes[i];
       printf("%d:",mac[i]);
    }
    printf("\n");
    strcpy(card.card_name,device_info->device_name);
    print_mac_address(&card.mac_address);
    setup_link();
    for(int i = 0; i < 0x80; i++)
    {
        writeCommand(0x5200 + i*4, 0);
    }
        
    enableInterrupt();
    isr_register_interrupt_handler(IRQ0_TIMER+device_info->header.interrupt_line,fire);
    e1000_rxinit();
    e1000_txinit();
     
    
    const void* data = "This text";
    size_t len = sizeof(data);
    printf("[Ethernet] sending packet\n");
    
    int ret = sendPacket(data, len);
    // sendArpBroadcast();
    
	DHCP_Send_Discovery(card.mac_address);
    // ARP_SendRequest(IPv4_PackIP(192,168,0,1),mac);
    if(ret != 0)
    {
        return main_exit(ETHERNET_ERROR,ETH_ERR_INITIALIZATION_FAILED,__FUNCTION__,true,eth_error_to_string);
    }
    else
    {
        return main_exit(SUCCESS,SUCCESS,__FUNCTION__,false,eth_error_to_string);
    }
    //writeCommand
    // Step 4: Initialize the Ethernet card, set up MAC address, buffers, etc.
    // This step involves writing to the Ethernet card's registers.
    // Refer to the card's datasheet for specific details.
}

// Define a pointer to represent the base address of physical memory

int detectEEProm()
{
     FUNC_ADDR_NAME(&detectEEProm,0,"");
    uint32_t val = 0;
    writeCommand(REG_EEPROM, 0x1); 
    
    for(int i = 0; i < 1000 && ! eerprom_exists; i++)
    {
            val = readCommand( REG_EEPROM);
            if(val & 0x10)
                    eerprom_exists = true;
            else
                    eerprom_exists = false;
    }
    return eerprom_exists;
}

uint32_t readCommand( uint16_t p_address)
{
    if ( bar_type == 0 )
    {
        return mmio_read32(mem_base+p_address);
    }
    else
    {
        
        outportl(io_base, p_address);
        return inportl(io_base + 4);
    }
}

uint8_t mmio_read8(uint64_t p_address) {
    return *((volatile uint8_t*)(p_address));
}

uint16_t mmio_read16(uint64_t p_address) {
    return *((volatile uint16_t*)(p_address));
}

uint32_t mmio_read32(uint64_t p_address) {
    return *((volatile uint32_t*)(p_address));
}

uint64_t mmio_read64(uint64_t p_address) {
    return *((volatile uint64_t*)(p_address));
}

void mmio_write8(uint64_t p_address, uint8_t p_value) {
    (*((volatile uint8_t*)(p_address))) = p_value;
}

void mmio_write16(uint64_t p_address, uint16_t p_value) {
    (*((volatile uint16_t*)(p_address))) = p_value;
}

void mmio_write32(uint64_t p_address, uint32_t p_value) {
    (*((volatile uint32_t*)(p_address))) = p_value;
}

void mmio_write64(uint64_t p_address, uint64_t p_value) {
    (*((volatile uint64_t*)(p_address))) = p_value;
}

void writeCommand( uint16_t p_address, uint32_t p_value)
{
    if (bar_type == 0 )
    {
        // outportl(io_base, p_address);
        mmio_write32(mem_base+p_address,p_value);
    }
    else
    {
        outportl(io_base, p_address);
        outportl(io_base + 4, p_value);
    }
}

uint32_t eepromRead( uint8_t addr)
{
    FUNC_ADDR_NAME(&eepromRead,1,"i");
	uint16_t data = 0;
	uint32_t tmp = 0;
        if ( eerprom_exists)
        {
            	writeCommand( REG_EEPROM, (1) | ((uint32_t)(addr) << 8) );
        	while( !((tmp = readCommand(REG_EEPROM)) & (1 << 4)) );
        }
        else
        {
            writeCommand( REG_EEPROM, (1) | ((uint32_t)(addr) << 2) );
            while( !((tmp = readCommand(REG_EEPROM)) & (1 << 1)) );
        }
	data = (uint16_t)((tmp >> 16) & 0xFFFF);
	return data;
}
// Function to read the MAC address from the Ethernet card

MacAddress read_mac_address() {
    FUNC_ADDR_NAME(&read_mac_address,0,"");
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
MacAddress read_eep_mac()
{
    FUNC_ADDR_NAME(&read_eep_mac,0,"0");
    MacAddress mac;
    if ( eerprom_exists)
    {
        uint32_t temp;
        temp = eepromRead( 0);
        mac.bytes[0] = temp &0xff;
        mac.bytes[1] = temp >> 8;
        temp = eepromRead( 1);
        mac.bytes[2] = temp &0xff;
        mac.bytes[3] = temp >> 8;
        temp = eepromRead( 2);
        mac.bytes[4] = temp &0xff;
        mac.bytes[5] = temp >> 8;
    }
    else
    {
        uint8_t * mem_base_mac_8 = (uint8_t *) (mem_base+0x5400);
        uint32_t * mem_base_mac_32 = (uint32_t *) (mem_base+0x5400);
        if ( mem_base_mac_32[0] != 0 )
        {
            for(int i = 0; i < 6; i++)
            {
                mac.bytes[i] = mem_base_mac_8[i];
            }
        }
        else {}//return false;
    }
    //return true;
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


void e1000_rxinit()
{
    uint8_t * ptr;
    struct e1000_rx_desc *descs;
 
    // Allocate buffer for receive descriptors. For simplicity, in my case khmalloc returns a virtual address that is identical to it physical mapped address.
    // In your case you should handle virtual and physical addresses as the addresses passed to the NIC should be physical ones
 
    ptr = (uint8_t *)(kmalloc(sizeof(struct e1000_rx_desc)*E1000_NUM_RX_DESC + 16));
 
    descs = (struct e1000_rx_desc *)ptr;
    for(int i = 0; i < E1000_NUM_RX_DESC; i++)
    {
        rx_descs[i] = (struct e1000_rx_desc *)((uint8_t *)descs + i*16);
        rx_descs[i]->addr = (uint64_t)(uint8_t *)(kmalloc(8192 + 16));
        rx_descs[i]->status = 0;
    }
 
    writeCommand(REG_TXDESCLO, (uint32_t)((uint64_t)ptr >> 32) );
    writeCommand(REG_TXDESCHI, (uint32_t)((uint64_t)ptr & 0xFFFFFFFF));
 
    writeCommand(REG_RXDESCLO, (uint64_t)ptr);
    writeCommand(REG_RXDESCHI, 0);
 
    writeCommand(REG_RXDESCLEN, E1000_NUM_RX_DESC * 16);
 
    writeCommand(REG_RXDESCHEAD, 0);
    writeCommand(REG_RXDESCTAIL, E1000_NUM_RX_DESC-1);
    rx_cur = 0;
    writeCommand(REG_RCTRL, RCTL_EN| RCTL_SBP| RCTL_UPE | RCTL_MPE | RCTL_LBM_NONE | RTCL_RDMTS_HALF | RCTL_BAM | RCTL_SECRC  | RCTL_BSIZE_8192);
 
}

void e1000_txinit()
{    
    uint8_t *  ptr;
    struct e1000_tx_desc *descs;
    // Allocate buffer for receive descriptors. For simplicity, in my case khmalloc returns a virtual address that is identical to it physical mapped address.
    // In your case you should handle virtual and physical addresses as the addresses passed to the NIC should be physical ones
    ptr = (uint8_t *)(kmalloc(sizeof(struct e1000_tx_desc)*E1000_NUM_TX_DESC + 15));
 
    descs = (struct e1000_tx_desc *)ptr;
    for(int i = 0; i < E1000_NUM_TX_DESC; i++)
    {
        tx_descs[i] = (struct e1000_tx_desc *)((uint8_t*)descs + i*16);
        tx_descs[i]->addr = 0;
        tx_descs[i]->cmd = 0;
        tx_descs[i]->status = TSTA_DD;
    }
 
    writeCommand(REG_TXDESCHI, (uint32_t)((uint64_t)ptr >> 32) );
    writeCommand(REG_TXDESCLO, (uint32_t)((uint64_t)ptr & 0xFFFFFFFF));
 
 
    //now setup total length of descriptors
    writeCommand(REG_TXDESCLEN, E1000_NUM_TX_DESC * 16);
 
 
    //setup numbers
    writeCommand( REG_TXDESCHEAD, 0);
    writeCommand( REG_TXDESCTAIL, 0);
    tx_cur = 0;
    writeCommand(REG_TCTRL,  TCTL_EN
        | TCTL_PSP
        | (15 << TCTL_CT_SHIFT)
        | (64 << TCTL_COLD_SHIFT)
        | TCTL_RTLC);
 
    // This line of code overrides the one before it but I left both to highlight that the previous one works with e1000 cards, but for the e1000e cards 
    // you should set the TCTRL register as follows. For detailed description of each bit, please refer to the Intel Manual.
    // In the case of I217 and 82577LM packets will not be sent if the TCTRL is not configured using the following bits.
    // writeCommand(REG_TCTRL,  0b0110000000000111111000011111010);
    // writeCommand(REG_TIPG,  0x0060200A);
 
}

int sendPacket(const void * p_data, uint16_t p_len)
{ 
    uint32_t index = readCommand(E1000_REG_TDT);
    uint8_t *netBuffer = (uint8_t *)(uint32_t)tx_descs[index]->addr;
    memcpy(netBuffer, p_data, p_len);
    tx_descs[index]->checksum_off = 0;
    tx_descs[index]->checksum_st = 0;
    tx_descs[index]->cmd = E1000_TX_CMD_EOP;
    tx_descs[index]->length = p_len;
    tx_descs[index]->special = 0;
    tx_descs[index]->status = 0;
    index = (index + 1) % E1000_NUM_TX_DESC;
    writeCommand(E1000_REG_TDT,index);
    // tx_descs[index]->addr = (uint64_t)p_data;
    // tx_descs[index]->length = p_len;
    // tx_descs[index]->cmd = CMD_EOP | CMD_IFCS | CMD_RS;
    // tx_descs[index]->status = 0;
    // tx_descs[index]->special = 0;
    // uint8_t old_cur = tx_cur;
    // tx_cur = (tx_cur + 1) % E1000_NUM_TX_DESC;
    // writeCommand(REG_TXDESCTAIL, tx_cur);   
    // while(!(tx_descs[old_cur]->status & 0xff));    
    return 0;
}

int send_raw(const void * p_data, uint16_t p_len)
{
    //void *amlloc = kmalloc(1000);
    //printf("&ammlloc: 0x%x\n",&amlloc);
    //printf("Payload: %s\n",p_data);
    //printf("Payload size in bytes: %d\n",p_len);
    size_t tx_current = readCommand(REG_TXDESCTAIL) % E1000_NUM_TX_DESC;
    struct e1000_tx_desc *descriptor = tx_descs[tx_current];
    //descriptor->addr = p_data;
    auto* vptr = (void*)tx_descs[tx_current];
    memcpy(vptr, p_data, p_len);
    descriptor->addr = &tx_descs[tx_current+1];
    descriptor->length = p_len;
    descriptor->status = 0;
    descriptor->cmd = CMD_EOP | CMD_IFCS | CMD_RS | CMD_RPS  ;
    printf("descriptor->addr = 0x%x\n", descriptor->addr);

    //printf("descriptor->status: 0x%x\n",descriptor->status);
    //tx_descs[tx_current] = descriptor;
    tx_current = (tx_current + 1) % E1000_NUM_TX_DESC;
    printf("tx_current->addr = 0x%x\n",&tx_descs[tx_current]);
    asm("cli");
    writeCommand(REG_TXDESCTAIL, tx_current);
    //printf("descriptor->status: 0x%x\n",descriptor->status);
    for (;;) {
        if (descriptor->status & 0x01 == TSTA_DD) {
            asm("sti");
            break;
        }
       // m_wait_queue.wait_forever("E1000NetworkAdapter"sv);
    }
    return exit(0);

}

void enableInterrupt()
{
    writeCommand(REG_IMASK ,0x1F6DC);
    writeCommand(REG_IMASK ,0xff & ~4);
    readCommand(0xc0);
 
}


void fire (REGISTERS *r)
{
    if (1==1)
    {        
        /* This might be needed here if your handler doesn't clear interrupts from each device and must be done before EOI if using the PIC.
           Without this, the card will spam interrupts as the int-line will stay high. */
        writeCommand(REG_IMASK, 0x1);
 
        uint32_t status = readCommand(0xc0);
        printf("Status: 0x%x\n",status);
        if(status & 0x04)
        {
            setup_link();
        }
        else if(status & 0x10)
        {
           // good threshold
        }
        else if(status & 0x80)
        {
            handleReceive();
        }
    }
}
 
void handleReceive()
{
    uint16_t old_cur;
    bool got_packet = false;
    
    while((rx_descs[rx_cur]->status & 0x1))
    {
            got_packet = true;
            uint8_t *buf = (uint8_t *)rx_descs[rx_cur]->addr;
            uint16_t len = rx_descs[rx_cur]->length;
 
            // Here you should inject the received packet into your network stack
 
 
            rx_descs[rx_cur]->status = 0;
            old_cur = rx_cur;
            rx_cur = (rx_cur + 1) % E1000_NUM_RX_DESC;
            writeCommand(REG_RXDESCTAIL, old_cur );
    }    
}
// In this code:

// compare_mac_addresses takes the MAC address obtained from QEMU (qemuMac) as a string and the MAC address obtained from your custom OS (osMac) as a MacAddress structure.

// It compares each byte of the MAC addresses and returns 1 if they match and 0 if they don't.

// In the main function, you can call read_mac_address() to get the MAC address from your custom OS, and then compare it to the QEMU-provided MAC address.

// This code will print a message indicating whether the MAC addresses match or not.

void setup_link()
{
    uint32_t flags = readCommand(REG_CTRL);//in32(REG_CTRL);
    writeCommand(REG_CTRL, flags | ECTRL_SLU);
}

int sendArpBroadcast() {
    // Create an ARP broadcast packet
    ArpPacket arpPacket;

    // Fill in the ARP packet fields (you need to set appropriate values based on your requirements)
    arpPacket.hardware_type = 1;  // Assuming Ethernet
    arpPacket.protocol_type = 0;  // Assuming IPv4
    arpPacket.hardware_addr_len = 6;  // Assuming Ethernet MAC address length
    arpPacket.protocol_addr_len = 4;  // Assuming IPv4 address length
    arpPacket.operation = 1;  // ARP Request

    // Set your valid MAC address as the sender's hardware address
    // Replace the following line with your actual MAC address
    const uint8_t myMacAddress[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB};
    memcpy(arpPacket.sender_hardware_addr, mac, 6);

    // Set sender protocol address (you need to set your IP address)
    arpPacket.sender_protocol_addr = 0;  // Set your IP address

    // Set target hardware address (all zeros for broadcast)
    memset(arpPacket.target_hardware_addr, 0, 6);

    // Set target protocol address (all zeros for broadcast)
    arpPacket.target_protocol_addr = 0;

    // Send the ARP packet using the sendPacket function
    if (sendPacket(&arpPacket, sizeof(ArpPacket)) == 0) {
        printf("ARP broadcast packet sent successfully.\n");
        return 0;
    } else {
        printf("Error sending ARP broadcast packet.\n");
        return -1;
    }
}
// uint32_t IPv4_PackIP(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth)
// {
//     return (fourth << 24 | third << 16 | second << 8 | first);
// // }
// enableInterrupt()
// {
//     writeCommand(REG_IMASK ,0x1F6DC);
//     writeCommand(REG_IMASK ,0xff & ~4);
//     readCommand(0xc0);
 
// // }
// void e1000_TX_Init()
// {
//     // Perform transmit initialization (From the Intel Manual:)

//     // Allocate memory for the transmit descriptor list (16-byte aligned)
//     void *txDescListMemory = kmalloc(sizeof(structe 1000_tx_desc ) * E1000_NUM_TX_DESC + 15);

//     // Ensure the descriptor list is aligned on a 16-byte boundary
//     if ((uint32_t)txDescListMemory % 16 != 0)
//         tx_descs = (struct e1000_tx_desc *)((uint32_t)txDescListMemory + 16 - ((uint32_t)txDescListMemory % 16));
//     else
//         tx_descs = (struct e1000_tx_desc *)txDescListMemory;

//     // Zero out the descriptor list
//     memset(tx_descs, 0, sizeof(struct e1000_tx_desc) * TX_DESCRIPTORS);

//     // Initialize the descriptor list with pre-allocated buffers
//     // (We can do this because we know MyOS will never send a packet larger than 1024 bytes)
//     for (int i = 0; i < TX_DESCRIPTORS; ++i)
//     {
//         // TODO: We may need to make sure buffer address is a physical address if malloc changes
//         tx_descs[i].bufferAddress = (uint64_t)malloc(MAX_TX_PACKET);
//     }

//     // Program the Transmit Descriptor Base Address TDBAL register with the address of the region
//     e1000_Write_Register(REG_TDBAL, (uint32_t)tx_descs);

//     // Set the Transmit Descriptor Length (TDLEN) register to the size (in bytes) of the descriptor ring.
//     // This value must be 128 - byte aligned (with 16 descriptors, it will be)
//     uint32_t tdLength = TX_DESCRIPTORS * sizeof(TX_DESC_LEGACY);
    
//     if (tdLength % 128 != 0)
//         terminal_writestring("Error: TDLEN isn't properly calculated!\n");
    
//     e1000_Write_Register(REG_TDLEN, tdLength);

//     // The Transmit Descriptor Head and Tail (TDH/TDT) registers are initialized (by hardware) to 0b
//     // after a power - on or a software initiated Ethernet controller reset. Software should write 0b to both
//     // these registers to ensure this.
//     e1000_Write_Register(REG_TDH, 0);
//     e1000_Write_Register(REG_TDT, 0);

//     /* Initialize the Transmit Control Register (TCTL) for desired operation to include the following:
//         -Set the Enable (TCTL.EN) bit to 1b for normal operation.
//         -Set the Pad Short Packets (TCTL.PSP) bit to 1b.
//         -Configure the Collision Threshold (TCTL.CT) to the desired value. Ethernet standard is 10h.
// This setting only has meaning in half duplex mode.
//         -Configure the Collision Distance (TCTL.COLD) to its expected value. For full duplex
// operation, this value should be set to 40h. For gigabit half duplex, this value should be set to
// 200h. For 10/100 half duplex, this value should be set to 40h. */

//     // TODO: Pay attention to the duplex mode
//     uint32_t tctl = TCTL_TX_ENABLE | TCTL_PAD_SHORT_PACKET | TCTL_DEFAULT_COLLISION_THRESHOLD;
//     tctl |= TCTL_DEFAULT_COLLISION_DISTANCE | TCTL_RETRANSMIT_ON_LATE_COLLISION;
//     e1000_Write_Register(REG_TCTL, tctl);

//     /*  -Program the Transmit IPG (TIPG) register with the following decimal values to get the minimum
// legal Inter Packet Gap:
//             IPGT = 10
//             IPGR1 = 10
//             IPGR2 = 10
//     */
    
//     // (QEMU seems to ignore this register)
//     e1000_Write_Register(REG_TIPG, TIPG_DEFAULTS);
// }
