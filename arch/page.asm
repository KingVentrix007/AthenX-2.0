section .bss
align 4096
page_directory:
  resb 4096             ; Page directory with 1024 entries
page_table:
  resb 4096             ; Page table with 1024 entries

section .text
global enable_paging
enable_paging:
  cli ; Disable interrupts

  ; Load the physical address of the page directory into CR3
  mov eax, page_directory
  mov cr3, eax

  ; Set the paging bit (bit 31) in CR0 to enable paging
  mov eax, cr0
  or eax, 0x80000001
  mov cr0, eax

  ; Identity mapping for the first 4 MB (kernel space)
  mov eax, 0
  mov ebx, 0x0100000 ; Physical address where the kernel is loaded
  .loop:
    mov ecx, ebx
    or ecx, 0x3     ; Set the permissions to Read/Write
    mov dword [page_table + eax], ecx
    add ebx, 4096 ; Increment to the next page
    add eax, 4    ; Increment to the next page table entry
    cmp eax, 1024 ; Check if all entries for kernel space are done
    jne .loop

  ; Identity mapping for other memory regions, if needed

  sti ; Enable interrupts

  ; Jump to a label, not an absolute address
  jmp paging_enabled

paging_enabled:
  ret
