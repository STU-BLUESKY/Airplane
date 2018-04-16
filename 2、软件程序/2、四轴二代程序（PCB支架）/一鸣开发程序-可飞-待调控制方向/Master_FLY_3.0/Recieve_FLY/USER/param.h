#ifndef _PARAM_H_
#define _PARAM_H_
/* Includes ------------------------------------------------------------------*/
#include "sys.h"

#define NumbOfVar               ((uint8_t)15)

extern uint16_t VirtAddVarTab[15];

void EE_SAVE_ACC_OFFSET(void);

void paramLoad(void);

#endif /* __EEPROM_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

