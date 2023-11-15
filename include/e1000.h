#ifndef e82540EM_ETHERNET_H
#define e82540EM_ETHERNET_H

#include <stdint.h>
#include "../include/pci.h"
#include "../include/isr.h"
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
typedef struct
{
    MacAddress mac_address;
    char card_name[200];
}NicCardInfo;
typedef struct {
    uint16_t hardware_type;         // Hardware type (e.g., Ethernet is 1)
    uint16_t protocol_type;         // Protocol type (e.g., IPv4 is 0x0800)
    uint8_t  hardware_addr_len;     // Length of hardware address in bytes (e.g., 6 for Ethernet MAC address)
    uint8_t  protocol_addr_len;     // Length of protocol address in bytes (e.g., 4 for IPv4 address)
    uint16_t operation;             // ARP operation (request or reply)
    uint8_t  sender_hardware_addr[6];  // Sender's hardware address (MAC address)
    uint32_t sender_protocol_addr;      // Sender's protocol address (IP address)
    uint8_t  target_hardware_addr[6];  // Target's hardware address (MAC address)
    uint32_t target_protocol_addr;      // Target's protocol address (IP address)
} ArpPacket;
// Function to map memory regions for the Intel e82540EM Ethernet card
void map_e82540EM_memory_regions(uint8_t bus, uint8_t device, uint8_t function);

// Function to initialize the Intel e82540EM Ethernet card
int init_e82540EM_ethernet_card(uint8_t bus, uint8_t device, uint8_t function);

// Function to read the MAC address from the Ethernet card
MacAddress read_mac_address();

int compare_mac_addresses(const char* qemuMac, const MacAddress* osMac);



MacAddress read_eep_mac();
#define ETH_ERR_NO_DRIVER 0x0001
#define ETH_ERR_INVALID_MAC 0x0002
#define ETH_ERR_BUFFER_OVERFLOW 0x0003
#define ETH_ERR_PACKET_TOO_LARGE 0x0004
#define ETH_ERR_PACKET_TOO_SMALL 0x0005
#define ETH_ERR_LINK_DOWN 0x0006
#define ETH_ERR_INVALID_IP_HEADER 0x0007
#define ETH_ERR_INVALID_ARP_PACKET 0x0008
#define ETH_ERR_INVALID_UDP_PACKET 0x0009
#define ETH_ERR_INVALID_TCP_PACKET 0x000A
#define ETH_ERR_IP_CONFLICT 0x000B
#define ETH_ERR_DHCP_FAILURE 0x000C
#define ETH_ERR_TCP_TIMEOUT 0x000D
#define ETH_ERR_UDP_PORT_IN_USE 0x000E
#define ETH_ERR_INVALID_PACKET_TYPE 0x000F
#define ETH_ERR_BUFFER_ALLOCATION_FAILED 0x0010
#define ETH_ERR_INVALID_DESTINATION_MAC 0x0011
#define ETH_ERR_INVALID_PACKET_LENGTH 0x0012
#define ETH_ERR_RX_FIFO_OVERFLOW 0x0013
#define ETH_ERR_TX_FIFO_OVERFLOW 0x0014
#define ETH_ERR_PHY_FAILURE 0x0015
#define ETH_ERR_INVALID_VLAN_TAG 0x0016
#define ETH_ERR_MULTICAST_FILTER_FULL 0x0017
#define ETH_ERR_INVALID_IPV6_PACKET 0x0018
#define ETH_ERR_PACKET_DROPPED 0x0019
#define ETH_ERR_INVALID_MTU 0x001A
#define ETH_ERR_TRANSMIT_ERROR 0x001B
#define ETH_ERR_INVALID_PACKET_HEADER 0x001C
#define ETH_ERR_PROMISCUOUS_MODE 0x001D
#define ETH_ERR_INVALID_LINK_SPEED 0x001E
#define ETH_ERR_INITIALIZATION_FAILED 0x001F
const char* eth_error_to_string(uint16_t error_code);

//!! THIS CODE I COPIED FROM wiki.osdev.org. I dont know what license it was under, if the owner would kindly tell me, I will happily include him in the credits section
//!! and include a header to file this file (ethernet.h) and to the source file (ethernet.c). FOR the moment the code will be under the GPLv3
#define REG_CTRL        0x0000
#define REG_STATUS      0x0008
#define REG_EEPROM      0x0014
#define REG_CTRL_EXT    0x0018
#define REG_IMASK       0x00D0
#define REG_RCTRL       0x0100
#define REG_RXDESCLO    0x2800
#define REG_RXDESCHI    0x2804
#define REG_RXDESCLEN   0x2808
#define REG_RXDESCHEAD  0x2810
#define REG_RXDESCTAIL  0x2818
 
#define REG_TCTRL       0x0400
#define REG_TXDESCLO    0x3800
#define REG_TXDESCHI    0x3804
#define REG_TXDESCLEN   0x3808
#define REG_TXDESCHEAD  0x3810
#define REG_TXDESCTAIL  0x03818
 
 
#define REG_RDTR         0x2820 // RX Delay Timer Register
#define REG_RXDCTL       0x2828 // RX Descriptor Control
#define REG_RADV         0x282C // RX Int. Absolute Delay Timer
#define REG_RSRPD        0x2C00 // RX Small Packet Detect Interrupt
 
 
 
#define REG_TIPG         0x0410      // Transmit Inter Packet Gap
#define ECTRL_SLU        0x40        //set link up
 
 
#define RCTL_EN                         (1 << 1)    // Receiver Enable
#define RCTL_SBP                        (1 << 2)    // Store Bad Packets
#define RCTL_UPE                        (1 << 3)    // Unicast Promiscuous Enabled
#define RCTL_MPE                        (1 << 4)    // Multicast Promiscuous Enabled
#define RCTL_LPE                        (1 << 5)    // Long Packet Reception Enable
#define RCTL_LBM_NONE                   (0 << 6)    // No Loopback
#define RCTL_LBM_PHY                    (3 << 6)    // PHY or external SerDesc loopback
#define RTCL_RDMTS_HALF                 (0 << 8)    // Free Buffer Threshold is 1/2 of RDLEN
#define RTCL_RDMTS_QUARTER              (1 << 8)    // Free Buffer Threshold is 1/4 of RDLEN
#define RTCL_RDMTS_EIGHTH               (2 << 8)    // Free Buffer Threshold is 1/8 of RDLEN
#define RCTL_MO_36                      (0 << 12)   // Multicast Offset - bits 47:36
#define RCTL_MO_35                      (1 << 12)   // Multicast Offset - bits 46:35
#define RCTL_MO_34                      (2 << 12)   // Multicast Offset - bits 45:34
#define RCTL_MO_32                      (3 << 12)   // Multicast Offset - bits 43:32
#define RCTL_BAM                        (1 << 15)   // Broadcast Accept Mode
#define RCTL_VFE                        (1 << 18)   // VLAN Filter Enable
#define RCTL_CFIEN                      (1 << 19)   // Canonical Form Indicator Enable
#define RCTL_CFI                        (1 << 20)   // Canonical Form Indicator Bit Value
#define RCTL_DPF                        (1 << 22)   // Discard Pause Frames
#define RCTL_PMCF                       (1 << 23)   // Pass MAC Control Frames
#define RCTL_SECRC                      (1 << 26)   // Strip Ethernet CRC
 
// Buffer Sizes
#define RCTL_BSIZE_256                  (3 << 16)
#define RCTL_BSIZE_512                  (2 << 16)
#define RCTL_BSIZE_1024                 (1 << 16)
#define RCTL_BSIZE_2048                 (0 << 16)
#define RCTL_BSIZE_4096                 ((3 << 16) | (1 << 25))
#define RCTL_BSIZE_8192                 ((2 << 16) | (1 << 25))
#define RCTL_BSIZE_16384                ((1 << 16) | (1 << 25))
 
 
// Transmit Command
 
#define CMD_EOP                         (1 << 0)    // End of Packet
#define CMD_IFCS                        (1 << 1)    // Insert FCS
#define CMD_IC                          (1 << 2)    // Insert Checksum
#define CMD_RS                          (1 << 3)    // Report Status
#define CMD_RPS                         (1 << 4)    // Report Packet Sent
#define CMD_VLE                         (1 << 6)    // VLAN Packet Enable
#define CMD_IDE                         (1 << 7)    // Interrupt Delay Enable
 
 
// TCTL Register
 
#define TCTL_EN                         (1 << 1)    // Transmit Enable
#define TCTL_PSP                        (1 << 3)    // Pad Short Packets
#define TCTL_CT_SHIFT                   4           // Collision Threshold
#define TCTL_COLD_SHIFT                 12          // Collision Distance
#define TCTL_SWXOFF                     (1 << 22)   // Software XOFF Transmission
#define TCTL_RTLC                       (1 << 24)   // Re-transmit on Late Collision
 
#define TSTA_DD                         (1 << 0)    // Descriptor Done
#define TSTA_EC                         (1 << 1)    // Excess Collisions
#define TSTA_LC                         (1 << 2)    // Late Collision
#define LSTA_TU                         (1 << 3)    // Transmit Underrun

#define E1000_NUM_RX_DESC 32
#define E1000_NUM_TX_DESC 8
 
uint32_t readCommand( uint16_t p_address);
uint8_t mmio_read8(uint64_t p_address);
uint16_t mmio_read16(uint64_t p_address);
uint32_t mmio_read32(uint64_t p_address);
uint64_t mmio_read64(uint64_t p_address);
void mmio_write8(uint64_t p_address, uint8_t p_value);
void mmio_write16(uint64_t p_address, uint16_t p_value);
void mmio_write32(uint64_t p_address, uint32_t p_value);
void mmio_write64(uint64_t p_address, uint64_t p_value);
struct e1000_rx_desc {
        volatile uint64_t addr;
        volatile uint16_t length;
        volatile uint16_t checksum;
        volatile uint8_t status;
        volatile uint8_t errors;
        volatile uint16_t special;
} __attribute__((packed));
 
typedef struct e1000_tx_desc
{
    uint64_t addr;              /**< Address of the transmit buffer (64-bit). */
    uint32_t buffer_h;          /**< High 32-bits of the transmit buffer (unused). */
    uint16_t length;            /**< Size of the transmit buffer contents. */
    uint8_t checksum_off;       /**< Checksum Offset. */
    uint8_t cmd;                /**< Command field. */
    uint8_t status;             /**< Status field. */
    uint8_t checksum_st;        /**< Checksum Start. */
    uint8_t cso;                /**< Checksum Offset. */
    uint8_t css;                /**< Checksum Start. */
    uint16_t special;           /**< Optional special bits. */
};

// typedef struct e1000
// {
//     int irq;			  /**< Interrupt Request Vector. */
//     int irq_hook;                 /**< Interrupt Request Vector Hook. */
//     uint8_t *regs;		  	  /**< Memory mapped hardware registers. */
//     uint8_t *flash;		  /**< Optional flash memory. */
//     uint32_t flash_base_addr;	  /**< Flash base address. */
//     uint16_t (*eeprom_read)(struct e1000 *, int reg);
// 				  /**< Function to read the EEPROM. */
//     int eeprom_done_bit;	  /**< Offset of the EERD.DONE bit. */    
//     int eeprom_addr_off;	  /**< Offset of the EERD.ADDR field. */

//     struct e1000_rx_desc *rx_desc;	  /**< Receive Descriptor table. */
//     int rx_desc_count;		  /**< Number of Receive Descriptors. */
//     char *rx_buffer;		  /**< Receive buffer returned by malloc(). */
//     int rx_buffer_size;		  /**< Size of the receive buffer. */

//     struct e1000_tx_desc *tx_desc;	  /**< Transmit Descriptor table. */
//     int tx_desc_count;		  /**< Number of Transmit Descriptors. */
//     char *tx_buffer;		  /**< Transmit buffer returned by malloc(). */
//     int tx_buffer_size;		  /**< Size of the transmit buffer. */
// } e1000_t;


void e1000_rxinit();
void e1000_txinit();
int sendPacket(const void *p_data, uint16_t p_len);
void writeCommand( uint16_t p_address, uint32_t p_value);
void fire (REGISTERS *r);
int send_raw(const void * p_data, uint16_t p_len);
int sendArpBroadcast();
// uint32_t IPv4_PackIP(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);
#define MAX_PACKET_SEND_CHECKS 100000000

//!Copied form the minex source code
#define NDEV_NAME_MAX	16	/* max network driver name length (incl nul) */
#define NDEV_HWADDR_MAX	6	/* max network hardware address length */
#define NDEV_IOV_MAX	8	/* max number of elements in I/O vector */
typedef int endpoint_t;			/* process identifier */
typedef long unsigned int vir_bytes;	/* virtual addresses/lengths in bytes */
typedef int32_t cp_grant_id_t;
typedef struct {
  cp_grant_id_t iov_grant;	/* grant ID of an I/O buffer */
  vir_bytes iov_size;		/* sizeof an I/O buffer */
} iovec_s_t;

// struct netdriver_data {
// 	endpoint_t endpt;
// 	uint32_t id;
// 	size_t size;
// 	unsigned int count;
// 	iovec_s_t iovec[NDEV_IOV_MAX];
// };

/**
 * @name Receive Status Field Bits.
 * @{
 */

/** Passed In-exact Filter. */
#define E1000_RX_STATUS_PIF	(1 << 7) 
 
/** End of Packet. */
#define E1000_RX_STATUS_EOP	(1 << 1)

/** Descriptor Done. */
#define E1000_RX_STATUS_DONE	(1 << 0)

/**
 * @}
 */

/**
 * @name Receive Errors Field Bits.
 * @{
 */

/** RX Data Error. */
#define E1000_RX_ERROR_RXE	(1 << 7)

/** Carrier Extension Error. */
#define E1000_RX_ERROR_CXE	(1 << 4)

/** Sequence/Framing Error. */
#define E1000_RX_ERROR_SEQ	(1 << 2)

/** CRC/Alignment Error. */
#define E1000_RX_ERROR_CE	(1 << 0)

/**
 * @}
 */

/**
 * @name Transmit Command Field Bits.
 * @{
 */

/** End of Packet. */
#define E1000_TX_CMD_EOP	(1 << 0)

/** Insert FCS/CRC. */
#define E1000_TX_CMD_FCS	(1 << 1)

/** Report Status. */
#define E1000_TX_CMD_RS		(1 << 3)

/**
 * @}
 */


/** Number of receive descriptors per card. */
#define E1000_RXDESC_NR 256

/** Number of transmit descriptors per card. */
#define E1000_TXDESC_NR 256

/** Size of each I/O buffer per descriptor. */
#define E1000_IOBUF_SIZE 2048

/** Debug verbosity. */
#define E1000_VERBOSE 0

/** MAC address override variable. */
#define E1000_ENVVAR "E1000ETH"

#define E1000_REG_CTRL		0x00000

/** Device Status. */
#define E1000_REG_STATUS	0x00008

/** EEPROM Read. */
#define E1000_REG_EERD		0x00014

/** Flow Control Address Low. */
#define E1000_REG_FCAL		0x00028

/** Flow Control Address High. */
#define E1000_REG_FCAH		0x0002c

/** Flow Control Type. */
#define E1000_REG_FCT		0x00030

/** Interrupt Cause Read. */
#define E1000_REG_ICR		0x000c0

/** Interrupt Mask Set/Read Register. */
#define E1000_REG_IMS		0x000d0

/** Receive Control Register. */
#define E1000_REG_RCTL		0x00100

/** Transmit Control Register. */
#define E1000_REG_TCTL		0x00400

/** Flow Control Transmit Timer Value. */
#define E1000_REG_FCTTV		0x00170

/** Receive Descriptor Base Address Low. */
#define E1000_REG_RDBAL		0x02800

/** Receive Descriptor Base Address High. */
#define E1000_REG_RDBAH		0x02804

/** Receive Descriptor Length. */
#define E1000_REG_RDLEN		0x02808

/** Receive Descriptor Head. */
#define E1000_REG_RDH		0x02810

/** Receive Descriptor Tail. */
#define E1000_REG_RDT		0x02818

/** Transmit Descriptor Base Address Low. */
#define E1000_REG_TDBAL		0x03800

/** Transmit Descriptor Base Address High. */
#define E1000_REG_TDBAH		0x03804

/** Transmit Descriptor Length. */
#define E1000_REG_TDLEN		0x03808

/** Transmit Descriptor Head. */
#define E1000_REG_TDH		0x03810

/** Transmit Descriptor Tail. */
#define E1000_REG_TDT		0x03818

/** CRC Error Count. */
#define E1000_REG_CRCERRS	0x04000

/** RX Error Count. */
#define E1000_REG_RXERRC	0x0400c

/** Missed Packets Count. */
#define E1000_REG_MPC		0x04010

/** Collision Count. */
#define E1000_REG_COLC		0x04028

/** Total Packets Received. */
#define E1000_REG_TPR		0x040D0

/** Total Packets Transmitted. */
#define E1000_REG_TPT		0x040D4

/** Receive Address Low. */
#define E1000_REG_RAL		0x05400

/** Receive Address High. */
#define E1000_REG_RAH		0x05404

/** Multicast Table Array. */
#define E1000_REG_MTA		0x05200

typedef struct e1000
{
    int irq;			  /**< Interrupt Request Vector. */
    int irq_hook;                 /**< Interrupt Request Vector Hook. */
    uint8_t *regs;		  	  /**< Memory mapped hardware registers. */
    uint8_t *flash;		  /**< Optional flash memory. */
    uint32_t flash_base_addr;	  /**< Flash base address. */
    uint16_t (*eeprom_read)(struct e1000 *, int reg);
				  /**< Function to read the EEPROM. */
    int eeprom_done_bit;	  /**< Offset of the EERD.DONE bit. */    
    int eeprom_addr_off;	  /**< Offset of the EERD.ADDR field. */

    struct e1000_rx_desc *rx_desc;	  /**< Receive Descriptor table. */
    int rx_desc_count;		  /**< Number of Receive Descriptors. */
    char *rx_buffer;		  /**< Receive buffer returned by malloc(). */
    int rx_buffer_size;		  /**< Size of the receive buffer. */

    struct e1000_tx_desc *tx_desc;	  /**< Transmit Descriptor table. */
    int tx_desc_count;		  /**< Number of Transmit Descriptors. */
    char *tx_buffer;		  /**< Transmit buffer returned by malloc(). */
    int tx_buffer_size;		  /**< Size of the transmit buffer. */
} e1000_t;
void init_E1000();

#define IOADDR  0x00
#define IODATA  0x04

// Register definitions (despite what the offsets might suggest, all registers are 32-bit unless otherwise noted)
        // register     offset
#define REG_CTRL        0x00
#define REG_STATUS      0x08
#define REG_EECD        0x10
#define REG_EERD        0x14
#define REG_FLA         0x1C
#define REG_CTRL_EXT    0x18
#define REG_MDIC        0x20
#define REG_FCAL        0x28
#define REG_FCAH        0x2C
#define REG_FCT         0x30
#define REG_VET         0x38
// ...
#define REG_ICR         0xC0
#define REG_IMS         0xD0
#define REG_IMC         0xD8
#define REG_RCTL        0x100
// ...
#define REG_FCTTV       0x170
// ...
#define REG_TCTL        0x400
#define REG_TIPG        0x410
// ...
#define REG_RDBAL       0x2800
#define REG_RDBAH       0x2804
#define REG_RDLEN       0x2808
#define REG_RDH         0x2810
#define REG_RDT         0x2818
// ...
#define REG_TDBAL       0x3800
#define REG_TDBAH       0x3804
#define REG_TDLEN       0x3808
#define REG_TDH         0x3810
#define REG_TDT         0x3818
// ...
#define REG_MTA0        0x5200  /* 128 registers */
// ...
#define REG_MAC_LOW     0x5400  /* AKA Receive Address Low(n), RAL(8*n)  */
#define REG_MAC_HIGH    0x5404  /* AKA Receive Address High(n), RAH(8*n) */


// defines for PCI BAR bits
#define BAR_MMIO_OR_IO_BIT   1
#define BAR_USING_IO         1
#define BAR_ADDRESS_TYPE     6
#define BAR_64BIT            4
#define BAR_32BIT            0
#define BAR_ADDRESS          (~0x0F)


// defines for CTRL register
#define CTRL_FULL_DUPLEX            0x01
#define CTRL_HALF_DUPLEX            0x00
#define CTRL_AUTO_SPEED_DETECT      0x10
#define CTRL_SET_LINK_UP            0x20
#define CTRL_INVERT_LOSS_OF_SIGNAL  0x40
#define CTRL_SPEED_10_MBS           0x00
#define CTRL_SPEED_100_MBS          0x100
#define CTRL_SPEED_200_MBS          0x200
#define CTRL_FORCE_SPEED            0x800
#define CTRL_FORCE_DUPLEX           0x1000
#define CTRL_PHY_RESET              0x80000000

// defines for IMS register
#define IMS_TXQE                0x02
#define IMS_LSC                 0x04
#define IMS_RXSEQ               0x08
#define IMS_RXDMT0              0x10
#define IMS_RXO                 0x40
#define IMS_RXT0                0x80

// defines for RCTL reg
#define RCTL_RX_ENABLE                      2
#define RCTL_BROADCAST_ACCEPT               0x8000
#define RCTL_BUFFER_SIZE_1024               0x10000

// defines for TCTL register
#define TCTL_TX_ENABLE                      2
#define TCTL_PAD_SHORT_PACKET               8
#define TCTL_DEFAULT_COLLISION_THRESHOLD    (0x10 << 4)   /* CT = 10h, CT is in bits 11:4 */
#define TCTL_DEFAULT_COLLISION_DISTANCE     (0x40 << 12)
#define TCTL_RETRANSMIT_ON_LATE_COLLISION   0x1000000

// defines for TIPG register
#define TIPG_DEFAULTS   (10 | (10 << 10) | (10 << 20))

// Layout of transmit descriptor (TDESC) 
typedef struct TX_DESC_LEGACY
{
    uint64_t bufferAddress;
    uint16_t length;
    uint8_t  checksumOffset;
    uint8_t  command;
    uint8_t  status;
    uint8_t  checksumStart;
    uint16_t  special;
} TX_DESC_LEGACY;

// defines for command bits of tdesc
#define TDESC_CMD_EOP               1
#define TDESC_CMD_INSERT_FCS        2
#define TDESC_CMD_INSERT_CHECKSUM   4
#define TDESC_CMD_REPORT_STATUS     8
#define TDESC_CMD_REPORT_PACKET_SENT    16  /* used by 82544GC/EI only */

// defines for status bits
#define TDESC_STATUS_DESCRIPTOR_DONE    1

#define TX_DESCRIPTORS  768

#define MAX_TX_PACKET   1024    /* We only use TFTP and DHCP, so a packet will never exceed this size */

typedef struct RX_DESCRIPTOR
{
    uint64_t bufferAddress;
    uint16_t length;
    uint16_t packetChecksum;
    uint8_t  status;
    uint8_t  errors;
    uint16_t special;
} RX_DESCRIPTOR;

// defines for receive status bits
#define RDESC_STATUS_EOP                2
#define RDESC_STATUS_DESCRIPTOR_DONE    1

#define RX_DESCRIPTORS  384

void e1000_Net_Init(uint8_t bus, uint8_t slot, uint8_t function);

void e1000_InterruptHandler();

uint32_t e1000_Read_Register(uint32_t reg);

void e1000_SendPacket(Ethernet_Header *packet, uint16_t dataSize);

bool e1000_SharedInterruptHandler(void);

void e1000_ReceivePacket();

void e1000_RX_Init();

void e1000_TX_Init();

void e1000_Write_Register(uint32_t regOffset, uint32_t value);


#endif /* e82540EM_ETHERNET_H */

