/************************************************************************************
							�������ṩ�����µ��̣�
								Ilovemcu.taobao.com
								epic-mcu.taobao.com
							ʵ�������Χ��չģ����������ϵ���
							���ߣ����زر���							
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "LED.h"
#include "usart.h"
#include "SPI.h"
#include "adc.h"
#include "pwm.h"
#include "MP6050.h"
#include "MS5611.h"
#include "HMC5883L.h"
#include "math.h"
#include "sys.h"
#include "timer.h"

u8 tmp_buf[33];
u16 adcx;
int RE[5];
// RE[0]:���α���  RE[1]:���ű���  RE[2]:������� RE[3]:�������� RE[4]:�������
u8 NRF24L01_FLAG=0;
extern float GYRO_XOUT,GYRO_YOUT,GYRO_ZOUT,
	         ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT,
           MP6050_Temperature;//X,Y,Z�ᣬ�¶�	;		 //�����Ǻͼ��ٶȼƵ�X,Y,Z�ᣬ�¶�	

	
//////////////////////////////////
int init_main(){ 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	Nvic_Init(); 
	Init_LEDpin();
	delay_init(); 
	delay_ms(1000);
	//uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200 
 	NRF24L01_Init();
	Adc_Init();
	TIM3_Init(2500);
	PWM_init();
	I2C_GPIO_Configuration();
  Init_MPU6050();
	//LED_Flash(5,400,400);
	MPU6050_WHO_AM_I();
	Gyro_OFFSET();  //�ɼ���������ƫ

	
	//MS561101BA_Init();
	//Identify_HMC5883L();
	//HMC5883L_Self_Test();
	EnTIM3();     //����ʱ�ж�
	return 0;
}
//++++++++++++++++++++++++++++

//////////////////////////////////
int NRF24L01_TX_main()     // Send mode
{ 
	if(NRF24L01_TxPacket(tmp_buf)==TX_OK)	{
	return 1;	
	}	
//	tmp_buf[0]=11;
//	tmp_buf[1]=11;
	tmp_buf[32]=0;//���������	
	return 0;
}
//////////////////////////////////
int NRF24L01_RX_mian(){    //  Recieve mode
		
	if(NRF24L01_RxPacket(tmp_buf)==0){
				RE[0]=tmp_buf[1]*100+tmp_buf[2]-5000;
				RE[1]=tmp_buf[3]*100+tmp_buf[4]-5000;
				RE[2]=tmp_buf[5]*100+tmp_buf[6]-5000;
				RE[3]=tmp_buf[7]*100+tmp_buf[8]-5000;
				RE[4]=tmp_buf[9]*100+tmp_buf[10]-5000;
				tmp_buf[32]=0;//�����ַ���������
				LED0=!LED0;	
				return 1;
			}
	else {
				delay_us(100);
				return 0;
			}
}
//+++++++++++++++++++++++++++++++++
int Choice_mode(u8 outline_FLAG){
	u8 k;
	if(outline_FLAG==1){
		k=NRF24L01_TX_main();
	}
	else if(outline_FLAG==2){
		k=NRF24L01_RX_mian();
	}
	return k;
}
///////////////////////////////////
int adc_get(){
	adcx=Get_Adc_Average(ADC_Channel_1,10);
	//printf("ADC:%d\n",adcx);
	return 0;
}
////////////////////////////////////
int main(void)     // MAIN main 
{	
	//u16 i,n=1;        //    
	init_main();  ////////////////
  LED_Flash(5,10,1);
	LED_Flash(0,400,400);
	NRF24L01_RX_Mode();
	while(1)
	{
		adc_get();
		LED1=!LED1;
		Choice_mode(2);
		if(NRF24L01_Check()==0){
			EnTIM3();
		}
		else{
			DISEnTIM3();
			motor(0,0,0,0);
		}		
	}	
}






