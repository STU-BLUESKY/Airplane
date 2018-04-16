#include "timer.h"
#include "mpu6050.h"
#include "pid.h"
#include "control.h"
#include "pwm.h"
#include "Altitude.h"

uint8_t  loop200HzFlag=0, loop100HzFlag=0, loop50HzFlag=0, loop10HzFlag=0;
volatile uint16_t loop200HzCnt=0, loop100HzCnt=0, loop50HzCnt=0, loop10HzCnt=0;

/*-----------------------------------------------
 函数功能: TIM3定时器为PID采样计算提供稳定中断
 函数参数: ARR寄存器值0-65535,预分频值0-65535
 参 考 值: TIM3_Config(999,71)
					 计数频率1MHz,中断频率1000Hz
				   计数器每1us加1,中断每1ms产生一次		  			  
-----------------------------------------------*/
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	    //响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	            
	NVIC_Init(&NVIC_InitStructure);		
	
	TIM_Cmd(TIM3,ENABLE);	
}
/*-----------------------------------------------
 函数功能:TIM3中断服务程序
 函数说明:每1ms进入一次中断,中断频率1000Hz
-----------------------------------------------*/
void TIM3_IRQHandler(void)   
{
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{	
		
		loop200HzCnt++;
		loop100HzCnt++;
		loop50HzCnt++;
		loop10HzCnt++;
		
		if( loop200HzCnt*200 >= 200 )
		{
			loop200HzCnt=0;
			loop200HzFlag=1;
		}	
		if( loop100HzCnt*100 >= 200)
		{		
			loop100HzCnt=0;
			loop100HzFlag=1;			
		}
		if( loop50HzCnt*50 >= 200)
		{
			loop50HzCnt=0;
			loop50HzFlag=1;
		}	
		if( loop10HzCnt*10 >= 200)
		{
			loop10HzCnt=0;
			loop10HzFlag=1;
		}
		
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	
	}
	
}


void TIM2_Config(unsigned short int Period,unsigned short int Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	
	TIM_TimeBaseStructure.TIM_Period = Period;			        //自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; 			//时钟预分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	    //时钟分频1
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);  					
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	    //响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	            
	NVIC_Init(&NVIC_InitStructure);		
	
	TIM_Cmd(TIM2,ENABLE);	
}


void TIM2_IRQHandler(void)   
{
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{	
		Get_Attitude();		
		//RCWL0801_Get_Altitude();	
		//detecZspeed();
		//Altitude_PID(altitude);		
		Throttle = basethrottle;
		Cascade_PID_Calc(EulerAngle.Pitch,EulerAngle.Roll,EulerAngle.Yaw);
		if(fly_allow) Motor_Move(	Motor_PWM.M1,	Motor_PWM.M2,	Motor_PWM.M3,	Motor_PWM.M4);
		else Motor_Move(0,0,0,0);	
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	
	}
	
}

