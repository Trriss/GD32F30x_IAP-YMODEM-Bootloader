/*
 * menu.c
 *
 *  Created on: 25 July 2023
 *      Author: Akash Virendra
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart0.h"
#include "common.h"
#include "flash_if.h"
#include "menu.h"
#include "ymodem.h"
#include "gd32f30x_usart.h"
#include "gd32f30x.h"
#include "gpio.h"
#include "config.h"
#include <stdint.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static pFunction JumpToApplication;
static uint32_t JumpAddress;
static uint32_t FlashProtection = 0;
uint8_t aFileName[FILE_NAME_LENGTH];

/* Private function prototypes -----------------------------------------------*/
void SerialDownload(void);
void SerialUpload(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
void SerialDownload(void)
{
  uint8_t number[12] = {0};
  uint32_t size = 0;
  COM_StatusTypeDef result;

  Serial_PutString((uint8_t *)"Waiting for the file to be sent ... (press 'a' to abort)\n\r", NAK_TIMEOUT);
  result = Ymodem_Receive( &size );
  if (result == COM_OK)
  {
    Serial_PutString((uint8_t *)"\n\n\r Programming Completed Successfully!\n\r------------------------\r\n Name: ", NAK_TIMEOUT);
    Serial_PutString((uint8_t *)aFileName, NAK_TIMEOUT);
    uint2strn(number, sizeof(number), size);
    Serial_PutString((uint8_t *)"\n\r Size: ", NAK_TIMEOUT);
    Serial_PutString((uint8_t *)number, NAK_TIMEOUT);
    Serial_PutString((uint8_t *)" Bytes\r\n", NAK_TIMEOUT);
    Serial_PutString((uint8_t *)"-------------------\n", NAK_TIMEOUT);
  }
  else if (result == COM_LIMIT)
  {
    Serial_PutString((uint8_t *)"\n\n\rThe image size is higher than the allowed space memory!\n\r", NAK_TIMEOUT);
  }
  else if (result == COM_DATA)
  {
    Serial_PutString((uint8_t *)"\n\n\rVerification failed!\n\r", NAK_TIMEOUT);
  }
  else if (result == COM_ABORT)
  {
    Serial_PutString((uint8_t *)"\r\n\nAborted by user.\n\r", NAK_TIMEOUT);
  }
  else
  {
    Serial_PutString((uint8_t *)"\n\rFailed to receive the file!\n\r", NAK_TIMEOUT);
  }
}

/**
  * @brief  Upload a file via serial port.
  * @param  None
  * @retval None
  */
void SerialUpload(void)
{
    Serial_PutString((uint8_t *)"\n\n\rSelect Receive File\n\r", NAK_TIMEOUT);

    uint8_t status = 0;
    usart0_receive(&status, 1, NAK_TIMEOUT);

    if ( status == CRC16)
    {
        /* Transmit the flash image through ymodem protocol */
        status = Ymodem_Transmit((uint8_t*)APPLICATION_ADDRESS, (const uint8_t*)"A36plusBackupImage.bin", USER_FLASH_SIZE);

      if (status != 0)
      {
          Serial_PutString((uint8_t *)"\n\rError Occurred while Transmitting File\n\r", NAK_TIMEOUT);
      }
      else
      {
          Serial_PutString((uint8_t *)"\n\rFile uploaded successfully \n\r", NAK_TIMEOUT);
      }
    }
}

/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
void Main_Menu(void)
{

  Serial_PutString((uint8_t *)"\r\n=========================================================", NAK_TIMEOUT);
  Serial_PutString((uint8_t *)"\r\n=    A36Plus Bootloader                                 =", NAK_TIMEOUT);
  Serial_PutString((uint8_t *)"\r\n=========================================================", NAK_TIMEOUT);
  Serial_PutString((uint8_t *)"\r\n\r\n", NAK_TIMEOUT);

  uint8_t warn_wrong_key_once = 0;
  while (1)
  {

    Serial_PutString((uint8_t *)"\r\n=================== Main Menu ===========================\r\n\n", NAK_TIMEOUT);
    Serial_PutString((uint8_t *)    "  Download image to MCU Flash (Install Firmware)------- 1\r\n\n", NAK_TIMEOUT);
    Serial_PutString((uint8_t *)    "  Upload image from MCU Flash (Backup  Firmware) ------ 2\r\n\n", NAK_TIMEOUT);
    Serial_PutString((uint8_t *)    "  Execute the installed application ------------------- 3\r\n\n", NAK_TIMEOUT);
    Serial_PutString((uint8_t *)"\r\n=========================================================\r\n\n", NAK_TIMEOUT);


    /* Clean the input path */
    usart_flag_clear(USART0, USART_FLAG_RBNE);
    usart_flag_clear(USART0, USART_FLAG_ORERR);

    /* Receive key */

    uint8_t key = 0;
    usart0_receive(&key, 1, RX_TIMEOUT);

    switch (key)
    {
    case '1' :
      /* Download user application in the Flash */
      SerialDownload();
      break;
    case '2' :
      /* Upload user application from the Flash */
      SerialUpload();
      break;
    case '3' :
      Serial_PutString((uint8_t *)"Start program execution......\r\n\n", NAK_TIMEOUT);
      jump_to_application((uint32_t) APPLICATION_ADDRESS);
      break;
    default:
      if (!warn_wrong_key_once) {
          Serial_PutString((uint8_t *)"Invalid Number ! ==> The number should be either 1, 2 or 3\r", NAK_TIMEOUT);
          warn_wrong_key_once = 1;
      }
      break;
    }
  }
}

