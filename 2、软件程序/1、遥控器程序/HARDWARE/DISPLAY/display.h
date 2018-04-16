#include "stm32f10x.h"
#include "oled.h"
#include "24l01.h"
#include "delay.h"
#include "led.h"
#include "string.h"
#include "adc.h"
#include "key.h"
#include "usart.h"


void diaplay(void);
void display_01(void);	//初始画面
void display_02(void);	//控制模式画面
void display_03(void);	//调试模式画面
void display_04(void);	//反馈模式画面
void display_05(void);	//摇杆反馈
void display_06(void);	//电量反馈
void display_07(void);	//2.4G调试
void display_08(void);	//发送调试
void display_09(void);	//接收调试
void display_10(void);  //按键反馈
void display_11(void);	//电量反馈
void display_12(void);	//演示模式画面
void display_13(void);	//参数调试

void show_gyro_despoint(void);
void show_pid(float kp, float ki, float kd);


