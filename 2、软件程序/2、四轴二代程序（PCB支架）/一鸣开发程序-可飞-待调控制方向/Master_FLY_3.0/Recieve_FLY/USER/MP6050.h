#ifndef _MP6050_H_
#define _MP6050_H_

/*包含头------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "usart.h"
#include "IIC.h"
#include  "delay.h"
/*变量声明----------------------------------------------------------------*/

struct _float{
	      float x;
				float y;
				float z;};

struct _int16{
       int16_t x;
	     int16_t y;
	     int16_t z;};		

struct _trans{
     struct _int16 origin;  //原始值
	   struct _int16 averag;  //平均值
	   struct _int16 histor;  //历史值
	   struct _int16 quiet;   //静态值
	   struct _float radian;  //弧度值 
          };

struct _sensor{   
	struct _trans acc;
	struct _trans gyro;
              };

extern struct _sensor sensor;	







/*函数声明----------------------------------------------------------------*/
void Init_MPU6050(void);
void READ_MPU6050_Gyro(void);
void READ_MPU6050_Accel(void);
void MPU6050_WHO_AM_I(void);
void MPU6050_Dataanl(void);
void MPU6050_Read(void);							
void Gyro_OFFSET(void);	
//void MPU6050_Read(void);							
#endif
