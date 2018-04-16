#ifndef _MP6050_H_
#define _MP6050_H_

/*����ͷ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "usart.h"
#include "IIC.h"
#include  "delay.h"
/*��������----------------------------------------------------------------*/

struct _float{
	      float x;
				float y;
				float z;};

struct _int16{
       int16_t x;
	     int16_t y;
	     int16_t z;};		

struct _trans{
     struct _int16 origin;  //ԭʼֵ
	   struct _int16 averag;  //ƽ��ֵ
	   struct _int16 histor;  //��ʷֵ
	   struct _int16 quiet;   //��ֵ̬
	   struct _float radian;  //����ֵ 
          };

struct _sensor{   
	struct _trans acc;
	struct _trans gyro;
              };

extern struct _sensor sensor;	







/*��������----------------------------------------------------------------*/
void Init_MPU6050(void);
void READ_MPU6050_Gyro(void);
void READ_MPU6050_Accel(void);
void MPU6050_WHO_AM_I(void);
void MPU6050_Dataanl(void);
void MPU6050_Read(void);							
void Gyro_OFFSET(void);	
//void MPU6050_Read(void);							
#endif
