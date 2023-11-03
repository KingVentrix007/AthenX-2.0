#include "printf.h"


void enable_paging_c() {
    __asm__ __volatile__(
        "call enable_paging"
    );
}