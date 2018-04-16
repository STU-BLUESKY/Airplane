#ifndef _MS5611_H_
#define _MS5611_H_

#include "stm32f10x_conf.h"
#include "usart.h"
#include "IIC.h"
#include  "delay.h"
/*º¯ÊýÉùÃ÷----------------------------------------------------------------*/
 void MS561101BA_Reset(void);
 void MS561101BA_readPROM(void);
 uint32_t MS561101BA_DO_CONVERSION(u8 command);
 void MS561101BA_GetTemperature(u8 OSR_Temp);
 void MS561101BA_GetPressure(u8 OSR_Pres);
 void MS561101BA_Init(void);
 void SampleANDExchange(void);
 
 #endif
 
 
 
 
 
 
 
 
 
