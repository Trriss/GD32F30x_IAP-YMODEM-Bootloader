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

**Directory Contents**
- "STM32G0xx_IAP/IAP_Main/Inc": contains the IAP firmware header files 

    - STM32G0xx_IAP/IAP_Main/Inc/main.h              The main include file of the project.
    - STM32G0xx_IAP/IAP_Main/Inc/common.h            This file provides all the headers of the common functions.
    - STM32G0xx_IAP/IAP_Main/Inc/flash_if.h          This file provides all the firmware 
                                                     function headers of the flash_if.c file.
    - STM32G0xx_IAP/IAP_Main/Inc/menu.h              This file provides all the firmware
                                                     function headers of the menu.c file.
    - STM32G0xx_IAP/IAP_Main/Inc/ymodem.h            This file provides all the firmware
                                                     function headers of the ymodem.c file.
    - STM32G0xx_IAP/IAP_Main/Inc/STM32G0xx_hal_conf.h  Library Configuration file
    - STM32G0xx_IAP/IAP_Main/Inc/STM32G0xx_it.h      Header for STM32G0xx_it.c 


- "STM32G0xx_IAP/IAP_Main/Src": contains the IAP firmware source files
    - STM32G0xx_IAP/IAP_Main/Src/main.c              Main program
    - STM32G0xx_IAP/IAP_Main/Src/STM32G0xx_it.c      Interrupt handlers
    - STM32G0xx_IAP/IAP_Main/Src/STM32G0xx_hal_msp.c Microcontroller specific packages
                                                     initialization file.
    - STM32G0xx_IAP/IAP_Main/Src/flash_if.c          The file contains write, erase and disable
                                                     write protection of the internal Flash
                                                     memory.
    - STM32G0xx_IAP/IAP_Main/Src/menu.c              This file contains the menu to select
                                                     downloading a new binary file, uploading
                                                     internal Flash memory, executing the binary
                                                     and disabling the write protection of
                                                     write-protected pages
    - STM32G0xx_IAP/IAP_Main/Src/common.c            This file provides functions related to
                                                     read/write from/to USART peripheral
    - STM32G0xx_IAP/IAP_Main/Src/ymodem.c            This file provides all the firmware functions
                                                     related to the ymodem protocol.
    - STM32G0xx_IAP/IAP_Main/Src/system_STM32G0xx.c  STM32G0xx system source file


**IAP implementation
(1) User application location address is defined in the configure.h file as: 
#define APPLICATION_ADDRESS           ((uint32_t)0x08008000) <br>
To modify it, change the default value to the desired one. Note that the application must be linked
relatively to the new address too.

**USART AND TERATERM CONFIGURATIONS**
- Word Length = 8 Bits
- One Stop Bit
- No parity
- BaudRate = 115200 baud
- flow control: None 
- Ymodem protocol is using CRC16 by default. To switch to checksum, comment #define CRC16_F in ymodem.c
