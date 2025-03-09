/*
 * common.c
 *
 *  Created on: 25 July 2023
 *      Author: Akash Virendra
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "common.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void generate_delay(uint16_t count);

/**
  * @brief  Convert an Integer to a string
  * @param  p_str: The string output pointer
  * @param  intnum: The integer to be converted
  * @retval None
  */
void uint2strn(uint8_t *p_str, uint8_t maxlen, uint32_t intnum) {

  if (0 == p_str || maxlen < 2) {
    return;
  }
  
  if (intnum == 0) {    // Special case for zero
    p_str[0] = '0';
    p_str[1] = '\0';
    return;
  }
  
  uint32_t divider = 1000000000;
  uint8_t pos = 0;

  for (uint8_t i = 0; i < 10 && pos < (maxlen - 1) ; i++) {
    uint8_t digit = (uint8_t)((intnum / divider) + 48);
    intnum = intnum % divider;
    divider /= 10;
    
    if ((digit == '0') && (pos == 0))
    {
      // Skip leading zeros
    }
    else
    {
        p_str[pos++] = digit;
    }
  }
  
  if (pos < maxlen) {
      p_str[pos] = '\0';
  } else {
      p_str[maxlen - 1] = '\0';
  }
  
}

/**
  * @brief  Convert a string to an integer
  * @param  p_inputstr: The string to be converted
  * @param  p_intnum: The integer value
  * @retval 1: Correct
  *         0: Error
  */
uint32_t Str2Int(uint8_t *p_inputstr, uint32_t *p_intnum)
{
  uint32_t i = 0, res = 0;
  uint32_t val = 0;

  if ((p_inputstr[0] == '0') && ((p_inputstr[1] == 'x') || (p_inputstr[1] == 'X')))
  {
    i = 2;
    while ( ( i < 11 ) && ( p_inputstr[i] != '\0' ) )
    {
      if (ISVALIDHEX(p_inputstr[i]))
      {
        val = (val << 4) + CONVERTHEX(p_inputstr[i]);
      }
      else
      {
        /* Return 0, Invalid input */
        res = 0;
        break;
      }
      i++;
    }

    /* valid result */
    if (p_inputstr[i] == '\0')
    {
      *p_intnum = val;
      res = 1;
    }
  }
  else /* max 10-digit decimal input */
  {
    while ( ( i < 11 ) && ( res != 1 ) )
    {
      if (p_inputstr[i] == '\0')
      {
        *p_intnum = val;
        /* return 1 */
        res = 1;
      }
      else if (((p_inputstr[i] == 'k') || (p_inputstr[i] == 'K')) && (i > 0))
      {
        val = val << 10;
        *p_intnum = val;
        res = 1;
      }
      else if (((p_inputstr[i] == 'm') || (p_inputstr[i] == 'M')) && (i > 0))
      {
        val = val << 20;
        *p_intnum = val;
        res = 1;
      }
      else if (ISVALIDDEC(p_inputstr[i]))
      {
        val = val * 10 + CONVERTDEC(p_inputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0;
        break;
      }
      i++;
    }
  }

  return res;
}

//Needs to be calibrated
void delayMs(uint32_t mseconds) {
    register const uint16_t count=23125-7725;

    for(unsigned int i=0;i<mseconds;i++)
    {
        generate_delay(count);
    }
}


static __attribute__((noinline)) void generate_delay(uint16_t count) {
        // This delay has been calibrated to take 1 millisecond
        // It is written in assembler to be independent on compiler optimization
        #ifdef __ARMCC_VERSION 
        __asm volatile("           mov   r1, #0     \n"
        #else
        asm volatile("           mov   r1, #0     \n"
        #endif
                     "___loop_gen_delay:              \n"
                     "           cmp   r1, %0     \n"
                     "           itt   lo         \n"
                     "           addlo r1, r1, #1 \n"
                     "           blo   ___loop_gen_delay  \n"::"r"(count):"r1");
}