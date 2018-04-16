/*����ͷ------------------------------------------------------------------*/
#include "MS5611.h"
#include "math.h"
#include "IIC.h"

#define MS5611Press_OSR  MS561101BA_D1_OSR_1024  //��ѹ��������
#define MS5611Temp_OSR   MS561101BA_D2_OSR_1024  //�¶Ȳ�������

//��ѹ��״���
#define STAR_CON_TEM  0x01	//��ʼת���¶�
#define CON_TEM_ING  	0x02 	//����ת���¶�
#define STAR_CON_PRE  0x03	//��ʼת����ѹ
#define CON_PRE_ING  	0x04	//����ת����ѹ

/*��������----------------------------------------------------------------*/

//�¶Ȳ�������ѹ(pa), ��Ը߶�(m), �¶�(��)
volatile float MS5611_Pressure, MS5611_Altitude, MS5611_Temperature;	
uint16_t Cal_C[6];  									//���ڴ��PROM�е�6������	
uint32_t D1_Pres,D2_Temp; 						// ���������ѹ���¶�
static u8 MS5611_STA = STAR_CON_TEM; 	//��ǰ����״̬(4��)
float dT,Temperature2;		//ʵ�ʺͲο��¶�֮��Ĳ���, �¶��м�ֵ
float Aux,OFF2,SENS2;   							//�¶�У��ֵ
double OFF,SENS;  										//ʵ���¶ȵ���,ʵ���¶�������

float Alt_offset_Pa=0;  //�������ƽ���Ӧ����ѹֵ
double PaOffsetNum = 0; 
int PaInitCnt=0;
u8 Pa_Init_Finished=0;
#define MOVAVG_SIZE (10)
static float Temp_buffer[MOVAVG_SIZE],Press_buffer[MOVAVG_SIZE],Alt_buffer[MOVAVG_SIZE];
static uint8_t temp_index=0,press_index=0,alt_index=0; 


/************************************************************   
* ������:MS5611_Read()
* ����: ʹ��״̬����֤��ȡ��ѹ���¶ȵ�ת��ʱ���㹻
*				ת��ʱ�����MS5611_Read���ü��
**************************************************************/
void MS5611_Read(void)
{
	switch(MS5611_STA)
	{
		case STAR_CON_TEM:					//�����¶�ת��
			MS5611_StartConversion(MS5611Temp_OSR);
			MS5611_STA = CON_TEM_ING;
			break;
		case CON_TEM_ING:						//��ȡ�¶�ת������
			MS5611_GetTemperature();
			MS5611_STA = STAR_CON_PRE;
			break;
		case STAR_CON_PRE:					//������ѹת��
			MS5611_StartConversion(MS5611Press_OSR);
			MS5611_STA = CON_PRE_ING;
			break;
		case CON_PRE_ING:						//��ȡ��ѹת������
			MS5611_GetPressure();
			MS5611_STA = STAR_CON_TEM;
			break;
		default:
			MS5611_STA = STAR_CON_TEM;
			break;
	}

}

/************************************************************   
* ������:MS5611_GetPressure   
* ���� : ��ȡ������ѹ
* ���� : ��    
**************************************************************/
void MS5611_GetPressure(void)
{
	
	D1_Pres= MS5611_GetConversion();
	
	OFF =(uint32_t)Cal_C[1]*65536+((uint32_t)Cal_C[3]*dT)/128;
	SENS=(uint32_t)Cal_C[0]*32768+((uint32_t)Cal_C[2]*dT)/256;


	//�¶Ȳ���
	if(MS5611_Temperature < 2000)// second order temperature compensation when under 20 degrees C
	{
		Temperature2 = (dT*dT) / 0x80000000;
		Aux = (MS5611_Temperature-2000)*(MS5611_Temperature-2000);
		OFF2 = 2.5*Aux;
		SENS2 = 1.25*Aux;
		if(MS5611_Temperature < -1500)	//Temperature < -1500
		{
			Aux = (MS5611_Temperature+1500)*(MS5611_Temperature+1500);
			OFF2 = OFF2 + 7*Aux;
			SENS2 = SENS + 5.5*Aux;
		}
	}else  //Temperature > 2000
	{
		Temperature2 = 0;
		OFF2 = 0;
		SENS2 = 0;
	}
	
	MS5611_Temperature = MS5611_Temperature - Temperature2;
	OFF = OFF - OFF2;
	SENS = SENS - SENS2;	


	
	MS5611_Pressure = (D1_Pres*SENS/2097152.0-OFF)/32768.0;
//	MS5611_NewPress(MS5611_Pressure);
//	if(Pa_Init_Finished) MS5611_Pressure = MS5611_getAvg(Press_buffer,MOVAVG_SIZE);
	
	MS5611_Altitude = MS5611_Get_Altitude();
//	MS5611_NewAlt(MS5611_Altitude);
//	if(Pa_Init_Finished) MS5611_Altitude = MS5611_getAvg(Alt_buffer,MOVAVG_SIZE);
}

/************************************************************   
* ������:MS5611_GetTemperature   
* ���� : ��ȡ�����¶�
* ����  :�� 
************************************************************/
void MS5611_GetTemperature(void)
{
	D2_Temp= MS5611_GetConversion();	
	dT=D2_Temp - (((uint32_t)Cal_C[4])<<8);	
	
	MS5611_Temperature=2000+dT*((int)Cal_C[5])/8388608;	//����¶�ֵ��100����2001��ʾ20.01��
//	MS5611_Temperature= D2_Temp;
	
//	MS5611_NewTemperature(MS5611_Temperature);
//	if(Pa_Init_Finished) MS5611_Temperature = MS5611_getAvg(Temp_buffer,MOVAVG_SIZE);
}

/************************************************************   
* ������:MS5611_StartConversion   
* ���� : ����ת��ģʽ
* ���� : MS5611Press_OSR 	��ѹ������ַ�;���
*				 MS5611Temp_OSR		�¶Ȳ�����ַ�;���
* ��� : ��  
************************************************************/
void MS5611_StartConversion(uint8_t command) 
{
	I2C_NoAddr_WriteByte(MS561101BA_ADDR,command);	//дת������	
}

/************************************************************   
* ������:MS5611_GetConversion   
* ���� :��ȡת�����
* ���� :��
* ��� :�������   
************************************************************/
uint32_t MS5611_GetConversion(void)
{
	uint32_t conversion;
	u8 tmp[3];

	tmp[0]=I2C_ReadByte(MS561101BA_ADDR, 0x00);
	tmp[1]=I2C_ReadByte(MS561101BA_ADDR, 0x01);
	tmp[2]=I2C_ReadByte(MS561101BA_ADDR, 0x02);
			
	conversion = (uint32_t)tmp[0]<<16 | (uint32_t)tmp[1]<<8 | (uint32_t)tmp[2];
	return conversion;
}

/************************************************************   
* ������:MS5611_Reset   
* ���� : ��λ  
* ���� :��   
* ��� :��    
************************************************************/ 
void MS5611_Reset(void)
{
		I2C_NoAddr_WriteByte(MS561101BA_ADDR,MS561101BA_RESET);
}

/************************************************************   
* ������:MS5611_readPROM   
* ���� : ��PROM��ȡ����У׼����
* ����  :��   
* ���  :��    
************************************************************/ 
void MS5611_readPROM(void)
{   
	uint16_t value=0;
	u8 temp[2]={0};
	u8 i;
	for (i=0;i<MS561101BA_PROM_REG_COUNT;i++) 
	{
//		I2C_Read_Len(MS561101BA_ADDR,MS561101BA_PROM_BASE_ADDR + (i * MS561101BA_PROM_REG_SIZE),2,temp);
//		value=(uint16_t)temp[0]<<8|temp[1];
//		Cal_C[i] = value;	
		
		temp[0]=I2C_ReadByte(MS561101BA_ADDR, MS561101BA_PROM_BASE_ADDR+(i*2));
		temp[1]=I2C_ReadByte(MS561101BA_ADDR, MS561101BA_PROM_BASE_ADDR+(i*2)+1);
		value=(uint16_t)temp[0]<<8|temp[1];
		Cal_C[i] = value;	
	}
	
}
/************************************************************   
* ������:MS5611_Init   
* ���� : MS5611��ʼ��
* ����  :��   
* ���  :��    
************************************************************/
void MS5611_Init(void)
{
	MS5611_Reset();
	delay_ms(100);
	MS5611_readPROM();
	delay_ms(100);
} 

/**************************************************************************
*	������:float MS5611_Get_Altitude(void)
*	���ܣ�����ǰ��ѹֵת����Ը߶� 
*******************************************************************************/
float MS5611_Get_Altitude(void)
{
	float Altitude;

	//�ж��Ƿ��Ѿ���ʼ��
	if(Pa_Init_Finished == 0)
	{ 
		if(PaInitCnt > 10)
		{
			Alt_offset_Pa = PaOffsetNum / PaInitCnt;
			Pa_Init_Finished=1;
		}
		else	PaOffsetNum += MS5611_Pressure;	//����10������
		PaInitCnt++;
		Altitude = 0; //���ø߶�Ϊ��
		return Altitude;
	}
	//������Ը߶� ��λΪm
	Altitude = 4433000.0 * (1 - pow((MS5611_Pressure / Alt_offset_Pa), 0.1903))*0.01f;
	return Altitude; 
	
}

/**************************************************************************
*	������:MS5611_OffsetInit(void)
*	���ܣ�����ǰ��ѹֵת����Ը߶� 
*******************************************************************************/
void MS5611_OffsetInit(void)
{
	while(!Pa_Init_Finished)
	{
		MS5611_Read();
		delay_ms(10);
	}
}


//���¶�ֵ
void MS5611_NewTemperature(float val)
{
	Temp_buffer[temp_index] = val;
	temp_index = (temp_index + 1) % MOVAVG_SIZE;
}
//����ѹֵ
void MS5611_NewPress(float val)
{
	Press_buffer[press_index] = val;
	press_index = (press_index + 1) % MOVAVG_SIZE;
}
//�¸߶�ֵ
void MS5611_NewAlt(float val) 
{
	Alt_buffer[alt_index] = val;
	alt_index = (alt_index + 1) % MOVAVG_SIZE;
}
//����ƽ��ֵ
float MS5611_getAvg(float * buff, int size) 
{
	float sum = 0.0;
	int i;
	for(i=0; i<size; i++) 
	{
		sum += buff[i];
	}
	return sum / size;
}


///******************************************************
//uint32_t ex_Pressure;			//���ڶ���ת��ֵ
//uint8_t  exchange_num[8];
/************************************************************   
* ������:SampleANDExchange   
* ���� : ��ȡ���ݲ�ת�����ڷ���
* ����  :��   
* ���  :��    
*/ 
/*
void SampleANDExchange(void) 
{
   uint8_t i=0;
//	MS561101BA_GetTemperature(MS561101BA_D2_OSR_4096);//0x58
//	MS561101BA_GetPressure(MS561101BA_D1_OSR_4096);		//0x48
	ex_Pressure=(long)(MS5611_Pressure);

    if(MS5611_Pressure<0)
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

*/


