#ifndef USART0_H
#define USART0_H


#include <stdint.h>


void usart0_start(void);
void usart0_reset(void);
uint8_t usart0_receive(uint8_t* p_data, uint32_t packet_size, uint32_t timeout_ms);
uint8_t usart0_transmit(uint8_t* p_data, uint32_t packet_size, uint32_t timeout_ms);
uint8_t Serial_PutByte(uint8_t param, uint32_t timeout_ms);
uint8_t Serial_PutString(uint8_t *p_string, uint32_t timeout_ms);
void usart0_clear_error(void);

#endif //USART0_H
