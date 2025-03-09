/*
 * usart.c
 *
 * https://github.com/Trriss/GD32F30x_IAP-YMODEM-Bootloader
 */

#include <stdint.h>
#include "usart0.h"
#include "gd32f30x_usart.h"
#include "gd32f30x.h"
#include "systick.h"

void usart0_start(void) {

    gpio_pin_remap_config(GPIO_USART0_REMAP, ENABLE);
    gpio_init((uint32_t)GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init((uint32_t)GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);  // RX pin as floating input, or there is no RX
  
    rcu_periph_clock_enable(RCU_USART0);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);

}

void usart0_reset(void) {
    gpio_pin_remap_config(GPIO_USART0_REMAP, DISABLE);    
    rcu_periph_reset_enable(RCU_USART0RST);
    rcu_periph_reset_disable(RCU_USART0RST);
    //GPIO pin reset will be done in jump_to_application by gpio_reset()
}

uint8_t usart0_receive(uint8_t* p_data, uint32_t packet_size, uint32_t timeout_ms) {
    uint32_t start_time = get_systick();
    uint32_t bytes_received = 0;

    while (bytes_received < packet_size) {
        // Check if data is available
        if (usart_flag_get(USART0, USART_FLAG_RBNE) != RESET) {
            p_data[bytes_received++] = usart_data_receive(USART0);
        }

        // Check timeout based on system tick
        if ((get_systick() - start_time) > timeout_ms) {
            return 1;  // Timeout error
        }
    }
    return 0;  // Success
}


uint8_t usart0_transmit(uint8_t* p_data, uint32_t packet_size, uint32_t timeout_ms) {
    uint32_t start_time = get_systick();
    uint32_t bytes_sent = 0;

    while (bytes_sent < packet_size) {
        // Wait for transmit buffer to be empty
        if (usart_flag_get(USART0, USART_FLAG_TBE) != RESET) {
            usart_data_transmit(USART0, p_data[bytes_sent++]);
        }

        // Check actual millisecond timeout
        if ((get_systick() - start_time) > timeout_ms) {
            return 1;  // Timeout error
        }
    }
    return 0;  // Success
}


uint8_t Serial_PutByte(uint8_t param, uint32_t timeout_ms) {
    uint32_t start_time = get_systick();

    // Wait for transmit buffer to be empty
    while (usart_flag_get(USART0, USART_FLAG_TBE) == RESET) {
        if ((get_systick() - start_time) > timeout_ms) {
            return 1;  // Timeout error
        }
    }

    // Transmit byte
    usart_data_transmit(USART0, param);

    // Wait for transmission complete
    start_time = get_systick();
    while (usart_flag_get(USART0, USART_FLAG_TC) == RESET) {
        if ((get_systick() - start_time) > timeout_ms) {
            return 1;  // Timeout error
        }
    }
    return 0;  // Success
}


uint8_t Serial_PutString(uint8_t *p_string, uint32_t timeout_ms) {
    uint16_t length = 0;

    // Calculate string length
    while (p_string[length] != '\0') {
        length++;
    }

    // Use previously created usart0_transmit function
    return usart0_transmit(p_string, length, timeout_ms);
}

void usart0_clear_error(void) {
    (void)usart_data_receive(USART0);
    if(usart_flag_get(USART0, USART_FLAG_ORERR)) {
        // Overrun error handling
        usart_flag_clear(USART0, USART_FLAG_ORERR);
    }
}