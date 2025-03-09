#ifndef CONFIG_H_
#define CONFIG_H_

#define GD32F303CCT6

/* Notable Flash addresses */

//For GD32F30x_CL with flash no more than 512KB and GD32F30x_HD, the page size is 2KB.
//For GD32F30x_CL and GD32F30x_XD, bank0 is used for the first 512KB where the page size is 2KB
#define FLASH_PAGE_SIZE                   ((uint32_t)2048)
#define APPLICATION_ADDRESS               (uint32_t)0x08002000      /* Start user code address: TBD */
#define FLASH_START		                    ((uint32_t)0x08000000)

#ifdef GD32F303CGT6
    #define FLASH_END_ADDRESS             ((uint32_t)0x00100000)  //1024Kb
    #define USER_FLASH_END_ADDRESS        ((uint32_t)0x00100000)  //Only used by FLASH_If_WriteProtectionConfig, end protection address

    /* Define the user application size */
    #define USER_FLASH_SIZE               ((uint32_t)0x00080000) /* Small default template application */
#endif

#ifdef GD32F303CCT6
    #define FLASH_END_ADDRESS             ((uint32_t)0x08040000) //256Kb
    #define USER_FLASH_END_ADDRESS        ((uint32_t)0x08040000) //Only used by FLASH_If_WriteProtectionConfig, end protection address

    /* Define the user application size */
    #define USER_FLASH_SIZE               ((uint32_t)0x00040000) /* Small default template application */
#endif



#endif
