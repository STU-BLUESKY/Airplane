#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define	PWM_ARR	(1000-1)
#define PWM_PSC	(72-1)
#define POWER_MAX (850)	

void TIM_1_Int_Init(u16 arr,u16 psc);
void PWM_Init(u16 arr,u16 psc);
void M1_SetSpeed(int val);
void M2_SetSpeed(int val);
void M3_SetSpeed(int val);
void M4_SetSpeed(int val);
void Motor_Move(int pwm1,int pwm2, int pwm3, int pwm4);

#endif
