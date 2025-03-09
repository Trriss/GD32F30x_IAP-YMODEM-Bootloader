/*
 * main.c
 *
 * https://github.com/Trriss/GD32F30x_IAP-YMODEM-Bootloader
 */


#include <stdint.h>
#include "main.h"

#include "gd32f30x_gpio.h"
#include "cmsis_compiler.h"
#include "cmsis_armclang_m.h"
#include "gpio.h"
#include "config.h"
#include "usart0.h"
#include "systick.h"
#include "core_cm4.h"
#include "menu.h"

__attribute__ (( naked, noreturn )) void BootJumpASM (uint32_t SP);

/*!
\brief main function
\param[in] none
\param[out] none
\retval none
*/
int main(void)
{
    /* init modules … */
    gpio_start();
    //Check if side keys are pressed.
    //other conditions, like reading predefined sequence from usart could be used instead.
    //if (1){ //for DEBUG
    if (gpio_check_buttons_pressed()){
        run_bootloader();
    } else {
        jump_to_application((uint32_t) APPLICATION_ADDRESS);
    }
}


static void run_bootloader(void) {
    usart0_start();
    SysTick_start();
    while (1){
        Main_Menu();
    }
}

void jump_to_application(uint32_t app_start_address) {
    if (0x20000000 == ((*(__IO uint32_t*)app_start_address) & 0x2FFE0000)) {
        /* Disable all necessary interrupts */
        //nvic_irq_disable(EXTI0_IRQn);

        __disable_irq();

        usart0_reset();
        SysTick_Disable();
        gpio_reset();

        /* Set the Vector Table Offset Register (VTOR) to the application's base address */
        SCB->VTOR = (uint32_t)app_start_address;
        // _set_MSP and jump to application running at start address
        BootJumpASM(app_start_address);

    } else {
        /* Indicate bad software */
        while(1) {
            /* Halt execution */
        }
    }
}

__attribute__ (( naked, noreturn )) void BootJumpASM (uint32_t SP) {
    __asm("MSR  MSP, r0");
    __asm("LDR  r0, [r0, #4]");
    __asm("BX   r0");
  
}
