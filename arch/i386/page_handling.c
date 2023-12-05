/**
 * @file paging.c
 * @brief Paging Management Functions and Definitions
 * @author KingVentrix007
 * @date <date>
 * @license GNU GPLv3 (https://www.gnu.org/licenses/gpl-3.0.en.html)
 *
 * @details This file contains functions and definitions for managing paging in the operating system.
 * It includes functions for allocating and mapping memory pages, printing page tables, and accessing control registers.
 *
 * @note If you choose to use the GNU GPLv3 license, please ensure that your project complies with the terms of the license.
 */

#include "printf.h"
#include "types.h"
#include "pageing.h"
#include "stdbool.h"

uint32_t paging_space[0x3FFF]; /**< Space for paging structures */
uint32_t *pageDir; /**< Pointer to the page directory */
uint32_t pagingNextAvailableMemory; /**< Physical address of the next available page */
uint32_t paging4MPagesAvailable; /**< Number of 4M physical memory pages available to be mapped */
uint32_t nextPageDirectory; /**< Temporary storage for the next created page directory */

// /**
//  * @brief Allocate a number of consecutive 4-megabyte pages.
//  *
//  * @param pages The number of pages to allocate.
//  * @param pPagesAllocated Pointer to store the number of pages actually allocated.
//  * @param pRetVal Pointer to store the virtual address of the first allocated page.
//  *
//  * @note Due to memory usage and memory mapping, the function may allocate less than the requested number of pages.
//  * @note The function assumes identity mapping.
//  *
//  * @return The virtual address of the first allocated page, or NULL if there was a problem.
//  */
// void KPageAllocator(unsigned int pages, unsigned int *pPagesAllocated, uint32_t *pRetVal) {
//     // Ensure interrupts are disabled
//     *pPagesAllocated = 0;
//     // Make sure there are enough pages available
//     if (paging4MPagesAvailable < pages) {
//         printf("Not enough pages available, %d out of %d\n", paging4MPagesAvailable, pages);
//         *pRetVal = (uint32_t)NULL;
//         return;
//     }
//     *pRetVal = pagingNextAvailableMemory;
//     // Limit the number of pages to allocate to the number available
//     if (pages > paging4MPagesAvailable)
//         pages = paging4MPagesAvailable;
//     uint32_t nextPage = pagingNextAvailableMemory / FOUR_MEGABYTES;
//     // Get the page directory we'll be using
//     PAGE_DIRECTORY_ENTRY *pageDirectory = (PAGE_DIRECTORY_ENTRY *)tasks[currentTask].cr3;
//     // Add each page to the page directory
//     for (size_t allocated = 0; allocated < pages; ++allocated) {
//         // Add the current page to the page directory
//         pageDirectory[nextPage] = ((nextPage * FOUR_MEGABYTES)
//             | DIRECTORY_ENTRY_PRESENT | DIRECTORY_ENTRY_USER_ACCESSIBLE | DIRECTORY_ENTRY_WRITABLE | DIRECTORY_ENTRY_4MB);
//         // If we're mapping this into the kernel space, we need to copy that mapping into every running task
//         if (pageDirectory == pageDir) {
//             for (int i = 0; i < MAX_TASKS; ++i) {
//                 if (!tasks[i].inUse || tasks[i].cr3 == (uint32_t)pageDir)
//                     continue;
//                 PAGE_DIRECTORY_ENTRY *otherPageDir = (PAGE_DIRECTORY_ENTRY *)tasks[i].cr3;
//                 otherPageDir[nextPage] = ((nextPage * FOUR_MEGABYTES)
//                                            | DIRECTORY_ENTRY_PRESENT | DIRECTORY_ENTRY_USER_ACCESSIBLE | DIRECTORY_ENTRY_WRITABLE | DIRECTORY_ENTRY_4MB);
//             }
//         }
//         // Update pointers and counters
//         ++nextPage;
//         --paging4MPagesAvailable;
//         pagingNextAvailableMemory += FOUR_MEGABYTES;
//         ++(*pPagesAllocated);
//     }
// }

// /**
//  * @brief Print the page table entries in the given page directory.
//  *
//  * @param thePageDir The page directory to print.
//  *
//  * @return True if all entries in the page directory are empty, false otherwise.
//  */
// bool Paging_Print_Page_Table(PAGE_DIRECTORY_ENTRY *thePageDir) {
//     bool done = true;
//     printf("Page table entries:\nLogical -> Physical  -- flags\n");
//     for (int i = 0; i < 1024; ++i) {
//         if (!(thePageDir[i] & PAGE_ENTRY_PRESENT))
//             continue;
//         done = false;
//         printf("0x%X -> 0x%X   - 0x%X\n", i * FOUR_MEGABYTES, thePageDir[i] & 0xFFFFF000, thePageDir[i] & 0xFFF);
//     }
//     return done;
// }

// /**
//  * @brief Get the physical address corresponding to a given virtual address.
//  *
//  * @param address The virtual address.
//  *
//  * @return The physical address.
//  */
// uint32_t Paging_Get_Physical_Address(void *address) {
//     uint32_t vAddr = (uint32_t)address;
//     uint32_t index = vAddr / FOUR_MEGABYTES;
//     uint32_t offset = vAddr - (index * FOUR_MEGABYTES);
//     // Get the page directory
//     PAGE_DIRECTORY_ENTRY *pDir = tasks[currentTask].cr3;
//     uint32_t pAddr = pDir[index] & PAGING_ADDRESS_BITS;
//     return pAddr + offset;
// }

/**
 * @brief Write the value to the CR3 register.
 *
 * @param cr3_value The value to write to the CR3 register.
 */
void write_cr3(unsigned long cr3_value) {
    // Inline assembly to write to the CR3 register
    asm volatile("mov %0, %%cr3" : : "r" (cr3_value));
}

/**
 * @brief Write the value to the CR4 register.
 *
 * @param cr4_value The value to write to the CR4 register.
 */
void write_cr4(unsigned long cr4_value) {
    // Inline assembly to write to the CR4 register
    asm volatile("mov %0, %%cr4" : : "r" (cr4_value));
}

/**
 * @brief Write the value to the CR0 register.
 *
 * @param cr0_value The value to write to the CR0 register.
 */
void write_cr0(unsigned long cr0_value) {
    // Inline assembly to write to the CR0 register
    asm volatile("mov %0, %%cr0" : : "r" (cr0_value));
}

/**
 * @brief Write the value to the CR2 register.
 *
 * @param cr2_value The value to write to the CR2 register.
 */
void write_cr2(unsigned long cr2_value) {
    // Inline assembly to write to the CR2 register
    asm volatile("mov %0, %%cr2" : : "r" (cr2_value));
}

/**
 * @brief Read the value from the CR4 register.
 *
 * @return The value read from the CR4 register.
 */
unsigned long read_cr4() {
    unsigned long cr4_value;
    // Inline assembly to read from the CR4 register
    asm volatile("mov %%cr4, %0" : "=r" (cr4_value));
    return cr4_value;
}

/**
 * @brief Read the value from the CR0 register.
 *
 * @return The value read from the CR0 register.
 */
unsigned long read_cr0() {
    unsigned long cr0_value;
    // Inline assembly to read from the CR0 register
    asm volatile("mov %%cr0, %0" : "=r" (cr0_value));
    return cr0_value;
}

/**
 * @brief Read the value from the CR2 register.
 *
 * @return The value read from the CR2 register.
 */
unsigned long read_cr2() {
    unsigned long cr2_value;
    // Inline assembly to read from the CR2 register
    asm volatile("mov %%cr2, %0" : "=r" (cr2_value));
    return cr2_value;
}