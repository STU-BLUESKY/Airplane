#ifndef __IMU__H__
#define __IMU__H__

#include "stm32f10x.h"
#include "math.h"


#define RtA 		57.324841				
#define AtR    	0.0174533		

/*------------------------------------------
							欧拉角
------------------------------------------*/
typedef volatile struct 
{
  float Pitch;
  float Roll;
  float Yaw;
}AHRS_EulerAngleTypeDef;


extern AHRS_EulerAngleTypeDef EulerAngle;
extern float DCMgb[3][3];

/*------------------------------------------
							函数定义
------------------------------------------*/
void Angle_Calculate(float gx, float gy, float gz, float ax, float ay, float az);
void Angle_Calculate_Nine_Axis(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
float FL_ABS(float x);
float Q_rsqrt(float number);
float VariableParameter(float error);

#endif

