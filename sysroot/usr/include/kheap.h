#ifndef KHEAP_H
#define KHEAP_H

#include "types.h"
#include "stddef.h"

// a singly linked list heap block
typedef struct _kheap_block {
    struct {
        uint32 size;  // memory size
        uint8 is_free; // block is free or not
    } metadata;
    struct _kheap_block *next; 
    void *data;  // data pointer
} __attribute__((packed)) KHEAP_BLOCK;


/**
 * initialize heap and set total memory size
*/
int kheap_init(void *start_addr, void *end_addr);

/**
 * increase the heap memory by size & get its address
*/
void *kbrk(int size);

/**
 * print list of allocated blocks
*/
void kheap_print_blocks();

/**
 * allocate given size if list is null
 * otherwise try some memory allocation algorithm like best fit etc
 * to find best block to allocate
 * # Need to work on internal/external segmentaion problem
*/
void *kmalloc(size_t size);

/**
 * allocate memory n * size & zeroing out
*/
void *kcalloc(int n, int size);

/**
 * allocate a new block of memory
 * copy previous block data & set free the previous block
*/
void *krealloc(void *ptr, int size);

/**
 * set free the block
*/
void kfree(void *addr);
#define malloc(size) kmalloc(size)
#define free(ptr) kfree(ptr)
#define realloc(ptr, size) krealloc(ptr, size)
void* malloc_aligned(size_t size, size_t alignment);
#endif
