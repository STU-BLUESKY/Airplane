#ifndef _HMC5883L_H_
#define _HMC5883L_H_
#include "stm32f10x.h"

#define MagnAxis_Sensitive (2048.0)

#define ABS(x) ((x)>=0?(x):(-(x)))

#define	HMC5883L_Addr   0x3C	//磁场传感器器件地址   
#define HMC5883L_ConfigurationRegisterA  0x00
#define HMC5883L_ConfigurationRegisterB  0x01
#define HMC5883L_ModeRegister            0x02
#define HMC5883L_Output_X_MSB            0x03
#define HMC5883L_Output_X_LSB 					 0x04
#define HMC5883L_Output_Z_MSB            0x05
#define HMC5883L_Output_Z_LSB 					 0x06
#define HMC5883L_Output_Y_MSB            0x07
#define HMC5883L_Output_Y_LSB 					 0x08
#define HMC5883L_StatusRegister					 0x09
#define HMC5883L_ID_A										 0x0A
#define HMC5883L_ID_B 									 0x0B
#define HMC5883L_ID_C 									 0x0C

#define HMC5883L_OFFSET_X   (9)
#define HMC5883L_OFFSET_Y   (149)

#define CalThreshold 0 


void Read_HMC5883L(float *mx,float *my,float *mz);	//读取
void HMC5883L_Self_Test(void);	//自测程序
void Initialize_Cal_Variables(int MagX, int MagY, int  MagZ); 
void Calibrate(int MagX, int MagY, int  MagZ);
void Compute_and_Save(void);
void Hard_Iron_Correction(int MagX, int MagY, int  MagZ);

u8 HMC5883L_Init(void);
void slide_filter( float *magn_x, float *magn_y, float *magn_z);


#endif




