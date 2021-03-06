#include "stm32f10x.h"

/*********************************************/
/*	BMP图像																	 */
/*																					 */
/*********************************************/
//取模方式：列行式 
//取模走向：逆向
//数据格式：C51格式
/**图片宽度像素要数清楚**/


unsigned char bmp_test[]=		
{	
0x00,0x00,0x00,0xFE,0xFE,0xFC,0x0C,0x18,0x10,0x30,0x30,0x20,0x60,0x60,0x40,0xC0,
0x80,0x80,0x80,0x80,0x80,0xE0,0xF0,0xF0,0xFE,0x7F,0x77,0x73,0x33,0x26,0x80,0x80,
0x80,0xC0,0xC0,0x40,0x60,0x60,0x30,0x30,0x18,0x18,0x1C,0x0C,0xFC,0xFE,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x02,0x16,0x2F,0x4B,
0xDB,0xB1,0xA1,0x61,0x41,0xFD,0xFF,0xFF,0xFF,0xF7,0x3E,0x1E,0x86,0xE2,0xE3,0x71,
0x71,0xB1,0xF0,0xF0,0xF0,0xF8,0x3F,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x80,
0xC0,0x60,0xE0,0x81,0x0B,0x0B,0x0E,0x0D,0x07,0x07,0x0F,0x1F,0xFE,0xFC,0x9B,0x9B,
0xF7,0xF7,0xB7,0xB7,0xF3,0x71,0xFC,0xD0,0xF0,0xE1,0x73,0x3E,0x1C,0x00,0x00,0x00,
0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x70,0xF8,0xD8,
0x8B,0x8F,0xF9,0x18,0x19,0xFF,0x8B,0x88,0xD8,0xF0,0x60,0x00,0x00,0x00,0xFF,0xFF,
0x01,0x01,0x0D,0x3E,0x7B,0x73,0x7B,0x7C,0x3F,0x9F,0xFF,0xFF,0xFF,0xFC,0xF8,0xC0,
0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x80,0x80,
0x0C,0x1E,0x13,0x11,0x1F,0x03,0x1F,0x1D,0x13,0x16,0x9C,0x88,0xCC,0xDE,0xDE,0xDF,
0xFF,0xFF,0xBE,0xBE,0xBE,0xBE,0xBE,0x9E,0x9E,0xDE,0xCF,0xE7,0xF3,0xFD,0xFF,0x7F,
0x3F,0x07,0x00,0x80,0xFF,0xFF,0x00,0x00,0x00,0x00,0xE0,0xF8,0x1E,0x1F,0x03,0x01,
0xE1,0xF3,0xB3,0x33,0x73,0xFE,0xFE,0xF8,0xF0,0xF8,0xF2,0xE7,0xE7,0xE7,0xCF,0xCF,
0xCF,0xCF,0xFF,0xFF,0xCF,0xCF,0xCF,0xCF,0x4F,0xCF,0xCF,0xCF,0xCF,0xC7,0x7F,0x7F,
0x37,0x32,0x32,0xB3,0xE1,0x81,0xFD,0x87,0x9F,0xFE,0xF0,0xC0,0x03,0x07,0x0E,0x0C,
0x3C,0x7C,0x70,0xE1,0xC7,0xD1,0xFC,0xFF,0xBF,0xDA,0xD5,0xFF,0x87,0xBF,0xBF,0x80,
0xBF,0xBF,0x87,0xFF,0xFF,0xBF,0xBF,0x9B,0xCB,0xE3,0xF0,0xCB,0x9B,0xBF,0xBF,0xFF,
0xF0,0xE8,0xEE,0xEE,0xEF,0xE7,0x77,0x73,0x39,0x1D,0x0D,0x07,0x07,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,
0x07,0x03,0x03,0x07,0x07,0x0F,0x1F,0x1F,0x0F,0x07,0x03,0x03,0x07,0x07,0x07,0x07,
0x07,0x07,0x07,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
	
unsigned char bmp_apple[] =		//苹果LOGO 宽度54
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,0xFE,0xFC,0xFC,0xFC,0xFC,0xFE,0xFE,
0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xF8,0xF0,
0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x80,0x3C,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xF8,0xE0,0x80,0x00,0xF8,0xFF,0xFF,
0xFF,0xFE,0xF8,0xF3,0xC7,0x8F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x0F,0x07,0x03,0x01,
0x01,0x00,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xF9,
0xF3,0xE7,0xCF,0x9F,0x1F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x03,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xFC,0xF9,
0xF3,0xF3,0xE7,0xEF,0xCF,0xDF,0xBF,0x3F,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xF0,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0x03,0x0F,0x1F,0x3F,0x3F,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x7F,0x7F,0x7F,0x7F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x7F,0x7F,0x7E,
0xFE,0xFE,0xFD,0xFD,0xFB,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x3F,0x3F,0x1F,
0x0E,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x3F,
0xFF,0xFF,0xFF,0xFE,0xFE,0xFC,0xF8,0xF0,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x0F,0x0F,0x1F,0x3F,0x3F,0x3F,
0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

unsigned char bmp_stu_logo[]=	//stuLOGO 宽度50 高度64
{
0x00,0x00,0x00,0xFE,0xFE,0xFC,0x0C,0x18,0x10,0x30,0x30,0x20,0x60,0x60,0x40,0xC0,
0x80,0x80,0x80,0x80,0x80,0xE0,0xF0,0xF0,0xFE,0x7F,0x77,0x73,0x33,0x26,0x80,0x80,
0x80,0xC0,0xC0,0x40,0x60,0x60,0x30,0x30,0x18,0x18,0x1C,0x0C,0xFC,0xFE,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x02,0x16,0x2F,0x4B,
0xDB,0xB1,0xA1,0x61,0x41,0xFD,0xFF,0xFF,0xFF,0xF7,0x3E,0x1E,0x86,0xE2,0xE3,0x71,
0x71,0xB1,0xF0,0xF0,0xF0,0xF8,0x3F,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x80,
0xC0,0x60,0xE0,0x81,0x0B,0x0B,0x0E,0x0D,0x07,0x07,0x0F,0x1F,0xFE,0xFC,0x9B,0x9B,
0xF7,0xF7,0xB7,0xB7,0xF3,0x71,0xFC,0xD0,0xF0,0xE1,0x73,0x3E,0x1C,0x00,0x00,0x00,
0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x70,0xF8,0xD8,
0x8B,0x8F,0xF9,0x18,0x19,0xFF,0x8B,0x88,0xD8,0xF0,0x60,0x00,0x00,0x00,0xFF,0xFF,
0x01,0x01,0x0D,0x3E,0x7B,0x73,0x7B,0x7C,0x3F,0x9F,0xFF,0xFF,0xFF,0xFC,0xF8,0xC0,
0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x80,0x80,
0x0C,0x1E,0x13,0x11,0x1F,0x03,0x1F,0x1D,0x13,0x16,0x9C,0x88,0xCC,0xDE,0xDE,0xDF,
0xFF,0xFF,0xBE,0xBE,0xBE,0xBE,0xBE,0x9E,0x9E,0xDE,0xCF,0xE7,0xF3,0xFD,0xFF,0x7F,
0x3F,0x07,0x00,0x80,0xFF,0xFF,0x00,0x00,0x00,0x00,0xE0,0xF8,0x1E,0x1F,0x03,0x01,
0xE1,0xF3,0xB3,0x33,0x73,0xFE,0xFE,0xF8,0xF0,0xF8,0xF2,0xE7,0xE7,0xE7,0xCF,0xCF,
0xCF,0xCF,0xFF,0xFF,0xCF,0xCF,0xCF,0xCF,0x4F,0xCF,0xCF,0xCF,0xCF,0xC7,0x7F,0x7F,
0x37,0x32,0x32,0xB3,0xE1,0x81,0xFD,0x87,0x9F,0xFE,0xF0,0xC0,0x03,0x07,0x0E,0x0C,
0x3C,0x7C,0x70,0xE1,0xC7,0xD1,0xFC,0xFF,0xBF,0xDA,0xD5,0xFF,0x87,0xBF,0xBF,0x80,
0xBF,0xBF,0x87,0xFF,0xFF,0xBF,0xBF,0x9B,0xCB,0xE3,0xF0,0xCB,0x9B,0xBF,0xBF,0xFF,
0xF0,0xE8,0xEE,0xEE,0xEF,0xE7,0x77,0x73,0x39,0x1D,0x0D,0x07,0x07,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,
0x07,0x03,0x03,0x07,0x07,0x0F,0x1F,0x1F,0x0F,0x07,0x03,0x03,0x07,0x07,0x07,0x07,
0x07,0x07,0x07,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

unsigned char bmp_stu_logo_small[] =	//STU LOGO small 宽度:27 高度：32
{
0x00,0x00,0xFE,0xFE,0x04,0x0C,0x0C,0x78,0xD8,0x98,0x90,0xF0,0xF8,0xBC,0xBF,0x99,
0xD3,0x58,0x68,0x28,0x38,0x0C,0x04,0xFE,0xFE,0x00,0x00,0x00,0x00,0xFF,0xFF,0x80,
0xC0,0xF0,0x98,0x98,0xF2,0xC7,0x83,0x07,0xFF,0x3F,0xEF,0xCF,0x5C,0x74,0x66,0xFE,
0xCC,0x00,0xFF,0xFF,0x00,0x00,0x00,0xC0,0x7F,0x3F,0x23,0xEE,0xCA,0x0F,0x4F,0x4A,
0x6E,0x65,0xEC,0xFF,0xEC,0xEE,0xCE,0xEE,0x6F,0x77,0xFD,0x5F,0xA0,0x3F,0x3F,0xC0,
0x80,0x03,0x07,0x0C,0x18,0x3B,0x2F,0xE6,0xFE,0xFE,0xA2,0xAA,0x8A,0xFE,0xFB,0x82,
0xFA,0xFE,0x83,0xBE,0x82,0xFF,0xFF,0x33,0x38,0x18,0x0C,0x07,
};

unsigned char bmp_arow[] =	//AROW 宽度24  高度11
{
0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
0xC0,0xC0,0xF8,0xF0,0xE0,0xC0,0xC0,0x80,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x1F,0x0F,0x07,0x03,0x01,0x00,
};

unsigned char bmp_arow_up[] =			//尺寸 16*16
{
0x00,0x00,0xE0,0xF0,0xF8,0x1C,0xFE,0xFF,0xFF,0xFE,0x1C,0xF8,0xF0,0xE0,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,	
};

unsigned char bmp_arow_down[] =		//尺寸 16*16
{
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x07,0x0F,0x1F,0x38,0x7F,0xFF,0xFF,0x7F,0x38,0x1F,0x0F,0x07,0x00,0x00
};

unsigned char bmp_arow_left[] =		//尺寸 16*16
{
0x80,0xC0,0xE0,0xF0,0xF8,0xDC,0xDC,0xDC,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,
0x01,0x03,0x07,0x0F,0x1F,0x3B,0x3B,0x3B,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,	
};

unsigned char bmp_arow_right[] =	//尺寸 16*16
{
0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xDC,0xDC,0xDC,0xF8,0xF0,0xE0,0xC0,0x80,
0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x3B,0x3B,0x3B,0x1F,0x0F,0x07,0x03,0x01,	
};

unsigned char bmp_arow_blank[] =	//尺寸 16*16
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

unsigned char bmp_rocker_circle[] =		//尺寸 16*16
{
0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
};

unsigned char bmp_key_press[] =				//尺寸 16*16
{
0x00,0x00,0xFC,0xFC,0x0C,0xCC,0xEC,0xEC,0xEC,0xEC,0xCC,0x0C,0xFC,0xFC,0x00,0x00,
0x00,0x00,0x3F,0x3F,0x30,0x33,0x37,0x37,0x37,0x37,0x33,0x30,0x3F,0x3F,0x00,0x00,
};

unsigned char bmp_key_up[] =				  //尺寸 16*16
{
0x00,0x00,0xFC,0xFC,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0xFC,0xFC,0x00,0x00,
0x00,0x00,0x3F,0x3F,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3F,0x3F,0x00,0x00,	
};

unsigned char bmp_battery_top[] =								//宽度80 高度8
{
0x00,0x00,0xC0,0x60,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,
};

unsigned char bmp_battery_bottom[] =						//宽度80 高度8
{
0x00,0x00,0x03,0x06,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x03,0x01,0x00,0x00,0x00,0x00,0x00,
};

unsigned char bmp_battery_line[] =
{
0xFF,
};

unsigned char bmp_battery_no[] =
{
0x00,
};

unsigned char bmp_battery_head[] =							//宽度4 高度8
{	
0xFF,0x4E,0x42,0x7E
};













