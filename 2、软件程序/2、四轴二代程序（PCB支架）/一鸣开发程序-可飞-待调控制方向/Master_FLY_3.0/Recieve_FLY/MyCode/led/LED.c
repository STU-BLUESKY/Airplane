/************************************************************************************
							本例程提供自以下店铺：
								Ilovemcu.taobao.com
								epic-mcu.taobao.com
							实验相关外围扩展模块均来自以上店铺
							作者：神秘藏宝室							
*************************************************************************************/
#include "LED.h"
#include "delay.h"

void Init_LEDpin(void){  // 5050LED
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PF端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB	
	GPIO_SetBits(GPIOB,GPIO_Pin_5| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15);					//初始化设置为0
}

void LED_Flash(u8 type, u16 on_time, u16 off_time){	// Like a adjustable star
	if(type==0){
	LED0 = 1;
  delay_ms(on_time);
	LED0 = 0;	
	delay_ms(off_time);
	}
	else if(type==1){
	LED1 = 0;
  delay_ms(on_time);
	LED1 = 1;	
	delay_ms(off_time);
	}
	else if(type==2){
	LED2 = 0;
  delay_ms(on_time);
	LED2 = 1;	
	delay_ms(off_time);
	}
	else if(type==3){
	LED3 = 0;
  delay_ms(on_time);
	LED3 = 1;	
	delay_ms(off_time);
	}
	else if(type==4){
	LED4 = 0;
  delay_ms(on_time);
	LED4 = 1;	
	delay_ms(off_time);
	}
	else {
	  LED0 =1;
		LED1 =LED2 =LED3 =LED4 = 0;
		delay_ms(on_time);
		LED0 =0;
		LED1 =LED2 =LED3 =LED4 = 1;	
		delay_ms(off_time);
	}
	
}

