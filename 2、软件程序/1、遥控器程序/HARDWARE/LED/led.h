#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED �˿ڶ���
#define LED1 PBout(6)	//�͵�ƽ��Ч
#define LED2 PBout(7)
#define BELL PBout(9)

void LED_Init(void);//��ʼ��
void BELL_Init(void);
#endif
