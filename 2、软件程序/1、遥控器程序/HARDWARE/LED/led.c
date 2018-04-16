#include <led.h>
  

//����

//LED IO��ʼ��
//PB8 PB9
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);	 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;		//PB6 PB7
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
 GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);						 

}


void BELL_Init(void)
{
GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);	 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//PB9
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);				
 GPIO_ResetBits(GPIOB,GPIO_Pin_9);	
}





















