/*
 * gpio.c
 *
 * https://github.com/Trriss/GD32F30x_IAP-YMODEM-Bootloader
 */

#include "gpio.h"
#include "gd32f30x_gpio.h"
#include "gd32f30x_rcu.h"
#include "systick.h"
#include "stdint.h"


void gpio_start(void) {
    // Enable GPIOA and GPIOB clocks
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

    // Configure PB9 (KBD_DB2/KEY S1) and PB14 (KBD_DB1/KEY S2) as input with pull-up
    gpio_init((uint32_t)GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, GPIO_PIN_10);
    gpio_init((uint32_t)GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, GPIO_PIN_9 | GPIO_PIN_14);
}


void gpio_reset(void) {
    // Disable GPIOA and GPIOB clocks
    rcu_periph_clock_disable(RCU_GPIOA);
    rcu_periph_clock_disable(RCU_GPIOB);

    //Reset to default state
    gpio_init((uint32_t)GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, GPIO_PIN_ALL);
    gpio_init((uint32_t)GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_2MHZ, GPIO_PIN_ALL);
}


//check (bootloader) buttons are pressed + debounce
uint8_t gpio_check_buttons_pressed(void)
{
    if (!gpio_input_bit_get((uint32_t)GPIOB, (GPIO_PIN_9)) && !gpio_input_bit_get((uint32_t)GPIOB, (GPIO_PIN_15))) {
        // Both buttons are pressed
        // Introduce a short delay (50 ms)
        delay_ms(50);
        // Recheck if buttons are still pressed after short delay
        if (!gpio_input_bit_get((uint32_t)GPIOB, (GPIO_PIN_9)) && !gpio_input_bit_get((uint32_t)GPIOB, (GPIO_PIN_15))) {
            // Buttons are still pressed after delay
            return 1;
        }
    }
    return 0;
}

