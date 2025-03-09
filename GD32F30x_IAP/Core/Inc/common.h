
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Constants used by Serial Command Line Mode */
#define TX_TIMEOUT          ((uint32_t)100)
#define RX_TIMEOUT          0xFFFFFFFF

/* Exported macro ------------------------------------------------------------*/
#define IS_CAP_LETTER(c)    (((c) >= 'A') && ((c) <= 'F'))
#define IS_LC_LETTER(c)     (((c) >= 'a') && ((c) <= 'f'))
#define IS_09(c)            (((c) >= '0') && ((c) <= '9'))
#define ISVALIDHEX(c)       (IS_CAP_LETTER(c) || IS_LC_LETTER(c) || IS_09(c))
#define ISVALIDDEC(c)       IS_09(c)
#define CONVERTDEC(c)       (c - '0')

#define CONVERTHEX_ALPHA(c) (IS_CAP_LETTER(c) ? ((c) - 'A'+10) : ((c) - 'a'+10))
#define CONVERTHEX(c)       (IS_09(c) ? ((c) - '0') : CONVERTHEX_ALPHA(c))

/* Exported functions ------------------------------------------------------- */
void uint2strn(uint8_t *p_str, uint8_t maxlen, uint32_t intnum);
uint32_t Str2Int(uint8_t *inputstr, uint32_t *intnum);
void delayMs(unsigned int mseconds);


#endif  /* __COMMON_H */

