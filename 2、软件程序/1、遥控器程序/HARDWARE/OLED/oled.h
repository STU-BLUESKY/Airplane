#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	

extern unsigned char bmp_test[],bmp_arow[];
extern unsigned char bmp_arow_up[],bmp_arow_down[],bmp_arow_left[],bmp_arow_right[],bmp_arow_blank[],bmp_rocker_circle[];
extern unsigned char bmp_key_press[], bmp_key_up[];
extern unsigned char bmp_battery_top[],bmp_battery_bottom[],bmp_battery_line[],bmp_battery_head[],bmp_battery_no[];

//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    


//-----------------OLED�˿ڶ���----------------  					   


#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_1)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_8)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_9)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_9)
 		     
//#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS
//#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p, u8 size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_ShowCHinese_Small(u8 x,u8 y,u8 no);
#endif  
	 



