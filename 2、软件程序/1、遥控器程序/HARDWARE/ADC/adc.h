#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

#define ROCKER_UP 		1
#define ROCKER_RIGHT  2
#define ROCKER_DOWN  	3
#define ROCKER_LEFT   4
#define ROCKER_CENTER 5

extern u8 L_Rocker,R_Rocker;
extern u8 L_Center, R_Center;
extern u16 LX_adc, LY_adc, RX_adc, RY_adc, POWER_adc;

#define get_LX_adc() Get_Adc_Average(ADC_Channel_3,1)		//PA3
#define get_LY_adc() Get_Adc_Average(ADC_Channel_8,1)		//PB0 
#define get_RX_adc() Get_Adc_Average(ADC_Channel_2,1)		//PA2 
#define get_RY_adc() Get_Adc_Average(ADC_Channel_1,1)		//PA1
#define get_POWER_adc() Get_Adc_Average(ADC_Channel_4,1)	//PA4
	
void Adc_Init(void);	//定义
u16  Get_Adc(u8 ch ); 	//采集
u16 Get_Adc_Average(u8 ch, u8 times);
void Rocker_scan(void);

#endif 
