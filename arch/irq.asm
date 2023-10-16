; filename irq.asm
section .text
    extern isr_irq_handler
    extern sys_handler
irq_handler:
    pusha                 ; push all registers
    mov ax, ds
    push eax              ; save ds

    mov ax, 0x10          ; load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    
    ; Load the interrupt number from the stack
    mov eax, [esp+28]

    cmp eax, 42            ; Compare the interrupt number to 42
    jne not_42             ; If it's not 42, jump to not_42

    ; Call dummy_func if the interrupt is 42
    call sys_handler
    jmp mod_done

not_42:
    call isr_irq_handler  ; Call isr_irq_handler for other interrupts
    
     jne mod_done
done:
    pop esp

    pop ebx                ; restore kernel data segment
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 0x8            ; restore stack for error not been pushed

    sti                     ; re-enable interrupts
    iret

mod_done:
    ; The modified done label
    pop eax                ; Restore all registers except EAX

    pop ebx                ; restore kernel data segment
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 0x8            ; restore stack for error not been pushed

    sti                     ; re-enable interrupts
    iret
%macro IRQ 2
  global irq_%1
  irq_%1:
    cli
    push byte 0
    push byte %2
    jmp irq_handler
%endmacro


IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47
IRQ 16, 48
IRQ 17, 49
