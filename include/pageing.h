#ifndef __PAGEING__H
#define __PAGEING__H

#include <stdint.h>
#include "printf.h"
#include "multiboot.h"
#include "stdbool.h"
// not sure why alignas() isn't working for me in MSVC 2015
// I can't get __declspect(align(4096)) to work either, the linker complains that 
// the /Align:512 switch is too low, but making it higher breaks the kernel :(
typedef /*__declspec(align(4096))*/ uint32_t PAGE_DIRECTORY_ENTRY;
typedef /*__declspec(align(4096))*/ uint32_t PAGE_TABLE_ENTRY;

#define PAGE_ENTRY_PRESENT          1
#define PAGE_ENTRY_WRITABLE         2
#define PAGE_ENTRY_USER_ACCESSIBLE  4

#define DIRECTORY_ENTRY_PRESENT          1
#define DIRECTORY_ENTRY_WRITABLE         2
#define DIRECTORY_ENTRY_USER_ACCESSIBLE  4
#define DIRECTORY_ENTRY_4MB              0x80

#define FOUR_MEGABYTES              0x400000
#define PAGING_ADDRESS_BITS         0xFFFFF000

extern uint32_t paging_space[0x3FFF];
extern uint32_t *pageDir;

extern uint32_t pagingNextAvailableMemory;
extern uint32_t paging4MPagesAvailable;
extern uint32_t nextPageDirectory; // TEMPTEMP

typedef uint32_t ULONG_PTR;

void KPageAllocator(unsigned int pages, unsigned int *pPagesAllocated, uint32_t *pRetVal);

// // setup the page directory and page tables and enable paging
// inline void Paging_Enable(MULTIBOOT_INFO *multibootInfo)
// {
//     // pageDirectory uses the global paging_space array. Since paging_space is global, the compiler thinks it's located relative to
//     // BASE_ADDRESS, when it's really relative to LOADBASE. We do some math to find the actual address of paging_space.
//     PAGE_DIRECTORY_ENTRY *pageDirectory = (PAGE_DIRECTORY_ENTRY*)((uint32_t)paging_space - BASE_ADDRESS + LOADBASE);

//     // The page tables will also use reside in the paging_space global array
//     PAGE_TABLE_ENTRY *initialKernelTable;   // for mapping the kernel to 0xC000 0000
//     PAGE_TABLE_ENTRY *videoIdentityTable;   // for identity mapping the linear frame buffer (HACK!)

//     // Ensure pageDirectory is aligned on a 0x1000-byte boundary
//     if ((ULONG_PTR)pageDirectory % 0x1000 != 0)
//         pageDirectory = (PAGE_TABLE_ENTRY*)((ULONG_PTR)pageDirectory - ((ULONG_PTR)pageDirectory % 0x1000) + 0x1000);

//     // Have the page tables follow pageDirectory in memory
//     initialKernelTable = (PAGE_TABLE_ENTRY *)((uint32_t)pageDirectory + 0x1000);
//     videoIdentityTable = (PAGE_TABLE_ENTRY *)((uint32_t)pageDirectory + 0x2000);

//     uint32_t i;

//     // Clear out the page directory
//     for (i = 0; i < 1024; ++i)
//     {
//         pageDirectory[i] = DIRECTORY_ENTRY_USER_ACCESSIBLE | DIRECTORY_ENTRY_WRITABLE;
//     }

//     // Setup identity mapping for the first four megabytes
//     pageDirectory[0] = (PAGE_DIRECTORY_ENTRY)(0x00 | DIRECTORY_ENTRY_PRESENT | DIRECTORY_ENTRY_WRITABLE | DIRECTORY_ENTRY_4MB);

//     // Setup identity mapping for the four megabytes starting at megabyte 4
//     pageDirectory[1] = (PAGE_DIRECTORY_ENTRY)((uint32_t)0x400000 | DIRECTORY_ENTRY_PRESENT | DIRECTORY_ENTRY_WRITABLE | DIRECTORY_ENTRY_4MB | DIRECTORY_ENTRY_USER_ACCESSIBLE);

//     // Setup identity mapping for the four megabytes starting at megabyte 8
//     pageDirectory[2] = (PAGE_DIRECTORY_ENTRY)((uint32_t)0x800000 | DIRECTORY_ENTRY_PRESENT | DIRECTORY_ENTRY_WRITABLE | DIRECTORY_ENTRY_4MB | DIRECTORY_ENTRY_USER_ACCESSIBLE);

//     // Identity map the next four megs (this is used by the GUI shell)
//     pageDirectory[3] = (PAGE_DIRECTORY_ENTRY)((uint32_t)0xC00000 | DIRECTORY_ENTRY_PRESENT | DIRECTORY_ENTRY_WRITABLE | DIRECTORY_ENTRY_4MB | DIRECTORY_ENTRY_USER_ACCESSIBLE);

//     // Identity map the next four megs
//     pageDirectory[4] = (PAGE_DIRECTORY_ENTRY)((uint32_t)0x1000000 | DIRECTORY_ENTRY_PRESENT | DIRECTORY_ENTRY_WRITABLE | DIRECTORY_ENTRY_4MB | DIRECTORY_ENTRY_USER_ACCESSIBLE);

//     // Map the kernel (4 megs starting at 0x10 0000) to 0xC000 0000
//     for (i = 0; i < 1024; ++i)
//     {
//         initialKernelTable[i] = (0x100000 + i * 0x1000) | PAGE_ENTRY_PRESENT | PAGE_ENTRY_WRITABLE;   // attributes: supervisor level, read/write, present
//     }

//     // put the kernel page table in the page directory into entry 768, which will map it to 0xC000 0000
//     pageDirectory[768] = (PAGE_DIRECTORY_ENTRY)((uint32_t)initialKernelTable | DIRECTORY_ENTRY_PRESENT | DIRECTORY_ENTRY_WRITABLE);

//     // TEMPTEMP HACKHACK! - identity map the linear frame buffer, which on my Qemu starts at 0xFD00 0000
//     uint32_t lfbAddress = 0xFD000000;

// // TODO: This section of code will bug out if GRUB_GRAPHICS isn't defined in VirtualBox
// // TODO: We can conditionally comment this out, but then the gfx command fails to switch modes properly
//     // see if Grub gave us an lfb address and use that one if it did
//     if (multibootInfo->flags &  MULTIBOOT_INFO_FRAMEBUFFER_INFO)
//     {
//         // fingers crossed that lfb is 32-bits // (TODO)
//         lfbAddress = (uint32_t)multibootInfo->framebuffer_addr;
//     }

//     // TODO: allow the lfb to exist anywhere in memory
//     // Setup identity mapping for the four megabytes starting at 0xFD00 0000
//     for (i = 0; i < 1024; ++i)
//     {
//         videoIdentityTable[i] = (lfbAddress + i * 0x1000) | PAGE_ENTRY_PRESENT | PAGE_ENTRY_WRITABLE;   // attributes: supervisor level, read/write, present
//     }
// // End buggy section

//     // put the lfb page table in the page directory
//     uint32_t page = lfbAddress / 0x400000;
//     pageDirectory[page] = (PAGE_DIRECTORY_ENTRY)((uint32_t)videoIdentityTable | DIRECTORY_ENTRY_PRESENT | DIRECTORY_ENTRY_WRITABLE);

//     // load page directory into cr3
//     write_cr3((uint32_t)pageDirectory);

//     // enable PSE (4MB pages)
//     write_cr4(read_cr4() | 0x10);

//     // enable paging!
//     write_cr0(read_cr0() | 0x80000000);

//     // save a pointer to pageDirectory
//     pageDir = pageDirectory;

//     // determine available pages
//     pagingNextAvailableMemory = FOUR_MEGABYTES * 5; // TEMP: Next available page will (likely) start at 20 Megs
//     // We'll assume we have 64 megs available
//     paging4MPagesAvailable = 16; // TODO: Calculate based on the memory map Grub gave us

//     // TEMPTEMP: put page directories between 16 - 20 megs
//     nextPageDirectory = FOUR_MEGABYTES * 4;

//     // Walk through the mem map grub gave us
//     // TODO: Assert that grub gave us a memory map
//     // TODO: Don't assume we have memory at the beginning of RAM
//     // TODO: Use all the entries we get from GRUB
//     MULTIBOOT_MEMORY_MAP *entry = (MULTIBOOT_MEMORY_MAP *)multibootInfo->mmap_addr;
    
//     // examine each mmap entry
//     for (uint32_t offset = 0; offset <= multibootInfo->mmap_length && entry->size; offset += entry->size + 4)
//     {
//         if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
//         {
//             // If the next page of memory we're assuming we can use resides here
//             if (pagingNextAvailableMemory >= entry->addr_low && pagingNextAvailableMemory < (entry->addr_low + entry->len_low))
//             {
//                 // determine how many pages we really have available
//                 uint64_t addrEnd = entry->addr_low + entry->len_low;
//                 uint64_t bytesAvailable = addrEnd - pagingNextAvailableMemory;
//                 paging4MPagesAvailable = bytesAvailable / FOUR_MEGABYTES;
//                 return;
//             }
//         }        

//         // advance pointer to next entry
//         entry = (MULTIBOOT_MEMORY_MAP*)((uint32_t)entry + entry->size + 4);
//     }
// }

bool Paging_Print_Page_Table(PAGE_DIRECTORY_ENTRY *thePageDir);

uint32_t Paging_Get_Physical_Address(void *address);
#endif