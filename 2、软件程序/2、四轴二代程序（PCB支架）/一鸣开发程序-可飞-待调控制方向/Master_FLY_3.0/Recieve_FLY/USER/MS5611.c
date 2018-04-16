/*����ͷ------------------------------------------------------------------*/
#include "MS5611.h"


/*�궨��------------------------------------------------------------------*/
//����������IIC�����еĴӵ�ַ,����CSB���Ų�ͬ�޸�
//#define MS561101BA_ADDR  0xec   //CBR=1 0x76 I2C address when CSB is connected to HIGH (VCC)
#define MS561101BA_ADDR   0xee   //CBR=0 0x77 I2C address when CSB is connected to LOW (GND)

// ����MS561101BA�ڲ���ַ
// registers of the device
#define MS561101BA_D1 0x40
#define MS561101BA_D2 0x50
#define MS561101BA_RESET 0x1E

// D1 and D2 result size (bytes)
#define MS561101BA_D1D2_SIZE 3

// OSR (Over Sampling Ratio) constants
#define MS561101BA_OSR_256 0x00
#define MS561101BA_OSR_512 0x02
#define MS561101BA_OSR_1024 0x04
#define MS561101BA_OSR_2048 0x06
#define MS561101BA_OSR_4096 0x08
//#define  MS561101BA_D1_OSR_256 0x40 
//#define  MS561101BA_D1_OSR_512 0x42 
//#define  MS561101BA_D1_OSR_1024 0x44 
//#define  MS561101BA_D1_OSR_2048 0x46 
#define  MS561101BA_D1_OSR_4096 0x48 

//#define  MS561101BA_D2_OSR_256 0x50 
//#define  MS561101BA_D2_OSR_512 0x52 
//#define  MS561101BA_D2_OSR_1024 0x54 
//#define  MS561101BA_D2_OSR_2048 0x56 
#define  MS561101BA_D2_OSR_4096 0x58 

#define MS561101BA_PROM_BASE_ADDR 0xA0 // by adding ints from 0 to 6 we can read all the prom configuration values. 
// C1 will be at 0xA2 and all the subsequent are multiples of 2
#define MS561101BA_PROM_REG_COUNT 6 // number of registers in the PROM
#define MS561101BA_PROM_REG_SIZE 2 // size in bytes of a prom registry.


/*��������----------------------------------------------------------------*/
uint16_t Cal_C[7];  //���ڴ��PROM�е�6������	
uint32_t D1_Pres,D2_Temp; // �������ѹ�����¶�
float Pressure;				//�¶Ȳ�������ѹ
float dT,Temperature,Temperature2;//ʵ�ʺͲο��¶�֮��Ĳ���,ʵ���¶�,�м�ֵ
double OFF,SENS;  //ʵ���¶ȵ���,ʵ���¶�������
float Aux,OFF2,SENS2;  //�¶�У��ֵ

uint32_t ex_Pressure;			//���ڶ���ת��ֵ
uint8_t  exchange_num[8];


/*��������----------------------------------------------------------------*/
 void MS561101BA_Reset(void);
 void MS561101BA_readPROM(void);
 uint32_t MS561101BA_DO_CONVERSION(u8 command);
 void MS561101BA_GetTemperature(u8 OSR_Temp);
 void MS561101BA_GetPressure(u8 OSR_Pres);
 void MS561101BA_Init(void);
 void SampleANDExchange(void);
/************************************************************   
* ������:MS561101BA_Reset   
* ���� : ��λ  
* ����  :��   
* ���  :��    
*/ 
void MS561101BA_Reset(void)
{
		I2C_NoAddr_WriteByte(MS561101BA_ADDR,MS561101BA_RESET);
}


/************************************************************   
* ������:MS561101BA_readPROM   
* ���� : ��PROM��ȡ����У׼����
* ����  :��   
* ���  :��    
*/ 
void MS561101BA_readPROM(void)
{   uint16_t value=0;u8 temp1[2]={0};
	  u8 i;
	  for (i=0;i<=MS561101BA_PROM_REG_COUNT;i++) 
	 {
      // I2C_Read_MultiBytes(MS561101BA_ADDR,MS561101BA_PROM_BASE_ADDR + (i * MS561101BA_PROM_REG_SIZE),2,temp1);
       
       //value=temp1[0]<<8|temp1[1];
		   //Cal_C[i]=value;
		 Cal_C[i]=I2C_Read_2Bytes(MS561101BA_ADDR,MS561101BA_PROM_BASE_ADDR + (i * MS561101BA_PROM_REG_SIZE));

		}
	//printf("\r�豸MS561101BA��ȡPROM: \r");
  //printf("\rC1=%d,C2=%d,C3=%d,C4=%d,C5=%d,C6=%d\r\n\r",Cal_C[1],Cal_C[2],Cal_C[3],Cal_C[4],Cal_C[5],Cal_C[6]);

  
}

/************************************************************   
* ������:MS561101BA_DO_CONVERSION   
* ���� :  
* ����  :��   
* ���  :��    
*/
uint32_t MS561101BA_DO_CONVERSION(uint8_t command)
{
		uint32_t conversion;

	 I2C_NoAddr_WriteByte(MS561101BA_ADDR,command);
	 
	delay_ms(10);//��ʱ,ȥ�����ݴ���
 
	conversion=I2C_Read_3Bytes(MS561101BA_ADDR,0);

   return conversion;

}

/************************************************************   
* ������:MS561101BA_GetTemperature   
* ���� : ��ȡ�����¶�
* ����  :��������   
* ���  :��    
*/
void MS561101BA_GetTemperature(u8 OSR_Temp)
{
   
	D2_Temp= MS561101BA_DO_CONVERSION(OSR_Temp);	
	delay_ms(100);
	
	dT=D2_Temp - (((uint32_t)Cal_C[5])<<8);
	Temperature=2000+dT*((uint32_t)Cal_C[6])/8388608;	//����¶�ֵ��100����2001��ʾ20.01��


}


/************************************************************   
* ������:MS561101BA_GetPressure   
* ���� : ��ȡ������ѹ
* ����  :��������   
* ���  :��    
*/
void MS561101BA_GetPressure(u8 OSR_Pres)
{
	
 
	
	D1_Pres= MS561101BA_DO_CONVERSION(OSR_Pres);

	delay_ms(100); 
	
	OFF=(uint32_t)(Cal_C[2]<<16)+((uint32_t)Cal_C[4]*dT)/128.0;
	SENS=(uint32_t)(Cal_C[1]<<15)+((uint32_t)Cal_C[3]*dT)/256.0;
	//�¶Ȳ���
	if(Temperature < 2000)// second order temperature compensation when under 20 degrees C
	{
		Temperature2 = (dT*dT) / 0x80000000;
		Aux = (Temperature-2000)*(Temperature-2000);
		OFF2 = 2.5*Aux;
		SENS2 = 1.25*Aux;
		if(Temperature < -1500)
		{
			Aux = (Temperature+1500)*(Temperature+1500);
			OFF2 = OFF2 + 7*Aux;
			SENS2 = SENS + 5.5*Aux;
		}
	}else  //(Temperature > 2000)
	{
		Temperature2 = 0;
		OFF2 = 0;
		SENS2 = 0;
	}
	
	Temperature = Temperature - Temperature2;
	OFF = OFF - OFF2;
	SENS = SENS - SENS2;	

	Pressure=(D1_Pres*SENS/2097152.0-OFF)/32768.0;

}

/************************************************************   
* ������:MS561101BA_Init   
* ���� : MS561101BA��ʼ��
* ����  :��   
* ���  :��    
*/ 
void MS561101BA_Init(void)
{
	MS561101BA_Reset();
	delay_ms(100);
	MS561101BA_readPROM();
	delay_ms(100);
} 

/************************************************************   
* ������:SampleANDExchange   
* ���� : ��ȡ���ݲ�ת�����ڷ���
* ����  :��   
* ���  :��    
*/ 
void SampleANDExchange(void) 
{
   uint8_t i=0;
	MS561101BA_GetTemperature(MS561101BA_D2_OSR_4096);//0x58
	MS561101BA_GetPressure(MS561101BA_D1_OSR_4096);		//0x48
	ex_Pressure=(long)(Pressure);

    if(Pressure<0)
	{
		ex_Pressure=-ex_Pressure;
		exchange_num[0]='-';
	}
	else exchange_num[0]=' ';

	exchange_num[1]=ex_Pressure/100000+0x30;
	ex_Pressure=ex_Pressure%100000;

	exchange_num[2]=ex_Pressure/10000+0x30;
	ex_Pressure=ex_Pressure%10000;

	exchange_num[3]=ex_Pressure/1000+0x30;
	ex_Pressure=ex_Pressure%1000;

	exchange_num[4]=ex_Pressure/100+0x30;
	ex_Pressure=ex_Pressure%100;

	exchange_num[5]='.';

	exchange_num[6]=ex_Pressure/10+0x30;
	ex_Pressure=ex_Pressure%10;

	exchange_num[7]=ex_Pressure+0x30;
	printf("Press:");
   	for(i=0;i<8;i++)
	{
	  printf("%c",exchange_num[i]);
	  //delay_ms(20);             //////QYQ///////////cancel
	}
		printf("mbar   ");
	printf("Temp:%4.3f��C  ",Temperature);
  	
}

