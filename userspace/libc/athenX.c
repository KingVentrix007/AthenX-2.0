#define ATHENX_OS
#ifdef ATHENX_OS
#include "athneX.h"
#include "stddef.h"
 #include "syscall.h"








void athenx_memalign(size_t size, size_t alignment)
{
    return syscall(SYS_ALLIGEND_MEM,size,alignment); 
}









#endif