#include "led.h"
#include "pwm.h"

//初始化通用定时器
void TIM_1_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//定时器初始化
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	//初始化PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;									//选择PWM模式1 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;			//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;					//输出极性高
	
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;        //死区后输出状态         
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//死区后互补端输出状态         
 
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);
	
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;//运行模式下输出
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;//空闲模式下输出选择          
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;         //锁定设置        
  TIM_BDTRInitStructure.TIM_DeadTime = 0x90;                                         //死区时间设置         
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                 //刹车功能使能         
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;//刹车输入极性        
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//自动输出使能          
  TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
	
	//使能预装载寄存器
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);                //使能TIMx在ARR上的预装载寄存器  
	TIM_Cmd(TIM1,ENABLE);

}


/*----------------------------------------------------
	函数功能：PWM输出初始化
----------------------------------------------------*/
void PWM_Init(u16 arr,u16 psc)
{
		TIM_1_Int_Init(arr, psc);			
}

/*------------------------------------------------
	函数功能：电机1驱动
	函数参数：val设置PWM占空比
--------------------------------------------------*/
void M1_SetSpeed(int val)
{
	if( val>POWER_MAX) val= POWER_MAX;
	else if( val<0) val= 0;
	TIM_SetCompare2(TIM1,val);
}
/*------------------------------------------------
	函数功能：电机2驱动
	函数参数：val设置PWM占空比
--------------------------------------------------*/
void M2_SetSpeed(int val)
{
	if( val>POWER_MAX) val= POWER_MAX;
	else if( val<0) val= 0;
	TIM_SetCompare1(TIM1,val);
}
/*------------------------------------------------
	函数功能：电机3驱动
	函数参数：val设置PWM占空比
--------------------------------------------------*/
void M3_SetSpeed(int val)
{
	if( val>POWER_MAX) val= POWER_MAX;
	else if( val<0) val= 0;
	TIM_SetCompare4(TIM1,val);
}
/*------------------------------------------------
	函数功能：电机4驱动
	函数参数：val设置PWM占空比
--------------------------------------------------*/
void M4_SetSpeed(int val)
{
	if( val>POWER_MAX) val= POWER_MAX;
	else if( val<0) val= 0;
	TIM_SetCompare3(TIM1,val);
} 


/*-------------------------------
	函数功能：输出PWM
-------------------------------*/

void Motor_Move(int pwm1,int pwm2, int pwm3, int pwm4)
{			
		M1_SetSpeed(pwm1);
		M2_SetSpeed(pwm2);
		M3_SetSpeed(pwm3);
		M4_SetSpeed(pwm4);	
}
