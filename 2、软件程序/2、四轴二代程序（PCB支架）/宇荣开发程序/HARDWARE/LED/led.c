#include "led.h"
  

//��ʼ��PC13Ϊ�����.��ʹ������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 																						 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;																						 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);																											 //�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);							 //�����
	
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

