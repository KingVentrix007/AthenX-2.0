
/**
 * @file timer.c
 * @brief This file implements low-level timer functionality.
 *
 * Detailed description of the contents and purpose of the file.
 *
 * @author Tristan Kuhn
 * @date 2023-11-28
 */
#include "../include/timer.h"
#include "../include/debug.h"
#include "../include/console.h"
#include "../include/idt.h"
#include "../include/io_ports.h"
#include "../include/isr.h"
#include "../include/string.h"
#include "../include/types.h"

// number of ticks since system booted
uint32 g_ticks = 0;
// frequency in hertz
uint16 g_freq_hz = 0;
// timer functions to be called when that ticks reached in irq handler
TIMER_FUNCTION_MANAGER g_timer_function_manager;
 cmos_address = 0x70;
cmos_data    = 0x71;

unsigned char get_RTC_register(int reg) {
    outportb(cmos_address, reg);
    return inportb(cmos_data);
}

// See https://wiki.osdev.org/Programmable_Interval_Timer
void timer_set_frequency(uint16 f) {
    FUNC_ADDR_NAME(&timer_set_frequency,1,"i");
    g_freq_hz = f;
    //int error_fake = 1/0;
    uint16 divisor = TIMER_INPUT_CLOCK_FREQUENCY / f;
    // set Mode 3 - Square Wave Mode
    outportb(TIMER_COMMAND_PORT, 0b00110110);
    // set low byte
    outportb(TIMER_CHANNEL_0_DATA_PORT, divisor & 0xFF);
    // set high byte
    outportb(TIMER_CHANNEL_0_DATA_PORT, (divisor >> 8) & 0xFF);
}


/**
 * @brief Timer handler function.
 *
 * This function is the handler for the timer interrupt. It is called whenever the timer interrupt is triggered.
 *
 * @param r Pointer to the register state at the time of the interrupt.
 *
 * @details The function performs the following steps:
 * 1. Calls FUNC_ADDR_NAME() macro to log the function address and name for debugging purposes.
 * 2. Increments the global variable g_ticks to keep track of the number of timer interrupts.
 * 3. Iterates through the array of TIMER_FUNC_ARGS structures to check if any timer functions need to be executed.
 * 4. If the timeout value of a TIMER_FUNC_ARGS structure is 0, it means the timer function is not active, so the loop continues to the next iteration.
 * 5. If the timeout value is not 0 and the current value of g_ticks is a multiple of the timeout value, the corresponding timer function is executed.
 *
 * @note The timer functions are stored in the g_timer_function_manager.functions array, and the corresponding arguments are stored in the g_timer_function_manager.func_args array.
 * The maximum number of timer functions is defined by MAXIMUM_TIMER_FUNCTIONS.
 */
void timer_handler(REGISTERS* r) {
    FUNC_ADDR_NAME(&timer_handler, 1, "T");

    uint32 i;
    TIMER_FUNC_ARGS *args = NULL;
    g_ticks++;

    //printf("timer triggered at frequency %d\n", g_ticks);

    for (i = 0; i < MAXIMUM_TIMER_FUNCTIONS; i++) {
        args = &g_timer_function_manager.func_args[i];

        if (args->timeout == 0)
            continue;

        if ((g_ticks % args->timeout) == 0) {
            g_timer_function_manager.functions[i](args);
        }
    }
}

void timer_register_function(TIMER_FUNCTION function, TIMER_FUNC_ARGS *args) {
    uint32 index = 0;
    if (function == NULL || args == NULL) {
        printf_("ERROR: failed to register timer function %x\n", function);
        return;
    }
    index = (++g_timer_function_manager.current_index) % MAXIMUM_TIMER_FUNCTIONS;
    g_timer_function_manager.current_index = index;
    g_timer_function_manager.functions[index] = function;
    memcpy(&g_timer_function_manager.func_args[index], args, sizeof(TIMER_FUNC_ARGS));
}

void timer_init() {
    FUNC_ADDR_NAME(&timer_init,0,"");
    // IRQ0 will fire 100 times per second
    //printf("0x%x\n",&timer_init);
    
    timer_set_frequency(100);
    isr_register_interrupt_handler(IRQ_BASE, timer_handler);
}

void sleep(double sec) {
    uint32 end = g_ticks + sec * g_freq_hz; 
    while (g_ticks < end);
}

int get_ticks()
{
    return g_ticks;
}
int reset_ticks()
{
    g_ticks = 0;
}

TIME get_time()
{
    int second;
    int minute;
    int hour;
    int day;
    int month;
    int year;
    second = get_RTC_register(0x00);
    minute = get_RTC_register(0x02);
    hour = get_RTC_register(0x04);
    day = get_RTC_register(0x07);
    month = get_RTC_register(0x08);
    year = get_RTC_register(0x09);
    second = (second & 0x0F) + ((second / 16) * 10);
    minute = (minute & 0x0F) + ((minute / 16) * 10);
    hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
    day = (day & 0x0F) + ((day / 16) * 10);
    month = (month & 0x0F) + ((month / 16) * 10);
    year = (year & 0x0F) + ((year / 16) * 10);
    TIME time;
    time.h = hour+2;
    time.m = minute;
    time.s = second;
    time.y = year;
    time.mo = month;
    time.d = day;
    // printf("time: %d/%d/%d\n",day,month,year);
    // printf("time: %d.%d:%d\n",hour+2,minute,second);
    return time;
}