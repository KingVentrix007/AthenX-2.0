
 #include "syscall.h"
#include "stddef.h"
#include "stdlib.h"
#define INT_MAX 2147483647
#define INT_MIN -2147483648








uint32_t atoi(const char *str)
{
    uint32_t sign = 1, base = 0, i = 0;

    // if whitespaces then ignore.
    while (str[i] == ' ')
    {
        i++;
    }

    // sign of number
    if (str[i] == '-' || str[i] == '+')
    {
        sign = 1 - 2 * (str[i++] == '-');
    }

    // checking for valid input
    while (str[i] >= '0' && str[i] <= '9')
    {
        // handling overflow test case
        if (base > INT_MAX / 10 || (base == INT_MAX / 10 && str[i] - '0' > 7))
        {
            if (sign == 1)
                return INT_MAX;
            else
                return INT_MIN;
        }
        base = 10 * base + (str[i++] - '0');
    }
    return base * sign;
}

/**
 * Function Name: malloc
 * Description: Memory allocation function.
 *
 * Parameters:
 *   size (size_t) - Size of the memory to allocate.
 *
 * Return:
 *   void* - A pointer to the allocated memory, or NULL if allocation fails.
 */
void* malloc(size_t size) {
    printf("\nrequested %d bytes of memory\n", size);
    void* memory = syscall(SYS_MMAP,size,0);
    if (memory == NULL) {
        // Allocation failed
        printf("Memory allocation failed.\n");
    }
    return memory;
}

/**
 * Function Name: free
 * Description: Memory deallocation function.
 *
 * Parameters:
 *   ptr (void*) - Pointer to the memory to deallocate.
 *
 * Return:
 *   None
 */
void free(void* ptr) {
    if (ptr != NULL) {
        syscall(SYS_MUNMAP,ptr,0);
    }
}
/**
 * Function Name: realloc
 * Description: Memory reallocation function.
 * This function changes the size of the memory block pointed to by ptr to the new size,
 * preserving the existing data. If necessary, it may move the data to a new location.
 *
 * Parameters:
 *   ptr (void*) - A pointer to the previously allocated memory block.
 *   size (size_t) - The new size, in bytes, for the memory block.
 *
 * Return:
 *   void* - A pointer to the reallocated memory block, or NULL if the reallocation fails.
 */
void* realloc(void* ptr, size_t size) {
    return syscall(SYS_REALLOC,ptr, size);
}
/**
 * Function Name: calloc
 * Description: Memory allocation function for initialized memory.
 * This function allocates a block of memory for an array of elements, each of a specified size,
 * and initializes all its bits to zero.
 *
 * Parameters:
 *   num (size_t) - The number of elements to allocate memory for.
 *   size (size_t) - The size, in bytes, of each element.
 *
 * Return:
 *   void* - A pointer to the allocated and zero-initialized memory block, or NULL if the allocation fails.
 */
void* calloc(size_t num, size_t size) {
    return syscall(SYS_CALLOC, num, size);
}
int exit(int code)
{
    return code;
}