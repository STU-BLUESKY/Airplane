#ifndef _MS5611_H_
#define _MS5611_H_
#include "IIC.h"
#include "delay.h"

//定义器件在IIC总线中的从地址,根据CSB引脚不同修改
//#define MS561101BA_ADDR  0xEC   //CBR=1 0x76 I2C address when CSB is connected to HIGH (VCC)
#define MS561101BA_ADDR  0xEE   //CBR=0 0x77 I2C address when CSB is connected to LOW (GND)
//#define MS561101BA_ADDR  0x76   //CBR=1 0x76 I2C address when CSB is connected to HIGH (VCC)
//#define MS561101BA_ADDR  0x77   //CBR=0 0x77 I2C address when CSB is connected to LOW (GND)

// 定义MS561101BA内部地址
// registers of the device
#define MS561101BA_D1 0x40
#define MS561101BA_D2 0x50
#define MS561101BA_RESET 0x1E

#define  MS561101BA_D1_OSR_256 0x40 	//气压采样精度
#define  MS561101BA_D1_OSR_512 0x42 
#define  MS561101BA_D1_OSR_1024 0x44 
#define  MS561101BA_D1_OSR_2048 0x46 
#define  MS561101BA_D1_OSR_4096 0x48 

#define  MS561101BA_D2_OSR_256 0x50 	//温度采样精度
#define  MS561101BA_D2_OSR_512 0x52 
#define  MS561101BA_D2_OSR_1024 0x54 
#define  MS561101BA_D2_OSR_2048 0x56 
#define  MS561101BA_D2_OSR_4096 0x58 

#define MS561101BA_PROM_BASE_ADDR 0xA2 // by adding ints from 0 to 6 we can read all the prom configuration values. 
#define MS561101BA_PROM_REG_COUNT 6 // number of registers in the PROM
#define MS561101BA_PROM_REG_SIZE 2 	// size in bytes of a prom registry.

//声明变量
extern volatile float MS5611_Pressure, MS5611_Altitude, MS5611_Temperature;	
extern float Alt_offset_Pa;
extern uint16_t Cal_C[6];
	
/*函数声明----------------------------------------------------------------*/
void MS5611_Init(void);
void MS5611_OffsetInit(void);
void MS5611_Reset(void);
void MS5611_readPROM(void); 
void MS5611_GetTemperature(void);
void MS5611_Read(void);
void MS5611_GetPressure(void);
void MS5611_StartConversion(uint8_t command);
uint32_t MS5611_GetConversion(void);
float MS5611_Get_Altitude(void);

void MS5611_NewTemperature(float val);
void MS5611_NewPress(float val);
void MS5611_NewAlt(float val);
float MS5611_getAvg(float * buff, int size);
// OSR (Over Sampling Ratio) constants
//#define MS561101BA_OSR_256 0x00			//采样精度选择
//#define MS561101BA_OSR_512 0x02
//#define MS561101BA_OSR_1024 0x04
//#define MS561101BA_OSR_2048 0x06
//#define MS561101BA_OSR_4096 0x08

//转换延时表 单位us 	
//uint32_t MS5611_Delay_us[9] = {	
//	//不同的采样精度对应不同的转换延时值
//	1500,//MS561101BA_OSR_256 0.9ms  0x00
//	2000,//MS561101BA_OSR_512 1.2ms  0x02
//	3000,//MS561101BA_OSR_1024 2.3ms 0x04
//	5000,//MS561101BA_OSR_2048 4.6ms 0x06
//	11000,//MS561101BA_OSR_4096 9.1ms 0x08
//};


#endif
 
 
 
 
 
 
 
 
 
