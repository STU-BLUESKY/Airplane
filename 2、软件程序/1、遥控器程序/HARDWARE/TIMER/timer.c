#include "timer.h"

void TIM3_Config(unsigned short int Period,unsigned short int Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	
	TIM_TimeBaseStructure.TIM_Period = Period;			        //自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; 			//时钟预分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	    //时钟分频1
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  					
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	    //响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	            
	NVIC_Init(&NVIC_InitStructure);		

	//TIM_Cmd(TIM3,ENABLE);  //不用
}


void TIM3_IRQHandler(void)   
{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);		//不用
}





