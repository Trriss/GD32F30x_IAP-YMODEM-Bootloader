/*
 * flash_if.c
 *
 *  Created on: 25 July 2023
 *      Author: Akash Virendra
 */

#include <stdint.h>
#include "flash_if.h"
#include "config.h"
#include "gd32f30x_fmc.h"

#define OK      0

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */

#define FLASH_DOUBLEWORD_SIZE 8

void FLASH_If_Init(void) {
  // Unlock flash memory for writing
  fmc_unlock();

  // Clear flash flags
  fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR | FMC_FLAG_BANK1_END | FMC_FLAG_BANK1_WPERR | FMC_FLAG_BANK1_PGERR);
  
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  start: start of user flash area
  * @retval FLASHIF_OK : user flash area successfully erased
  *         FLASHIF_ERASEKO : error occurred
  */
uint32_t FLASH_If_Erase(uint32_t start) {
  uint32_t result = FLASHIF_OK;
  //uint32_t page_num;

  // Unlock flash to enable the flash control register access
  FLASH_If_Init();

  /* NOTE: Following implementation expects the IAP code address to be < Application address */
  // Check valid erase range
  if (start < FLASH_END_ADDRESS) {
    // Calculate number of pages to erase
    uint32_t pages_to_erase = (FLASH_END_ADDRESS - start) / FLASH_PAGE_SIZE;

    // Erase pages
    for (uint32_t i = 0; i < pages_to_erase; i++) {
      if (fmc_page_erase(start + (i * FLASH_PAGE_SIZE)) != FMC_READY) {
        result = FLASHIF_ERASEKO;
        break;
      }
    }
  } else {
    result = FLASHIF_ERASEKO;
  }

  // Lock flash
  fmc_lock();

  return result;
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  destination: start address for target location
  * @param  p_source: pointer on buffer with data to write
  * @param  length: length of data buffer (unit is 32-bit word)
  * @retval uint32_t 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
uint32_t FLASH_If_Write(uint32_t destination, uint32_t *p_source, uint32_t length) {
  uint32_t status = FLASHIF_OK;
  uint32_t i = 0;

  fmc_unlock();

  for (i = 0; i < length && (destination <= (USER_FLASH_END_ADDRESS-4)); i++) {
    if (fmc_word_program(destination, p_source[i]) == FMC_READY) {
      if (*(uint32_t*)destination != p_source[i]) {
        status = FLASHIF_WRITINGCTRL_ERROR;
        break;
      }
      destination += 4;
    } else {
      status = FLASHIF_WRITING_ERROR;
      break;
    }
  }

  fmc_lock();
  return status;
}

