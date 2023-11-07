; paging.asm
section .text
global load_page_directory
global enable_paging

; Function to load the page directory base address into CR3
load_page_directory:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8] ; The page directory address is an argument.
    mov cr3, eax
    ; Flushing the TLB
    mov eax, cr4
    or eax, 0x90
    mov cr4, eax
    mov esp, ebp
    pop ebp
    ret

; Function to set the most significant bit of CR0 to enable paging
enable_paging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80000000 ; Set the PG bit of CR0 to enable paging.
    mov cr0, eax
    mov esp, ebp
    pop ebp
    ret
