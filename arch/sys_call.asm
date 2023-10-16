section .text

global system_call

system_call:
    ; Arguments:
    ;   [esp+4]  - syscall_number
    ;   [esp+8]  - arg1
    ;   [esp+12] - results (pointer)
    
    mov     eax, [esp+4]          ; Load syscall_number into eax
    mov     ebx, [esp+8]          ; Load arg1 into ebx
    int     42                    ; Trigger the system call
    mov     [esp+12], eax         ; Store the result in results
    
    ret