#include "delay.h"
#include "stm32f10x_it.h"

void delay_init(void)
{
	GetSysclkFreq();
	SysTick_Config(SystemCoreClock / 1000);  //初始化SysTick定时器 开启1ms中断
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































