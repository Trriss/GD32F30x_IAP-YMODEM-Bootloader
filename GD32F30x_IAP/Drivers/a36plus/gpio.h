#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

void gpio_start(void);
void gpio_reset(void);
uint8_t gpio_check_buttons_pressed(void);

#endif
