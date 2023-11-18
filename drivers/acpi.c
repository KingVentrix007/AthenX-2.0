#include "../include/acpi.h"
#include "../include/string.h"

struct XSDP_t* find_rsdp(unsigned char* start, unsigned int length) {
    for (unsigned int i = 0; i < length - sizeof(struct XSDP_t); i++) {
        if (memcmp(start + i, RSDP_SIGNATURE, sizeof(RSDP_SIGNATURE) - 1) == 0) {
            struct XSDP_t* rsdp = (struct XSDP_t*)(start + i);
            // Checksum verification and other checks can be added here
            return rsdp;
        }
    }
    return NULL;
}

bool doChecksum(struct ACPISDTHeader *tableHeader)
{
    unsigned char sum = 0;
 
    for (int i = 0; i < tableHeader->Length; i++)
    {
        sum += ((char *) tableHeader)[i];
    }
 
    return sum == 0;
}


// void *findFACP(void *RootSDT)
// {
//     struct RSDT *rsdt = (struct RSDT *) RootSDT;
//     int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;
 
//     for (int i = 0; i < entries; i++)
//     {
//         struct ACPISDTHeader *h = (struct ACPISDTHeader *) rsdt->PointerToOtherSDT[i];
//         if (!strncmp(h->Signature, "FACP", 4))
//             return (void *) h;
//     }
 
//     // No FACP found
//     return NULL;
// }


#include <stddef.h>
#include <printf.h>
#include <string.h>
#include <io_ports.h>


dword *SMI_CMD;
byte ACPI_ENABLE;
byte ACPI_DISABLE;
dword *PM1a_CNT;
dword *PM1b_CNT;
word SLP_TYPa;
word SLP_TYPb;
word SLP_EN;
word SCI_EN;
byte PM1_CNT_LEN;



struct RSDPtr
{
   byte Signature[8];
   byte CheckSum;
   byte OemID[6];
   byte Revision;
   dword *RsdtAddress;
};



struct FACP
{
   byte Signature[4];
   dword Length;
   byte unneded1[40 - 8];
   dword *DSDT;
   byte unneded2[48 - 44];
   dword *SMI_CMD;
   byte ACPI_ENABLE;
   byte ACPI_DISABLE;
   byte unneded3[64 - 54];
   dword *PM1a_CNT_BLK;
   dword *PM1b_CNT_BLK;
   byte unneded4[89 - 72];
   byte PM1_CNT_LEN;
};



// check if the given address has a valid header
unsigned int *acpiCheckRSDPtr(unsigned int *ptr)
{
   char *sig = "RSD PTR ";
   struct RSDPtr *rsdp = (struct RSDPtr *) ptr;
   byte *bptr;
   byte check = 0;
   int i;

   if (memcmp(sig, rsdp, 8) == 0)
   {
      // check checksum rsdpd
      bptr = (byte *) ptr;
      for (i=0; i<sizeof(struct RSDPtr); i++)
      {
         check += *bptr;
         bptr++;
      }

      // found valid rsdpd   
      if (check == 0) {
         /*
          if (desc->Revision == 0)
            printf("acpi 1");
         else
            printf("acpi 2");
         */
         return (unsigned int *) rsdp->RsdtAddress;
      }
   }

   return NULL;
}



// finds the acpi header and returns the address of the rsdt
unsigned int *acpiGetRSDPtr(void)
{
   unsigned int *addr;
   unsigned int *rsdp;

   // search below the 1mb mark for RSDP signature
   for (addr = (unsigned int *) 0x000E0000; (int) addr<0x00100000; addr += 0x10/sizeof(addr))
   {
      rsdp = acpiCheckRSDPtr(addr);
      if (rsdp != NULL)
         return rsdp;
   }


   // at address 0x40:0x0E is the RM segment of the ebda
   int ebda = *((short *) 0x40E);   // get pointer
   ebda = ebda*0x10 &0x000FFFFF;   // transform segment into linear address

   // search Extended BIOS Data Area for the Root System Description Pointer signature
   for (addr = (unsigned int *) ebda; (int) addr<ebda+1024; addr+= 0x10/sizeof(addr))
   {
      rsdp = acpiCheckRSDPtr(addr);
      if (rsdp != NULL)
         return rsdp;
   }

   return NULL;
}



// checks for a given header and validates checksum
int acpiCheckHeader(unsigned int *ptr, char *sig)
{
   if (memcmp(ptr, sig, 4) == 0)
   {
      printf("cecking ptr(sig(%s))\n",sig);
      char *checkPtr = (char *) ptr;
      int len = *(ptr + 1);
      char check = 0;
      while (0<len--)
      {
         check += *checkPtr;
         checkPtr++;
      }
      if (check == 0)
         return 0;
   }
   return -1;
}



int acpiEnable(void)
{
   // check if acpi is enabled
   if ( (inports((unsigned int) PM1a_CNT) &SCI_EN) == 0 )
   {
      // check if acpi can be enabled
      if (SMI_CMD != 0 && ACPI_ENABLE != 0)
      {
         outportb((unsigned int) SMI_CMD, ACPI_ENABLE); // send acpi enable command
         // give 3 seconds time to enable acpi
         int i;
         for (i=0; i<300; i++ )
         {
            if ( (inports((unsigned int) PM1a_CNT) &SCI_EN) == 1 )
               break;
            sleep(10);
         }
         if (PM1b_CNT != 0)
            for (; i<300; i++ )
            {
               if ( (inports((unsigned int) PM1b_CNT) &SCI_EN) == 1 )
                  break;
               sleep(10);
            }
         if (i<300) {
            printf("enabled acpi.\n");
            return 0;
         } else {
            printf("couldn't enable acpi.\n");
            return -1;
         }
      } else {
         printf("no known way to enable acpi.\n");
         return -1;
      }
   } else {
      //printf("acpi was already enabled.\n");
      return 0;
   }
}



//
// bytecode of the \_S5 object
// -----------------------------------------
//        | (optional) |    |    |    |   
// NameOP | \          | _  | S  | 5  | _
// 08     | 5A         | 5F | 53 | 35 | 5F
//
// -----------------------------------------------------------------------------------------------------------
//           |           |              | ( SLP_TYPa   ) | ( SLP_TYPb   ) | ( Reserved   ) | (Reserved    )
// PackageOP | PkgLength | NumElements  | byteprefix Num | byteprefix Num | byteprefix Num | byteprefix Num
// 12        | 0A        | 04           | 0A         05  | 0A          05 | 0A         05  | 0A         05
//
//----this-structure-was-also-seen----------------------
// PackageOP | PkgLength | NumElements |
// 12        | 06        | 04          | 00 00 00 00
//
// (Pkglength bit 6-7 encode additional PkgLength bytes [shouldn't be the case here])
//
int initAcpi(void)
{
   // initAcpi2();
   return 1;
   // unsigned int *ptr = acpiGetRSDPtr();
   //  printf("init acpi\n");
   // // check if address is correct  ( if acpi is available on this pc )
   // struct RSDPtr rsdp;
   // uint32_t rsdt  = rsdp.RsdtAddress;
   
   // if (ptr != NULL && acpiCheckHeader(ptr, "RSDT") == 0)
   // {
   //    // the RSDT contains an unknown number of pointers to acpi tables
   //    int entrys = *(ptr + 1);
   //    entrys = (entrys-36) /4;
   //    ptr += 36/4;   // skip header information
   //    printf("looking for FACP(%d)\n",entrys);
   //    while (0<entrys--)
   //    {
   //       printf("here");
   //       // check if the desired table is reached
        
   //       if (acpiCheckHeader((unsigned int *) *ptr, "FACP") == 0)
   //       {
   //          entrys = -2;
   //          struct FACP *facp = (struct FACP *) *ptr;
   //          if (acpiCheckHeader((unsigned int *) facp->DSDT, "DSDT") == 0)
   //          {
   //             // search the \_S5 package in the DSDT
   //             char *S5Addr = (char *) facp->DSDT +36; // skip header
   //             int dsdtLength = *(facp->DSDT+1) -36;
   //             while (0 < dsdtLength--)
   //             {
   //                if ( memcmp(S5Addr, "_S5_", 4) == 0)
   //                   break;
   //                S5Addr++;
   //             }
   //             // check if \_S5 was found
   //             if (dsdtLength > 0)
   //             {
   //                // check for valid AML structure
   //                if ( ( *(S5Addr-1) == 0x08 || ( *(S5Addr-2) == 0x08 && *(S5Addr-1) == '\\') ) && *(S5Addr+4) == 0x12 )
   //                {
   //                   S5Addr += 5;
   //                   S5Addr += ((*S5Addr &0xC0)>>6) +2;   // calculate PkgLength size

   //                   if (*S5Addr == 0x0A)
   //                      S5Addr++;   // skip byteprefix
   //                   SLP_TYPa = *(S5Addr)<<10;
   //                   S5Addr++;

   //                   if (*S5Addr == 0x0A)
   //                      S5Addr++;   // skip byteprefix
   //                   SLP_TYPb = *(S5Addr)<<10;

   //                   SMI_CMD = facp->SMI_CMD;

   //                   ACPI_ENABLE = facp->ACPI_ENABLE;
   //                   ACPI_DISABLE = facp->ACPI_DISABLE;

   //                   PM1a_CNT = facp->PM1a_CNT_BLK;
   //                   PM1b_CNT = facp->PM1b_CNT_BLK;
                     
   //                   PM1_CNT_LEN = facp->PM1_CNT_LEN;

   //                   SLP_EN = 1<<13;
   //                   SCI_EN = 1;

   //                   return 0;
   //                } else {
   //                   printf("\\_S5 parse error.\n");
   //                }
   //             } else {
   //                printf("\\_S5 not present.\n");
   //             }
   //          } else {
   //             printf("DSDT invalid.\n");
   //          }
   //       }
   //       else
   //       {
   //          printf("no RSDT\n");
   //       }
   //       ptr++;
   //    }
   //    printf("no valid FA present.\n");
   // } else {
   //    printf("no acpi.\n");
   // }

   // return -1;
}

void initAcpi2(void)
{
    unsigned int *ptr = acpiGetRSDPtr();
    printf("init acpi\n");

    // Check if address is correct (if ACPI is available on this PC)
    struct RSDPtr *rsdp = (struct RSDPtr *)ptr;

    // Access the RSDT
    struct ACPISDTHeader* RSDT = (struct ACPISDTHeader*)rsdp->RsdtAddress;

    // Find the FACP within the RSDT
    struct FACP* facp = findFACP(RSDT);

    if (facp != NULL) {
        // Extract information from the FACP
        // For example, you might want to check the DSDT pointer and PM1a_CNT_BLK address
        dword *dsdtPointer = facp->DSDT;
        dword *pm1aCntBlk = facp->PM1a_CNT_BLK;

        // ... (do something with the information)
    } else {
        // FACP not found, handle the case accordingly
        // ...
    }
}
// void *findFACP(void *RootSDT)
// {
//     struct RSDT *rsdt = (struct RSDT *) RootSDT;
//     int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;
 
//     for (int i = 0; i < entries; i++)
//     {
//         struct ACPISDTHeader *h = (struct ACPISDTHeader *) rsdt->PointerToOtherSDT[i];
//         if (!strncmp(h->Signature, "FACP", 4))
//             return (void *) h;
//     }
 
//     // No FACP found
//     return NULL;
// }

void acpiPowerOff(void)
{
   // SCI_EN is set to 1 if acpi shutdown is possible
   if (SCI_EN == 0)
      return;

   acpiEnable();

   // send the shutdown command
   outports((unsigned int) PM1a_CNT, SLP_TYPa | SLP_EN );
   if ( PM1b_CNT != 0 )
      outports((unsigned int) PM1b_CNT, SLP_TYPb | SLP_EN );

   printf("acpi poweroff failed.\n");
}

struct FACP* findFACP(struct ACPISDTHeader* rsdt)
{
    // Check if the provided pointer is not NULL
    if (rsdt == NULL) {
        return NULL;
    }

    // Get the number of entries in the RSDT
    int entryCount = (rsdt->Length - sizeof(struct ACPISDTHeader)) / sizeof(uint32_t);
    uint32_t* entries = (uint32_t*)(rsdt + 1);  // Start of the table entries

    // Iterate through the entries in the RSDT
    for (int i = 0; i < entryCount; ++i) {
        // Access each table using the entry addresses
        struct ACPISDTHeader* currentTable = (struct ACPISDTHeader*)entries[i];

        // Check if the current table has the FACP signature
        if (currentTable->Signature == 0x50434146 /* 'FACP' in ASCII */) {
            return (struct FACP*)currentTable;  // Found the FACP, return its pointer
        }
    }

    // FACP not found in the RSDT
    return NULL;
}