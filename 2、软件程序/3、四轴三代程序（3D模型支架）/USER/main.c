#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "pwm.h"
#include "24l01.h"
#include "timer.h"
#include "mpu6050.h"
#include "string.h"
#include "IIC.h"
#include "HMC5883L.h"
#include "MS5611.h"
#include "Altitude.h"
#include "param.h"
#include "control.h"
#include "pid.h"


#define DATA_LEN (32)

extern float angle_D;

float Mx,My,Mz;

u16 pwmval =100;
u8 buf_tmp[33]={"MPU"};
u8 MSG_RX_BUF[33];


void send_mpu6050(void);
void Init(void)
{	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	LED_Init();
	NRF24L01_Init();          //G2.4G初始化
	while( NRF24L01_Check()){};	//检查NRF24L01
	NRF24L01_RX_Mode();
	
  PWM_Init(PWM_ARR,PWM_PSC);	
	MPU_Init();
  Gyro_OFFSET();	
  Acc_OFFSET();	
	HMC5883L_Init();
//	MS5611_Init();				//待调试
//	MS5611_OffsetInit();	//待调试
	paramLoad();
	TIM2_Config(9999,71);	//10ms		need to change AHRS if thange this
	TIM3_Config(999,71);	//1ms		
	LED_on();
}

int main(void)	
{		
	Init();
	while(1)
	{
//		if(loop50HzFlag)
//		{				
//			send_mpu6050();		
//			loop50HzFlag = 0;	
//		}	
		
//		if(loop50HzFlag)
//		{
//			MS5611_Read();	
//			AltitudeCombineThread();
//			loop50HzFlag = 0;
//		}
				
		if(loop200HzFlag)
		{
			if(NRF24L01_RxPacket(MSG_RX_BUF)==0)
			{
				NRF24L01_MsgAnalysis(MSG_RX_BUF);
			}
			loop200HzFlag = 0;
		}
		
	}
}

void send_mpu6050(void)
{
	float sendPitch=11.1,sendRoll=12.1,sendYaw=13.1;
		
			sendPitch = EulerAngle.Pitch;
			sendRoll  = EulerAngle.Roll;
			sendYaw   = EulerAngle.Yaw; //EulerAngle.Yaw;
	
//			sendPitch = (float)MS5611_Altitude;
//			sendRoll  = (float)MS5611_Pressure;
//			sendYaw   = (float)MS5611_Temperature;
	
//			sendPitch = (float)nav.z;
//			sendRoll  = (float)nav.vz;
//			sendYaw   = (float)nav.az;
	
			memcpy(&buf_tmp[3],&sendPitch, sizeof(sendPitch));	//发送姿态角数据
			memcpy(&buf_tmp[7],&sendRoll,  sizeof(sendRoll));	
			memcpy(&buf_tmp[11],&sendYaw,  sizeof(sendYaw));	
			NRF24L01_TxPacket(buf_tmp);	
}


