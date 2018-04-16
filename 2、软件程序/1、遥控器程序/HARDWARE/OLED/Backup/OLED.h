#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	 

extern unsigned char bmp_test[];

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������V3
//SSD1306 OLED ����IC��������
//������ʽ:8080����/4�ߴ���
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//OLEDģʽ����
//0: 4�ߴ���ģʽ  ��ģ���BS1��BS2����GND��
//1: ����8080ģʽ ��ģ���BS1��BS2����VCC��
#define OLED_MODE 	0 
		    						  
//---------------------------OLED�˿ڶ���--------------------------  					   
//#define OLED_CS  PBout(6)
#define OLED_RST PAout(8) 	
#define OLED_DC  PAout(9)   //OLED_DC
//#define OLED_WR  PGout(14)		  
//#define OLED_RD  PGout(13)	   

//ʹ��4�ߴ��нӿ�ʱʹ�� 
#define OLED_SCLK PAout(7)
#define OLED_SDIN PBout(1)

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_1)
		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);  		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
#endif  
	 







 

