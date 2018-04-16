#include "delay.h"
#include "key.h"
#include "led.h"
#include "sys.h"
#include "oled.h"
#include "mode.h"
#include "usart.h"

//改完

u8 key=0;

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_5|GPIO_Pin_8);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
}

//按键处理函数
//返回按键值
//mode：0，不支持连续按键； 1，支持连续按键；
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY3按下 WK_UP
//注意此函数由响应优先级， KEY0>KEY1>KEY_UP
u8 KEY_Scan(u8 mode)
{
	static u8 key_up = 1;				//按键松开标志
		
	if(mode)key_up = 1;					
	if(key_up &&(KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0||KEY6==0))
	{
		delay_ms(10);
		key_up=0;
		if( KEY1==0) return KEY1_PRES;
		else if( KEY2==0) return KEY2_PRES;
		else if( KEY3==0) return KEY3_PRES;
		else if( KEY4==0) return KEY4_PRES;
		else if( KEY5==0) return KEY5_PRES;
		else if( KEY6==0) return KEY6_PRES;
		
	}else if( KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1&&KEY6==1) key_up=1;

	return 0;
		
}

void Key_press(u8 key)
{
	switch(key)
		{
			case KEY1_PRES:	Key1_press();	break;
			case KEY2_PRES:	Key2_press();	break;						
			case KEY3_PRES:	Key3_press();	break;					
			case KEY4_PRES:	Key4_press();	break;	
			case KEY5_PRES:	Key5_press();	break;					
			case KEY6_PRES:	Key6_press();	break;	
			default: break;				
		}	
}

void Key1_press(void)										//上键
{
	NextMode --;
	if(NextMode<1) NextMode=1; 
}

void Key2_press(void)										//左键
{
	switch(CurMode)
	{
		case 2:  CurMode =1;	NextMode =1; break;
		case 3:  CurMode =1;  NextMode =2; break;
		case 4:  CurMode =1;  NextMode =3; break;
		case 5:  CurMode =4;  NextMode =2; break;
		case 6:  CurMode =4;  NextMode =1; break; 
		case 7:  CurMode =3;  NextMode =2; break; 
		case 8:  CurMode =7;  NextMode =1; break; 
		case 9:  CurMode =7;  NextMode =2; break; 
		case 10: CurMode =4;  NextMode =3; break;
		case 11: CurMode =7;  NextMode =3; break;
		case 12: CurMode =1;  NextMode =4; break;
		case 13: CurMode =3;  NextMode =1; break;
		default: break;		
	}
}	

void Key3_press(void)										//右键
{
	changeMode();	//模式转换
	
	switch(CurMode)
	{
		case 1: 
			if(NextMode==1) CurMode =2;
			else if(NextMode==2) CurMode=3;
			else if(NextMode==3) CurMode=4;
			else if(NextMode==4) CurMode=12;
			NextMode=1;
			break;
		case 3:
			if(NextMode==1) CurMode=13;
			if(NextMode==2) CurMode=7;
			NextMode=1;
			break;
		case 4: 
			if(NextMode==2) CurMode =5;
			if(NextMode==1) CurMode =6;	
			if(NextMode==3) CurMode =10;
			NextMode=1;		
			break;
		case 7:
			if(NextMode==1) CurMode =8;
			if(NextMode==2) CurMode =9;	
			if(NextMode==3) CurMode =11;	
			NextMode=1;		
			break;
		case 12:
			NextMode=1;		
			break;
		default: break;		
	}		
	
}	

void Key4_press(void)									//下键
{
	NextMode ++;
	switch(CurMode)
	{
		case 1: if(NextMode>4) NextMode=4; break;
		case 3: if(NextMode>4) NextMode=4; break;
		case 4: if(NextMode>4) NextMode=4; break;
		case 7: if(NextMode>3) NextMode=3; break;
		case 12: if(NextMode>1) NextMode=1; break;
		case 13: if(NextMode>21) NextMode=21; break;
		default: break;	
	}
}	

void Key5_press(void)	
{
//	LED1=!LED1;
//	printf("Left_Press\r\n");
}

void Key6_press(void)	
{
//	LED2=!LED2;
//	printf("Right_Press\r\n");
}

