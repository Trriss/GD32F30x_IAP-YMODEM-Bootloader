/*!
\brief main function
\param[in] none
\param[out] none
\retval none
*/
//include pinmap?
#define PTT_SW GPIOA, 10
#define KBD_DB2 GPIOB, 9
#define KBD_DB1 GPIOB, 14


int main(void)
{
    /* init modules … */
    
    gpio_init();


    //Check if all three side keys: PTT, S1 and S2 are low (low when pressed).
    uint8_t BootLoadKeys_pressed = 0;
    if (((GPIOA->IDR & GPIO_IDR_IDR10) == 0) &&  // Check PA10 (PTT_SW)
        ((GPIOB->IDR & GPIO_IDR_IDR9) == 0)  &&  // Check PB9 (KBD_DB2)
        ((GPIOB->IDR & GPIO_IDR_IDR14) == 0))    // Check PB14 (KBD_DB1)
    {
        BootLoadKeys_pressed = 1;  // All buttons are pressed (all pins are low)
    }

    if(!BootLoadKeys_pressed){
        //Bootloader keys not pressed, go to our application.
        jump_to_application(uint32_t USER_FLASH_BANK0_FIRST_PAGE_ADDRESS);
    } else {
        
        /* Bootloader realizing codes */
        /* including commands of operating flash */
        ……
        while (1){
            /* Bootloader realizing codes */
        }
    }
}


void jump_to_application(uint32_t app_start_address) {
    if (0x20000000 == ((*(__IO uint32_t*)app_start_address) & 0x2FFE0000)) {
        /* Disable all necessary interrupts */
        //nvic_irq_disable(EXTI0_IRQn);
        gpio_deinit();
        
        /* Get application address and function pointer */
        uint32_t jumpAddress = *(__IO uint32_t*) (app_start_address + 4);
        pFunction start_application = (pFunction) jumpAddress;

        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) app_start_address);
        
        /* Jump to application */
        start_application();
    } else {
        /* Indicate bad software */
        while(1) {
            /* Halt execution */
        }
    }
}

static inline void gpio_init() {
    #ifdef a36plus 
    
    // Enable GPIOA and GPIOB clocks for GD32 and STM32
    RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN);  // Enable GPIOA and GPIOB clocks
    
    //gpio_setMode(PTT_SW,    INPUT_PULL_UP);     //RXD/PTT (PA10)
    
    // Configure PA10 (RXD/PTT) as input with pull-up
    GPIOA->CTL &= ~(GPIO_CTL_MODE10_Msk);  // Clear previous mode
    GPIOA->OCTL |= GPIO_OCTL_ODR10;        // Set pull-up
    
    //gpio_setMode(KBD_DB2, INPUT_PULL_UP);       //DB2/KEY_ S1 (PB9)
    //gpio_setMode(KBD_DB1, INPUT_PULL_UP);       //DB1/KEY_ S2 (PB14)
    
    // Configure PB9 (KBD_DB2/KEY S1) and PB14 (KBD_DB1/KEY S2) as input with pull-up
    GPIOB->CTL &= ~(GPIO_CTL_MODE9_Msk | GPIO_CTL_MODE14_Msk); // Clear previous mode for PB9 and PB14
    GPIOB->OCTL |= (GPIO_OCTL_ODR9 | GPIO_OCTL_ODR14);          // Enable pull-up for PB9 and PB14

    #endif
    
}

static inline void gpio_deinit() {
    #ifdef a36plus 
    
    // Disable GPIOA and GPIOB clocks
    RCC->APB2ENR &= ~(RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN);  // Disable GPIOA and GPIOB clocks

    // Reset GPIOA and GPIOB
    RCC->APB2RSTR |= RCC_APB2RSTR_IOPARST;   // Reset GPIOA
    RCC->APB2RSTR |= RCC_APB2RSTR_IOPBRST;   // Reset GPIOB
    
    //GPIOA and GPIOB reset
    RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPARST;  // Clear GPIOA reset
    RCC->APB2RSTR &= ~RCC_APB2RSTR_IOPBRST;  // Clear GPIOB reset

    #endif
    
}


static inline void usart0_init() {
    #ifdef a36plus
    RCC->APB2ENR |= RCC_APB2ENR_USART0EN;  // Enable USART0 clock

    USART0->BRR = (SystemCoreClock / 115200U); // Assuming SystemCoreClock is the clock frequency
    USART0->CR1 &= ~USART_CR1_M; // 8-bit word length (clear the M bit)
    USART0->CR2 &= ~USART_CR2_STOP;  // 1 stop bit (clear STOP bits)
    USART0->CR1 &= ~USART_CR1_PCE; // No parity (clear the PCE bit)
    USART0->CR1 |= USART_CR1_RE | USART_CR1_TE;  // Enable receiver (RE) and transmitter (TE)
    USART0->CR1 |= USART_CR1_UE;  // Enable USART

    #endif
}





if (USART0->SR & USART_SR_TXE) {
    // USART0 is ready to transmit data
} else {
    // USART0 is not ready to transmit data
}
