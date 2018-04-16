#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED 端口定义
#define LED1 PBout(12)
#define LED2 PBout(13)
#define LED3 PBout(14)
#define LED4 PBout(15)

void LED_Init(void);//初始化
void LED_on(void);
void LED_off(void);
void LED_rev(void);
#endif
