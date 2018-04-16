#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "oled.h"
#include "key.h"
#include "24l01.h"
#include "mode.h"
#include "timer.h"
#include "adc.h"
#include "display.h"
#include "param.h"


extern u8 key;

u8 start=0; 

void logo_show(void);
void BSP_Init(void)
{
	delay_init();					
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	BELL_Init();
	KEY_Init();
	OLED_Init();
	Adc_Init();				
	uart_init(115200);
	NRF24L01_Init();
	while( NRF24L01_Check())			//ºÏ≤ÈNRF24L01
	{
		OLED_ShowString(0,0,"NRF24L01 Error",16);
		delay_ms(100);		
	}
	OLED_DrawBMP(38,0,50,8,bmp_test);
	while(!start)	logo_show();
	OLED_Clear();
	paramLoad();
	printf("Init OK£°\r\n");
	printf("Start... \r\n");
}

int main(void)	
{
	
	BSP_Init();
	
	while(1)
	{	
		delay_ms(10);
		diaplay();
		
		key=KEY_Scan(0);	
		if(key) 
		{ 
			LED1=0;
			OLED_Clear(); 
			Key_press(key);
		}
		chooseMode();

	}
}


void logo_show(void)
{
	key=KEY_Scan(0);						
	if(key!=0) start = 1; 			 
	delay_ms(10);
}





