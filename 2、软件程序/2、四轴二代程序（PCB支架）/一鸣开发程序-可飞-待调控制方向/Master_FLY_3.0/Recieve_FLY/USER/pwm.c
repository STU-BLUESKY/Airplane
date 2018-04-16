#include "pwm.h"



void PWM_init(void){ 
	
 GPIO_InitTypeDef GPIO_InitStructure2;         
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;        
 TIM_OCInitTypeDef TIM_OCInitStructure;        
 TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
 //第一步：配置时钟                 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_TIM1,ENABLE);
 //第二步，配置goio口          
 GPIO_InitStructure2.GPIO_Pin=GPIO_Pin_8;         // PA8_PWM_1
 GPIO_InitStructure2.GPIO_Speed=GPIO_Speed_50MHz;         
 GPIO_InitStructure2.GPIO_Mode=GPIO_Mode_AF_PP;                 //设置为复用浮空输出         
 GPIO_Init(GPIOA,&GPIO_InitStructure2);        
 
 GPIO_InitStructure2.GPIO_Pin=GPIO_Pin_9;         // PA9_PWM_2
 GPIO_InitStructure2.GPIO_Speed=GPIO_Speed_50MHz;         
 GPIO_InitStructure2.GPIO_Mode=GPIO_Mode_AF_PP;                 //设置为复用浮空输出         
 GPIO_Init(GPIOA,&GPIO_InitStructure2);
	
 GPIO_InitStructure2.GPIO_Pin=GPIO_Pin_10;         // PA10_PWM_3
 GPIO_InitStructure2.GPIO_Speed=GPIO_Speed_50MHz;         
 GPIO_InitStructure2.GPIO_Mode=GPIO_Mode_AF_PP;                 //设置为复用浮空输出         
 GPIO_Init(GPIOA,&GPIO_InitStructure2);
 
 GPIO_InitStructure2.GPIO_Pin=GPIO_Pin_11;         // PA11_PWM_4
 GPIO_InitStructure2.GPIO_Speed=GPIO_Speed_50MHz;         
 GPIO_InitStructure2.GPIO_Mode=GPIO_Mode_AF_PP;                 //设置为复用浮空输出         
 GPIO_Init(GPIOA,&GPIO_InitStructure2);
 
 GPIO_SetBits(GPIOA,GPIO_Pin_8);	 
 GPIO_SetBits(GPIOA,GPIO_Pin_9);	
 GPIO_SetBits(GPIOA,GPIO_Pin_10);	
 GPIO_SetBits(GPIOA,GPIO_Pin_11);	 

 //第三步，定时器基本配置         
 TIM_TimeBaseStructure.TIM_Period=1000-1;                   // 自动重装载寄存器的值        
 TIM_TimeBaseStructure.TIM_Prescaler=72-1;                  // 时钟预分频数        
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;      // 采样分频        
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数        
 TIM_TimeBaseStructure.TIM_RepetitionCounter=0;      //重复寄存器，用于自动更新pwm占空比                       
 TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 //第四步pwm输出配置         
 TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;                    //设置为pwm1输出模式         
 TIM_OCInitStructure.TIM_Pulse=50;                                 //设置占空比时间         
 TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;             //设置输出极性         
 TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;        //使能该通道输出         
 
 //下面几个参数是高级定时器才会用到，通用定时器不用配置         
 //TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;        //设置互补端输出极性         
 //TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;//使能互补端输出         
 TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;        //死区后输出状态         
 TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//死区后互补端输出状态         
////////////////////////////////
 TIM_OC1Init(TIM1,&TIM_OCInitStructure);                                            //按照指定参数初始化           
 TIM_OC2Init(TIM1,&TIM_OCInitStructure);                                            //按照指定参数初始化           
 TIM_OC3Init(TIM1,&TIM_OCInitStructure);                                            //按照指定参数初始化           
 TIM_OC4Init(TIM1,&TIM_OCInitStructure);                                            //按照指定参数初始化           
//////////////////////////////////




 //第五步，死区和刹车功能配置，高级定时器才有的，通用定时器不用配置         
 TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;//运行模式下输出
 TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;//空闲模式下输出选择          
 TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;         //锁定设置        
 TIM_BDTRInitStructure.TIM_DeadTime = 0x90;                                         //死区时间设置         
 TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                 //刹车功能使能         
 TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;//刹车输入极性        
 TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//自动输出使能          
 TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
 //第六步，使能端的打开 
 TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR1上的预装载寄存器  
 TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器 
 TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR3上的预装载寄存器 
 TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR4上的预装载寄存器 
 TIM_ARRPreloadConfig(TIM1, ENABLE);                //使能TIMx在ARR上的预装载寄存器         
 
 
 TIM_Cmd(TIM1,ENABLE);                              //打开TIM1                      Great !!!!!!!!!
 
 //下面这句是高级定时器才有的，输出pwm必须打开         
 

 // 在运行当中想要改变pwm的频率和占空比调用：
 TIM_SetAutoreload(TIM1,1000); 
 TIM_SetCompare1(TIM1,0);
 TIM_SetCompare2(TIM1,0);
 TIM_SetCompare3(TIM1,0);
 TIM_SetCompare4(TIM1,0);
 //TIM_CtrlPWMOutputs(TIM1, ENABLE);                                   //pwm输出使能，buyao打开
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
		




