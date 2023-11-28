
#include "../include/debug.h"
#include "../include/kheap.h"
#include "../include/console.h"
#include "../include/kernel.h"
#include "../include/pmm.h"
#include "../include/string.h"
#include "../include/terminal.h"
#include "../include/stddef.h"
#include "../include/debug.h"
#include "../include/stdio.h"
// start & end addresses pointing to memory
void *g_kheap_start_addr = NULL, *g_kheap_end_addr = NULL;
unsigned long g_total_size = 0;
unsigned long g_total_used_size = 0;
// list head
KHEAP_BLOCK *g_head = NULL;

/**
 * @file kheap.c
 * @brief Implementation of kheap_init() function to initialize the heap and set the total memory size.
 * @language C
 */

/**
 * @brief Initialize the heap and set the total memory size.
 *
 * This function initializes the heap memory by setting the start and end addresses, calculating the total size, and initializing the total used size to 0.
 *
 * @param start_addr The starting address of the heap memory.
 * @param end_addr The ending address of the heap memory.
 * @return 0 on successful initialization, -1 if the start address is greater than the end address.
 *
 * @details The function performs the following steps:
 * 1. Checks if the start address is greater than the end address. If true, it prints an error message and returns -1.
 * 2. Sets the global variable g_kheap_start_addr to the provided start address.
 * 3. Sets the global variable g_kheap_end_addr to the provided end address.
 * 4. Calculates the total size of the heap memory by subtracting the start address from the end address.
 * 5. Sets the global variable g_total_size to the calculated total size.
 * 6. Initializes the global variable g_total_used_size to 0.
 * 7. Prints the total size for debugging purposes.
 * 8. Returns 0 to indicate successful initialization.
 */
int kheap_init(void *start_addr, void *end_addr) {
    if (start_addr > end_addr) {
        printf_("failed to init kheap\n");
        return -1;
    }
    g_kheap_start_addr = start_addr;
    g_kheap_end_addr = end_addr;
    g_total_size = end_addr - start_addr;
    printf_("total size ->%d\n", g_total_size);
    g_total_used_size = 0;
    return 0;
}
/**
 * increase the heap memory by size & get its address
*/
void *kbrk(int size) {
    FUNC_ADDR_NAME(&kbrk, 1, "i");
    void *addr = NULL;
    if (size <= 0)
        return NULL;
    // Check if memory is available or not
    if (g_total_size - g_total_used_size < size + sizeof(void*)) {
         printf("(kbrk)Memory allocation failed: Not enough free space:\nrequested: %d \n| total %d \n| extra nedded %d\n",size,g_total_size,g_total_size-size);
            
        sleep(10);
        return NULL;
    }
    // Add start address with total previously used memory and difference between each data block pointer
    addr = g_kheap_start_addr + g_total_used_size;
    g_total_used_size += size + sizeof(void*);
    return addr;
}

/**
 * print list of allocated blocks
*/
void kheap_print_blocks() {
    FUNC_ADDR_NAME(&kheap_print_blocks,0,"");
    KHEAP_BLOCK *temp = g_head;
    printf_("{/330:0,255,255}");
    printf_("Block Size: %d\n", sizeof(KHEAP_BLOCK));
    printf_("{/330:0,255,0}");
    while (temp != NULL) {
        printf_("size:%d, free:%d, data: 0x%x, curr: 0x%x, next: 0x%x\n",
               temp->metadata.size, temp->metadata.is_free, temp->data, temp, temp->next);
        temp = temp->next;
    }
}

BOOL is_block_free(KHEAP_BLOCK *block) {
    FUNC_ADDR_NAME(&is_block_free,1,"T");
    if (!block)
        return FALSE;
    return (block->metadata.is_free == TRUE);
}

/**
 * this just check freed memory is greater than the required one
*/
KHEAP_BLOCK *worst_fit(int size) {
    FUNC_ADDR_NAME(&worst_fit,1,"i");
    KHEAP_BLOCK *temp = g_head;
    while (temp != NULL) {
        if (is_block_free(temp)) {
            if ((int)temp->metadata.size >= size)
                return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// allocate a new heap block
KHEAP_BLOCK *allocate_new_block(int size) {
    FUNC_ADDR_NAME(&allocate_new_block,1,"i");
    KHEAP_BLOCK *temp = g_head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    KHEAP_BLOCK *new_block = (KHEAP_BLOCK *)kbrk(sizeof(KHEAP_BLOCK));
    new_block->metadata.is_free = FALSE;
    new_block->metadata.size = size;
    new_block->data = kbrk(size);
    new_block->next = NULL;
    temp->next = new_block;
    return new_block;
}

/**
 * allocate given size if list is null
 * otherwise try some memory allocation algorithm like best fit etc
 * to find best block to allocate
 * # Need to work on internal/external segmentaion problem
*/
void *kmalloc(size_t size) {
    //printf("Allocting memory of size ->%d\n",size);
    FUNC_ADDR_NAME(&kmalloc, 1, "i");
    if (size <= 0)
        return NULL;
    if (g_head == NULL) {
        // Initialize the heap if it's empty
        g_head = (KHEAP_BLOCK *)kbrk(sizeof(KHEAP_BLOCK));
        if (g_head == NULL) {
            printf("Memory allocation failed: Not enough free space for initial block: requested %d | total %d | extra nedded %d\n",size,g_total_size,g_total_size-size);
            sleep(10);
            return NULL;
        }
        g_head->metadata.is_free = FALSE;
        g_head->metadata.size = size;
        g_head->next = NULL;
        g_head->data = kbrk(size);
        if (g_head->data == NULL) {
            printf("Memory allocation failed: Not enough free space for data: requested %d | total %d | extra nedded %d\n",size,g_total_size,g_total_size-size);
            sleep(10);
            sleep(10);
            return NULL;
        }
        return g_head->data;
    } else {
        KHEAP_BLOCK *worst = worst_fit(size);
        if (worst == NULL) {
            KHEAP_BLOCK *new_block = allocate_new_block(size);
            if (new_block == NULL) {
                perror("Memory allocation failed: Not enough free space for a new block.\n");
                sleep(10);
                return NULL;
            }
            return new_block->data;
        } else {
            worst->metadata.is_free = FALSE;
            return worst->data;
        }
    }
    return NULL;
}


/**
 * allocate memory n * size & zeroing out
*/
void *kcalloc(int n, int size) {
    FUNC_ADDR_NAME(&kcalloc, 2, "ii");
    if (n < 0 || size < 0)
        return NULL;
    size_t total_size = n * size;
    void *mem = kmalloc(total_size);
    if (mem != NULL) {
        memset(mem, 0, total_size);
    }
    return mem;
}
/**
 * allocate a new block of memory
 * copy previous block data & set free the previous block
*/
void *krealloc(void *ptr, int size) {
    FUNC_ADDR_NAME(&krealloc,2,"pi");
    KHEAP_BLOCK *temp = g_head;
    while (temp != NULL) {
        if (temp->data == ptr) {
            KHEAP_BLOCK *new_block = allocate_new_block(size);
            if ((int)temp->metadata.size > size)
                memcpy(new_block->data, ptr, size);
            else
                memcpy(new_block->data, ptr, temp->metadata.size);
            temp->metadata.is_free = TRUE;
            return new_block->data;
        }
        temp = temp->next;
    }
    return NULL;
}

/**
 * set free the block
*/
void kfree(void *addr) {
    FUNC_ADDR_NAME(&kfree,1,"p");
    KHEAP_BLOCK *temp = g_head;
    while (temp != NULL) {
        if (temp->data == addr) {
            temp->metadata.is_free = TRUE;
            return;
        }
        temp = temp->next;
    }
}

void* malloc_aligned(size_t size, size_t alignment) {
    void* mem = kmalloc(size + alignment - 1);
    
    if (mem == NULL) {
        // Handle allocation failure.
        return NULL;
    }
    
    // Calculate the aligned pointer within the allocated memory.
    void* aligned_mem = (void*)(((uintptr_t)(mem + alignment - 1) / alignment) * alignment);
    
    return aligned_mem;
}
