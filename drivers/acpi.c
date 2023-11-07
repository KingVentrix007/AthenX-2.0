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


