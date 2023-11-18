#ifndef _ACPI_
#define _ACPI_
#include "../include/types.h"
#include "stdbool.h"
//! RSDP
#define RSDP_SIGNATURE "RSD PTR "
struct XSDP_t {
 char Signature[8];
 uint8_t Checksum;
 char OEMID[6];
 uint8_t Revision;
 uint32_t RsdtAddress;      // deprecated since version 2.0
 
 uint32_t Length;
 uint64_t XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed));
struct XSDP_t* find_rsdp(unsigned char* start, unsigned int length);

//! RSDT
struct ACPISDTHeader {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
};
struct RSDT {
  struct ACPISDTHeader h;
  uint32_t PointerToOtherSDT[]; // Flexible array member, size is determined at runtime
};
bool doChecksum(struct ACPISDTHeader *tableHeader);
//!XsdtPointer
// struct XSDT {
//   struct ACPISDTHeader h;
//   uint64_t PointerToOtherSDT[(h.Length - sizeof(h)) / 8];
// };
// struct RSDT {
//   struct ACPISDTHeader h;
//   uint32_t PointerToOtherSDT[(h.Length - sizeof(h)) / 4];
// };
unsigned int *acpiCheckRSDPtr(unsigned int *ptr);

unsigned int *acpiGetRSDPtr(void);

int acpiCheckHeader(unsigned int *ptr, char *sig);

int acpiEnable(void);

int initAcpi(void);

void acpiPowerOff(void);
void initAcpi2(void);
struct FACP* findFACP(struct ACPISDTHeader* rsdt);
#endif