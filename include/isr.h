/**
 * Interrupt Service Routine(ISR) setup
 */

#ifndef ISR_H
#define ISR_H

#include "../include/types.h"

#define NO_INTERRUPT_HANDLERS    256

typedef struct {
    uint32 ds;
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  // pushed by pusha
    uint32 int_no, err_code;                        // interrupt number and error code
    uint32 eip, cs, eflags, useresp, ss;            // pushed by the processor automatically
} REGISTERS;

typedef struct {
    uint16 di;
    uint16 si;
    uint16 bp;
    uint16 sp;
    uint16 bx;
    uint16 dx;
    uint16 cx;
    uint16 ax;
    // segments
    uint16 ds;
    uint16 es;
    uint16 fs;
    uint16 gs;
    uint16 ss;
    uint16 eflags;
} REGISTERS16;

// ISR function prototype
typedef void (*ISR)(REGISTERS *);

/**
 * register given handler to interrupt handlers at given num
 */
void isr_register_interrupt_handler(int num, ISR handler);

void print_registers(REGISTERS *reg, char caller[100]);
/*
 * turn off current interrupt
*/
void isr_end_interrupt(int num);

/**
 * invoke exception routine,
 * being called in exception.asm
 */
void isr_exception_handler(REGISTERS reg);

/**
 * invoke isr routine and send eoi to pic,
 * being called in irq.asm
 */
void isr_irq_handler(REGISTERS *reg);


// defined in exception.asm
extern void exception_0();
extern void exception_1();
extern void exception_2();
extern void exception_3();
extern void exception_4();
extern void exception_5();
extern void exception_6();
extern void exception_7();
extern void exception_8();
extern void exception_9();
extern void exception_10();
extern void exception_11();
extern void exception_12();
extern void exception_13();
extern void exception_14();
extern void exception_15();
extern void exception_16();
extern void exception_17();
extern void exception_18();
extern void exception_19();
extern void exception_20();
extern void exception_21();
extern void exception_22();
extern void exception_23();
extern void exception_24();
extern void exception_25();
extern void exception_26();
extern void exception_27();
extern void exception_28();
extern void exception_29();
extern void exception_30();
extern void exception_31();
extern void exception_128();

// defined in irq.asm
extern void irq_0();
extern void irq_1();
extern void irq_2();
extern void irq_3();
extern void irq_4();
extern void irq_5();
extern void irq_6();
extern void irq_7();
extern void irq_8();
extern void irq_9();
extern void irq_10();
extern void irq_11();
extern void irq_12();
extern void irq_13();
extern void irq_14();
extern void irq_15();
extern void irq_16();
extern void irq_17();
extern void system_call_handler();
// IRQ default constants
#define IRQ_BASE            0x20
#define IRQ0_TIMER          0x00
#define IRQ1_KEYBOARD       0x01
#define IRQ2_CASCADE        0x02
#define IRQ3_SERIAL_PORT2   0x03
#define IRQ4_SERIAL_PORT1   0x04
#define IRQ5_RESERVED       0x05
#define IRQ6_DISKETTE_DRIVE 0x06
#define IRQ7_PARALLEL_PORT  0x07
#define IRQ8_CMOS_CLOCK     0x08
#define IRQ9_CGA            0x09
#define IRQ10_RESERVED      0x0A
#define IRQ11_RESERVED      0x0B
#define IRQ12_AUXILIARY     0x0C
#define IRQ13_FPU           0x0D
#define IRQ14_HARD_DISK     0x0E
#define IRQ15_RESERVED      0x0F

void TraceStackTrace(unsigned int MaxFrames,uintptr_t ebp_in);
void inspect_stack_frame(int* ebp,int *esp, int total_params, const char* param_types);
void extractLocalVars(int* esp, int* ebp) ;
uint32_t *unwind_stack(REGISTERS *reg,int search_level);
REGISTERS *get_regs();

int my_system_call(uint32_t syscall_number, uint32_t arg1);
int sys_handler(REGISTERS *reg);
int int_print(REGISTERS* reg);
#include <stdint.h>

// Function to set the value of EAX register
void set_EAX(uint32_t value);

// Function to set the value of EBX register
void set_EBX(uint32_t value);

// Function to set the value of ECX register
void set_ECX(uint32_t value);

// Function to set the value of EDX register
void set_EDX(uint32_t value);

// Function to set the value of ESI register
void set_ESI(uint32_t value);

// Function to set the value of EDI register
void set_EDI(uint32_t value);

// Function to get the value of EAX register
uint32_t get_EAX();

// Function to get the value of EBX register
uint32_t get_EBX();

// Function to get the value of ECX register
uint32_t get_ECX();

// Function to get the value of EDX register
uint32_t get_EDX();

// Function to get the value of ESI register
uint32_t get_ESI();

// Function to get the value of EDI register
uint32_t get_EDI();

void IRQ_Enable_Line(unsigned char IRQline);


static inline void io_wait(void);
/**
 * Function Name: init_irq_manager
 * Description: Initialize the IRQ manager and set up the data structures.
 */
void init_irq_manager();

/**
 * Function Name: request_irq
 * Description: Request an IRQ for a device. If the preferred IRQ is available,
 *              assign it; otherwise, find and assign an available IRQ.
 *
 * Parameters:
 *   device_id (int) - The ID of the device requesting the IRQ.
 *   preferred_irq (int) - The preferred IRQ requested by the device.
 *
 * Return:
 *   int - The assigned IRQ.
 */
int request_irq(int device_id, int preferred_irq);


/**
 * Function Name: release_irq
 * Description: Release an IRQ when a device is done using it.
 *
 * Parameters:
 *   irq (int) - The IRQ to release.
 */
void release_irq(int irq);


int reserve_irq(int irq_to_reserve);

int print_used_irq();
#endif
