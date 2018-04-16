#include "delay.h"
#include "stm32f10x_it.h"

void delay_init(void)
{
	GetSysclkFreq();
	SysTick_Config(SystemCoreClock / 1000);  //��ʼ��SysTick��ʱ�� ����1ms�ж�
}

void delay_ms(uint16_t nms)
{
		uint32_t t0=micros();
		while(micros() - t0 < nms * 1000);
			
}

void delay_us(u32 nus)
{
		uint32_t t0=micros();
		while(micros() - t0 < nus);
			
}































