#include "printf.h"
#include "types.h"
// A single page table to map the first 4MB of memory.
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

// A single page directory entry.
uint32_t page_directory[1024] __attribute__((aligned(4096)));

void init_paging() {
    // Identity map the first 4MB of memory.
    for (int i = 0; i < 1024; i++) {
        // Each page table entry points to a 4KB page, with read/write and present attributes.
        first_page_table[i] = (i * 0x1000) | 3;
    }
    
    // Set the first entry of the page directory to point to our page table.
    page_directory[0] = ((unsigned int)first_page_table) | 3;

    // Zero out the rest of the page directory.
    for (int i = 1; i < 1024; i++) {
        page_directory[i] = 0 | 2; // Attributes: supervisor-level, read/write, not present.
    }

    // Load the page directory.
    load_page_directory(page_directory);

    // Enable paging.
    enable_paging();
}