#include "pwm.h"



void PWM_init(void){ 
	
 GPIO_InitTypeDef GPIO_InitStructure2;         
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;        
 TIM_OCInitTypeDef TIM_OCInitStructure;        
 TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
 //��һ��������ʱ��                 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_TIM1,ENABLE);
 //�ڶ���������goio��          
 GPIO_InitStructure2.GPIO_Pin=GPIO_Pin_8;         // PA8_PWM_1
 GPIO_InitStructure2.GPIO_Speed=GPIO_Speed_50MHz;         
 GPIO_InitStructure2.GPIO_Mode=GPIO_Mode_AF_PP;                 //����Ϊ���ø������         
 GPIO_Init(GPIOA,&GPIO_InitStructure2);        
 
 GPIO_InitStructure2.GPIO_Pin=GPIO_Pin_9;         // PA9_PWM_2
 GPIO_InitStructure2.GPIO_Speed=GPIO_Speed_50MHz;         
 GPIO_InitStructure2.GPIO_Mode=GPIO_Mode_AF_PP;                 //����Ϊ���ø������         
 GPIO_Init(GPIOA,&GPIO_InitStructure2);
	
 GPIO_InitStructure2.GPIO_Pin=GPIO_Pin_10;         // PA10_PWM_3
 GPIO_InitStructure2.GPIO_Speed=GPIO_Speed_50MHz;         
 GPIO_InitStructure2.GPIO_Mode=GPIO_Mode_AF_PP;                 //����Ϊ���ø������         
 GPIO_Init(GPIOA,&GPIO_InitStructure2);
 
 GPIO_InitStructure2.GPIO_Pin=GPIO_Pin_11;         // PA11_PWM_4
 GPIO_InitStructure2.GPIO_Speed=GPIO_Speed_50MHz;         
 GPIO_InitStructure2.GPIO_Mode=GPIO_Mode_AF_PP;                 //����Ϊ���ø������         
 GPIO_Init(GPIOA,&GPIO_InitStructure2);
 
 GPIO_SetBits(GPIOA,GPIO_Pin_8);	 
 GPIO_SetBits(GPIOA,GPIO_Pin_9);	
 GPIO_SetBits(GPIOA,GPIO_Pin_10);	
 GPIO_SetBits(GPIOA,GPIO_Pin_11);	 

 //����������ʱ����������         
 TIM_TimeBaseStructure.TIM_Period=1000-1;                   // �Զ���װ�ؼĴ�����ֵ        
 TIM_TimeBaseStructure.TIM_Prescaler=72-1;                  // ʱ��Ԥ��Ƶ��        
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;      // ������Ƶ        
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���        
 TIM_TimeBaseStructure.TIM_RepetitionCounter=0;      //�ظ��Ĵ����������Զ�����pwmռ�ձ�                       
 TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 //���Ĳ�pwm�������         
 TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;                    //����Ϊpwm1���ģʽ         
 TIM_OCInitStructure.TIM_Pulse=50;                                 //����ռ�ձ�ʱ��         
 TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;             //�����������         
 TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;        //ʹ�ܸ�ͨ�����         
 
 //���漸�������Ǹ߼���ʱ���Ż��õ���ͨ�ö�ʱ����������         
 //TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;        //���û������������         
 //TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;//ʹ�ܻ��������         
 TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;        //���������״̬         
 TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//�����󻥲������״̬         
////////////////////////////////
 TIM_OC1Init(TIM1,&TIM_OCInitStructure);                                            //����ָ��������ʼ��           
 TIM_OC2Init(TIM1,&TIM_OCInitStructure);                                            //����ָ��������ʼ��           
 TIM_OC3Init(TIM1,&TIM_OCInitStructure);                                            //����ָ��������ʼ��           
 TIM_OC4Init(TIM1,&TIM_OCInitStructure);                                            //����ָ��������ʼ��           
//////////////////////////////////




 //���岽��������ɲ���������ã��߼���ʱ�����еģ�ͨ�ö�ʱ����������         
 TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;//����ģʽ�����
 TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;//����ģʽ�����ѡ��          
 TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;         //��������        
 TIM_BDTRInitStructure.TIM_DeadTime = 0x90;                                         //����ʱ������         
 TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                 //ɲ������ʹ��         
 TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;//ɲ�����뼫��        
 TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//�Զ����ʹ��          
 TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
 //��������ʹ�ܶ˵Ĵ� 
 TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���  
 TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ��� 
 TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ��� 
 TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR4�ϵ�Ԥװ�ؼĴ��� 
 TIM_ARRPreloadConfig(TIM1, ENABLE);                //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���         
 
 
 TIM_Cmd(TIM1,ENABLE);                              //��TIM1                      Great !!!!!!!!!
 
 //��������Ǹ߼���ʱ�����еģ����pwm�����         
 

 // �����е�����Ҫ�ı�pwm��Ƶ�ʺ�ռ�ձȵ��ã�
 TIM_SetAutoreload(TIM1,1000); 
 TIM_SetCompare1(TIM1,0);
 TIM_SetCompare2(TIM1,0);
 TIM_SetCompare3(TIM1,0);
 TIM_SetCompare4(TIM1,0);
 //TIM_CtrlPWMOutputs(TIM1, ENABLE);                                   //pwm���ʹ�ܣ�buyao��
} 


int motor(int m1, int m2, int m3, int m4){
//________________________________		
//////////////////////////////////  M1
	int Limit=850;
	if(m1<0){
		TIM_SetCompare1(TIM1,0);
		m1=0;
	}
	else if(m1>Limit){
		TIM_SetCompare1(TIM1,Limit);
		m1=Limit;
	}
	else
		TIM_SetCompare1(TIM1,m1);
//________________________________		
//////////////////////////////////  M2	
	if(m2<0){
		TIM_SetCompare2(TIM1,0);
		m2=0;
	}
	else if(m2>Limit){
		TIM_SetCompare2(TIM1,Limit);
		m2=Limit;
	}
	else
		TIM_SetCompare2(TIM1,m1);
//________________________________	
//////////////////////////////////	M3
	if(m3<0){
		TIM_SetCompare3(TIM1,0);
		m3=0;
	}
	else if(m3>Limit){
		TIM_SetCompare3(TIM1,Limit);
		m3=Limit;
	}
	else
		TIM_SetCompare3(TIM1,m1);
//________________________________		
//////////////////////////////////	M4
	if(m4<0){
		TIM_SetCompare4(TIM1,0);
		m4=0;
	}
	else if(m4>Limit){
		TIM_SetCompare4(TIM1,Limit);
		m4=Limit;
	}
	else
		TIM_SetCompare4(TIM1,m1);
	
	return 0;
}
		




