#ifndef  __KEY_H
#define	 __KEY_H
#include "sys.h"

#define KEY1 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15)
#define KEY2 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14)
#define KEY3 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
#define KEY4 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)
#define KEY5 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)
#define KEY6 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)

#define KEY1_PRES  1
#define KEY2_PRES  2
#define KEY3_PRES  3
#define KEY4_PRES  4
#define KEY5_PRES  5
#define KEY6_PRES  6

extern u8 key;

void KEY_Init(void);
u8 KEY_Scan(u8);

void Key_press(u8 key);
void Key1_press(void);
void Key2_press(void);
void Key3_press(void);
void Key4_press(void);
void Key5_press(void);
void Key6_press(void);

#endif




