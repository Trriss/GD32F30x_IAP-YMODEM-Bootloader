/*
 * systick.c
 *
 * https://github.com/Trriss/GD32F30x_IAP-YMODEM-Bootloader
 */

#include "gd32f30x.h"
#include "systick.h"

void SysTick_start(void) {
    uint32_t ticks = TICKS_FOR_THIS_MCU;
    SysTick->LOAD = ticks - 1;  // Load the value into the SysTick Reload Register
    SysTick->VAL = 0;           // Clear the current value register
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;  // Enable SysTick and interrupt
}


void SysTick_Disable(void) {
    SysTick->CTRL = 0;                          // Disable SysTick, CTRL To default value
    SysTick->VAL = 0;                           // Clear
    SysTick->LOAD = 0xFFFFFF;                   // Clear the register to default
    SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;        //Clear exception pending bit
}


static volatile uint32_t sysTickCounter = 0;
volatile uint32_t delay = 0;

void SysTick_Handler(void) {
    if (delay) {
        delay--;
    }
    sysTickCounter++;  // Increment the counter every 1 ms
}

// Get the current SysTick counter value
uint32_t get_systick(void) {
    return sysTickCounter;
}

void delay_ms(uint32_t ms){
    delay = ms;
    while(0U != delay)
    {
    }
}