#ifndef SYSTICK_H
#define SYSTICK_H

#define TICKS_FOR_THIS_MCU                120000    // 120 MHz / 1000 = 120 ticks per millisecond -> for GD32F303CGT6

#include "stdint.h"

void SysTick_start(void);
void SysTick_Disable(void);
void SysTick_Handler(void);
uint32_t get_systick(void);
void delay_ms(uint32_t ms);

#endif