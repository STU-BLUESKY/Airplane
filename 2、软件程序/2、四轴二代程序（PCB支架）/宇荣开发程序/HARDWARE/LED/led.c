#include "led.h"
  

//初始化PC13为输出口.并使能这个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 																						 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																						 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);																											 //根据设定参数初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);							 //输出高
	
}
 
void LED_on(void)
{		
		LED1 =0;
		LED2 =0;
		LED3 =0;
		LED4 =0;
}

void LED_off(void)
{
		LED1 =1;
		LED2 =1;
		LED3 =1;
		LED4 =1;
}

void LED_rev(void)
{
		LED1 = !LED1;
		LED2 = !LED2;
		LED3 = !LED3;
		LED4 = !LED4;
}

