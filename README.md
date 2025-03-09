# GD32 in-application programming (IAP) using the USART via YMODEM Protocol
 In-application programming (IAP) is a method of updating firmware in the end product using a custom bootloader stored in application space.
## Example
This example is based on the [X-CUBE-IAP-USART](https://www.st.com/en/embedded-software/x-cube-iap-usart.html) Firmware Package designed to work for STM32G073Z_EVAL, STM32L476G_EVAL and STM3210C_EVAL boards.
This firmware package is ported to work on Gigadevice GD32F0x devices. 


## Hardware and Tools required
- Device with gd32f30x mcu, this specific example in the main repo is configured for Talkpod A36
- Teraterm(v4.105 or higher) is recommended, although any terminal emulator software can be used.
- KEIL MDK (Free Non-Commercial Licence is sufficient: https://www.keil.arm.com/mdk-community/, unless for Commercial Use)

## Description of Core directory 
This directory contains a set of sources and header files for an IAP bootloader using USART via YMODEM protocol for flashing/reading firmware.

**IAP implementation**
(1) User application location address is defined in the configure.h file as: 
#define APPLICATION_ADDRESS           ((uint32_t)0x08008000) <br>
To modify it, change the default value to the desired one. Note that the application must be linked
relatively to the new address too.

(2)The function gpio_check_buttons_pressed(void) in gpio.c handles the logic for detecting button combinations that trigger the bootloader. You may modify this function, and the gpio_start(void) and gpio_reset(void) functions will also need to be updated to use different pins.

**USART AND TERATERM CONFIGURATIONS**
- Word Length = 8 Bits
- One Stop Bit
- No parity
- BaudRate = 115200 baud
- flow control: None 
- Ymodem protocol is using CRC16 by default. To switch to checksum, comment #define CRC16_F in ymodem.c
