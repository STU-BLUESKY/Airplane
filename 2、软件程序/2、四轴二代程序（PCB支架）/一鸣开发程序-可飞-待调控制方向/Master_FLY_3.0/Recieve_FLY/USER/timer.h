#ifndef __TIME_H__
#define __TIME_H__
#include "stm32f10x.h"

#define EnTIM3()  TIM_Cmd(TIM3,ENABLE)
#define DISEnTIM3()  TIM_Cmd(TIM3,DISABLE)

void Nvic_Init(void);
void TIM3_Init(u16 period_num);

#endif 
