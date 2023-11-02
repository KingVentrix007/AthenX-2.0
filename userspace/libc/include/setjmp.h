#ifndef __SETJMP__H
#define __SETJMP__H
struct RegisterState {
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int es, ds;
};
#endif