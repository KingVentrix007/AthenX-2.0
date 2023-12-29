
#include "../../../include/types.h"
#include "stdint.h"
#include "../../../include/mem_alloction.h"
static uint8_t* bitmap_start = NULL;
static uint64_t bitmap_size = 0;
// Memory region information
static uint8_t* mem_start = NULL;
static uint8_t* mem_end = NULL;
Allocation *allocations = NULL;
int num_memory_allocations = 0;
Allocation system_allocations[10] = {0};

// ...

void init_memory_allocation(uint8_t mem_start, uint64_t mem_size) {
    // Initialize memory region and bitmap.
    printf("Initializing memory allocation\n");
    bitmap_start = (uint32 *)mem_start;
    bitmap_size = mem_size / 8;
    printf("memory size: %d\n", mem_size);
    printf("bitmap size: %d\n", bitmap_size);
    if(mem_size > 0)
    {
        printf("True\n");
    }
    printf("Available memory allocation space: %d\n", mem_size-bitmap_size);
    // Clear the bitmap (set all bits to 0, indicating free memory).
    for (uint64_t i = 0; i < bitmap_size; i++) {
        bitmap_start[i] = 0;
    }
    allocations = (Allocation *)sys_kmalloc(sizeof(Allocation) * 10);
    printf("initalized allocation\n");
}

// ...


// Function to find and allocate a block of memory from the low end
Allocation alloc_low(uint64_t size) {
    Allocation allocation = {NULL, 0};

    // Calculate the number of bytes needed for the specified size
    uint64_t required_bytes = (size + 7) / 8;  // Round up to the nearest byte

    // Iterate through the bitmap to find a free block of the required size
    for (uint64_t i = 0; i < bitmap_size * 8; i++) {
        // Check if the current bit is free
        if ((bitmap_start[i / 8] & (1 << (i % 8))) == 0) {
            uint64_t j;
            // Check if the consecutive bits are free
            for (j = i; j < i + required_bytes * 8; j++) {
                if ((bitmap_start[j / 8] & (1 << (j % 8))) != 0) {
                    break;  // This bit is not free, break the loop
                }
            }

            // If we found a block of the required size, mark the bits as used
            if (j == i + required_bytes * 8) {
                for (uint64_t k = i; k < j; k++) {
                    bitmap_start[k / 8] |= (1 << (k % 8));  // Mark the bit as used
                }

                // Calculate the pointer to the allocated memory
                allocation.pointer = mem_start + i * 8;
                allocation.size = size;

                break;  // Break the loop, as we found a suitable block
            }
        }
    }

    return allocation;
}

// Function to free a block of memory from the low end
void free_low(Allocation allocation) {
    // Calculate the starting bit position in the bitmap for the given pointer
    uint64_t start_bit = (allocation.pointer - mem_start) * 8;

    // Calculate the number of bytes needed for the specified size
    uint64_t required_bytes = (allocation.size + 7) / 8;  // Round up to the nearest byte

    // Mark the corresponding bits as free in the bitmap
    for (uint64_t i = start_bit; i < start_bit + required_bytes * 8; i++) {
        bitmap_start[i / 8] &= ~(1 << (i % 8));  // Mark the bit as free
    }
}

void *kalloc(int size)
{
    Allocation allocation = alloc_low(size);
    if (allocation.pointer == NULL)
    {
        return NULL;
    }
    else
    {
        allocations[num_memory_allocations] = allocation;
        num_memory_allocations++;
        return allocation.pointer;
    }
}

#define MAX_SYSTEM_ALLOCATIONS 10

void append_to_system_allocations(Allocation allocation) {
    // Check if the system allocations list has reached its maximum capacity
    if (system_allocations[MAX_SYSTEM_ALLOCATIONS - 1].pointer != NULL) {
        // Handle the case where the list is full
        // You may want to implement a more robust handling mechanism
        return;
    }

    // Find the first empty slot in the system allocations list
    for (int i = 0; i < MAX_SYSTEM_ALLOCATIONS; i++) {
        if (system_allocations[i].pointer == NULL) {
            // Copy the Allocation structure to the system allocations list
            system_allocations[i] = allocation;
            break;  // Exit the loop after appending the allocation
        }
    }
}
void * sys_kmalloc(int size)
{
    Allocation allocation = alloc_low(size);
    if (allocation.pointer == NULL)
    {
        return NULL;
    }
    else
    {
        append_to_system_allocations(allocation);
        return allocation.pointer;
    }
}