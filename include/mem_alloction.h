#ifndef MEMORY_ALLOCATION_H
#define MEMORY_ALLOCATION_H

#include <stdint.h>
#include <stdbool.h>

// Structure to represent memory allocation
typedef struct {
    uint8_t* pointer;  // Pointer to the allocated memory
    uint64_t size;     // Size of the allocated memory
    char name[20]; // Name of the allocation
} Allocation;

/**
 * Function Name: init_memory_allocation
 * Description: Initializes memory allocation by setting the memory region and clearing the bitmap.
 *
 * Parameters:
 *   mem_s (uint8_t*) - Pointer to the start of the memory region
 *   mem_e (uint8_t*) - Pointer to the end of the memory region
 *
 * Return:
 *   void
 */

void init_memory_allocation(uint8_t mem_start, uint64_t mem_size);

/**
 * Function Name: alloc_low
 * Description: Finds and allocates a block of memory from the low end.
 *
 * Parameters:
 *   size (uint64_t) - Size of the memory block to allocate
 *
 * Return:
 *   Allocation - Structure containing the allocated memory pointer and size
 */
Allocation alloc_low(uint64_t size);

/**
 * Function Name: free_low
 * Description: Frees a block of memory from the low end.
 *
 * Parameters:
 *   allocation (Allocation) - Structure containing the memory pointer and size to free
 *
 * Return:
 *   void
 */
void free_low(Allocation allocation);

/**
 * Function Name: kmalloc
 * Description: Allocates memory using the alloc_low function and keeps track of allocations.
 *
 * Parameters:
 *   size (int) - Size of the memory block to allocate
 *
 * Return:
 *   void* - Pointer to the allocated memory
 */
void* kalloc(int size);

/**
 * Function Name: sys_kmalloc
 * Description: Allocates system memory using the alloc_low function and appends it to the system allocations list.
 *
 * Parameters:
 *   size (int) - Size of the memory block to allocate
 *
 * Return:
 *   void* - Pointer to the allocated system memory
 */
void* sys_kmalloc(int size);

#endif // MEMORY_ALLOCATION_H