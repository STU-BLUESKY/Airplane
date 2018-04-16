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
void display_01(void);	//��ʼ����
void display_02(void);	//����ģʽ����
void display_03(void);	//����ģʽ����
void display_04(void);	//����ģʽ����
void display_05(void);	//ҡ�˷���
void display_06(void);	//��������
void display_07(void);	//2.4G����
void display_08(void);	//���͵���
void display_09(void);	//���յ���
void display_10(void);  //��������
void display_11(void);	//��������
void display_12(void);	//��ʾģʽ����
void display_13(void);	//��������

void show_gyro_despoint(void);
void show_pid(float kp, float ki, float kd);


