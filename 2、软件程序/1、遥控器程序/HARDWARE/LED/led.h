#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED 端口定义
#define LED1 PBout(6)	//低电平有效
#define LED2 PBout(7)
#define BELL PBout(9)

void LED_Init(void);//初始化
void BELL_Init(void);
#endif
