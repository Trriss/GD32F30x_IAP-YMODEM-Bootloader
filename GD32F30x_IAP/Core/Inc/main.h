#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>

#define     __IO    volatile

//check bootloader size and adapt
#define USER_FLASH_BANK0_FIRST_PAGE_ADDRESS 0x0801000

typedef void (*pFunction)(void);

void jump_to_application(uint32_t app_start_address);
static void run_bootloader(void);

#endif /* __MAIN_H */