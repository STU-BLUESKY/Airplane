#include "timer.h"
#include "mpu6050.h"
#include "pid.h"
#include "control.h"
#include "pwm.h"
#include "Altitude.h"

uint8_t  loop200HzFlag=0, loop100HzFlag=0, loop50HzFlag=0, loop10HzFlag=0;
volatile uint16_t loop200HzCnt=0, loop100HzCnt=0, loop50HzCnt=0, loop10HzCnt=0;

/*-----------------------------------------------
 ��������: TIM3��ʱ��ΪPID���������ṩ�ȶ��ж�
 ��������: ARR�Ĵ���ֵ0-65535,Ԥ��Ƶֵ0-65535
 �� �� ֵ: TIM3_Config(999,71)
					 ����Ƶ��1MHz,�ж�Ƶ��1000Hz
				   ������ÿ1us��1,�ж�ÿ1ms����һ��		  			  
-----------------------------------------------*/
void TIM3_Config(unsigned short int Period,unsigned short int Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	
	TIM_TimeBaseStructure.TIM_Period = Period;			        //�Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; 			//ʱ��Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	    //ʱ�ӷ�Ƶ1
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);  					
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	    //��Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	            
	NVIC_Init(&NVIC_InitStructure);		
	
	TIM_Cmd(TIM3,ENABLE);	
}
/*-----------------------------------------------
 ��������:TIM3�жϷ������
 ����˵��:ÿ1ms����һ���ж�,�ж�Ƶ��1000Hz
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
	
	TIM_TimeBaseStructure.TIM_Period = Period;			        //�Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; 			//ʱ��Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	    //ʱ�ӷ�Ƶ1
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);  					
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	    //��Ӧ���ȼ�0
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

