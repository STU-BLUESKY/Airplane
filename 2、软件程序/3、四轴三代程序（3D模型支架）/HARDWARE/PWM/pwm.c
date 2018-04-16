#include "led.h"
#include "pwm.h"

//��ʼ��ͨ�ö�ʱ��
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
	
	//��ʱ����ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	//��ʼ��PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;									//ѡ��PWMģʽ1 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;			//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;					//������Ը�
	
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;        //���������״̬         
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//�����󻥲������״̬         
 
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);
	
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;//����ģʽ�����
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;//����ģʽ�����ѡ��          
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;         //��������        
  TIM_BDTRInitStructure.TIM_DeadTime = 0x90;                                         //����ʱ������         
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                 //ɲ������ʹ��         
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;//ɲ�����뼫��        
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//�Զ����ʹ��          
  TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
	
	//ʹ��Ԥװ�ؼĴ���
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);                //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���  
	TIM_Cmd(TIM1,ENABLE);

}


/*----------------------------------------------------
	�������ܣ�PWM�����ʼ��
----------------------------------------------------*/
void PWM_Init(u16 arr,u16 psc)
{
		TIM_1_Int_Init(arr, psc);			
}

/*------------------------------------------------
	�������ܣ����1����
	����������val����PWMռ�ձ�
--------------------------------------------------*/
void M1_SetSpeed(int val)
{
	if( val>POWER_MAX) val= POWER_MAX;
	else if( val<0) val= 0;
	TIM_SetCompare2(TIM1,val);
}
/*------------------------------------------------
	�������ܣ����2����
	����������val����PWMռ�ձ�
--------------------------------------------------*/
void M2_SetSpeed(int val)
{
	if( val>POWER_MAX) val= POWER_MAX;
	else if( val<0) val= 0;
	TIM_SetCompare1(TIM1,val);
}
/*------------------------------------------------
	�������ܣ����3����
	����������val����PWMռ�ձ�
--------------------------------------------------*/
void M3_SetSpeed(int val)
{
	if( val>POWER_MAX) val= POWER_MAX;
	else if( val<0) val= 0;
	TIM_SetCompare4(TIM1,val);
}
/*------------------------------------------------
	�������ܣ����4����
	����������val����PWMռ�ձ�
--------------------------------------------------*/
void M4_SetSpeed(int val)
{
	if( val>POWER_MAX) val= POWER_MAX;
	else if( val<0) val= 0;
	TIM_SetCompare3(TIM1,val);
} 


/*-------------------------------
	�������ܣ����PWM
-------------------------------*/

void Motor_Move(int pwm1,int pwm2, int pwm3, int pwm4)
{			
		M1_SetSpeed(pwm1);
		M2_SetSpeed(pwm2);
		M3_SetSpeed(pwm3);
		M4_SetSpeed(pwm4);	
}
