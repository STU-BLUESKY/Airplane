/*包含头------------------------------------------------------------------*/
#include "MS5611.h"
#include "math.h"
#include "IIC.h"

#define MS5611Press_OSR  MS561101BA_D1_OSR_1024  //气压采样精度
#define MS5611Temp_OSR   MS561101BA_D2_OSR_1024  //温度采样精度

//气压计状体机
#define STAR_CON_TEM  0x01	//开始转换温度
#define CON_TEM_ING  	0x02 	//正在转换温度
#define STAR_CON_PRE  0x03	//开始转换气压
#define CON_PRE_ING  	0x04	//正在转换气压

/*变量声明----------------------------------------------------------------*/

//温度补偿大气压(pa), 相对高度(m), 温度(度)
volatile float MS5611_Pressure, MS5611_Altitude, MS5611_Temperature;	
uint16_t Cal_C[6];  									//用于存放PROM中的6组数据	
uint32_t D1_Pres,D2_Temp; 						// 存放数字气压和温度
static u8 MS5611_STA = STAR_CON_TEM; 	//当前工作状态(4种)
float dT,Temperature2;		//实际和参考温度之间的差异, 温度中间值
float Aux,OFF2,SENS2;   							//温度校验值
double OFF,SENS;  										//实际温度抵消,实际温度灵敏度

float Alt_offset_Pa=0;  //起飞所在平面对应的气压值
double PaOffsetNum = 0; 
int PaInitCnt=0;
u8 Pa_Init_Finished=0;
#define MOVAVG_SIZE (10)
static float Temp_buffer[MOVAVG_SIZE],Press_buffer[MOVAVG_SIZE],Alt_buffer[MOVAVG_SIZE];
static uint8_t temp_index=0,press_index=0,alt_index=0; 


/************************************************************   
* 函数名:MS5611_Read()
* 描述: 使用状态机保证读取气压和温度的转换时间足够
*				转换时间等于MS5611_Read调用间隔
**************************************************************/
void MS5611_Read(void)
{
	switch(MS5611_STA)
	{
		case STAR_CON_TEM:					//启动温度转换
			MS5611_StartConversion(MS5611Temp_OSR);
			MS5611_STA = CON_TEM_ING;
			break;
		case CON_TEM_ING:						//读取温度转换数据
			MS5611_GetTemperature();
			MS5611_STA = STAR_CON_PRE;
			break;
		case STAR_CON_PRE:					//启动气压转换
			MS5611_StartConversion(MS5611Press_OSR);
			MS5611_STA = CON_PRE_ING;
			break;
		case CON_PRE_ING:						//读取气压转换数据
			MS5611_GetPressure();
			MS5611_STA = STAR_CON_TEM;
			break;
		default:
			MS5611_STA = STAR_CON_TEM;
			break;
	}

}

/************************************************************   
* 函数名:MS5611_GetPressure   
* 描述 : 读取数字气压
* 输入 : 无    
**************************************************************/
void MS5611_GetPressure(void)
{
	
	D1_Pres= MS5611_GetConversion();
	
	OFF =(uint32_t)Cal_C[1]*65536+((uint32_t)Cal_C[3]*dT)/128;
	SENS=(uint32_t)Cal_C[0]*32768+((uint32_t)Cal_C[2]*dT)/256;


	//温度补偿
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
* 函数名:MS5611_GetTemperature   
* 描述 : 读取数字温度
* 输入  :无 
************************************************************/
void MS5611_GetTemperature(void)
{
	D2_Temp= MS5611_GetConversion();	
	dT=D2_Temp - (((uint32_t)Cal_C[4])<<8);	
	
	MS5611_Temperature=2000+dT*((int)Cal_C[5])/8388608;	//算出温度值的100倍，2001表示20.01°
//	MS5611_Temperature= D2_Temp;
	
//	MS5611_NewTemperature(MS5611_Temperature);
//	if(Pa_Init_Finished) MS5611_Temperature = MS5611_getAvg(Temp_buffer,MOVAVG_SIZE);
}

/************************************************************   
* 函数名:MS5611_StartConversion   
* 描述 : 进入转换模式
* 输入 : MS5611Press_OSR 	气压采样地址和精度
*				 MS5611Temp_OSR		温度采样地址和精度
* 输出 : 无  
************************************************************/
void MS5611_StartConversion(uint8_t command) 
{
	I2C_NoAddr_WriteByte(MS561101BA_ADDR,command);	//写转换命令	
}

/************************************************************   
* 函数名:MS5611_GetConversion   
* 描述 :获取转换结果
* 输入 :无
* 输出 :采样结果   
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
* 函数名:MS5611_Reset   
* 描述 : 复位  
* 输入 :无   
* 输出 :无    
************************************************************/ 
void MS5611_Reset(void)
{
		I2C_NoAddr_WriteByte(MS561101BA_ADDR,MS561101BA_RESET);
}

/************************************************************   
* 函数名:MS5611_readPROM   
* 描述 : 从PROM读取出厂校准数据
* 输入  :无   
* 输出  :无    
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
* 函数名:MS5611_Init   
* 描述 : MS5611初始化
* 输入  :无   
* 输出  :无    
************************************************************/
void MS5611_Init(void)
{
	MS5611_Reset();
	delay_ms(100);
	MS5611_readPROM();
	delay_ms(100);
} 

/**************************************************************************
*	函数名:float MS5611_Get_Altitude(void)
*	功能：将当前气压值转成相对高度 
*******************************************************************************/
float MS5611_Get_Altitude(void)
{
	float Altitude;

	//判断是否已经初始化
	if(Pa_Init_Finished == 0)
	{ 
		if(PaInitCnt > 10)
		{
			Alt_offset_Pa = PaOffsetNum / PaInitCnt;
			Pa_Init_Finished=1;
		}
		else	PaOffsetNum += MS5611_Pressure;	//积累10个数据
		PaInitCnt++;
		Altitude = 0; //设置高度为零
		return Altitude;
	}
	//计算相对高度 单位为m
	Altitude = 4433000.0 * (1 - pow((MS5611_Pressure / Alt_offset_Pa), 0.1903))*0.01f;
	return Altitude; 
	
}

/**************************************************************************
*	函数名:MS5611_OffsetInit(void)
*	功能：将当前气压值转成相对高度 
*******************************************************************************/
void MS5611_OffsetInit(void)
{
	while(!Pa_Init_Finished)
	{
		MS5611_Read();
		delay_ms(10);
	}
}


//新温度值
void MS5611_NewTemperature(float val)
{
	Temp_buffer[temp_index] = val;
	temp_index = (temp_index + 1) % MOVAVG_SIZE;
}
//新气压值
void MS5611_NewPress(float val)
{
	Press_buffer[press_index] = val;
	press_index = (press_index + 1) % MOVAVG_SIZE;
}
//新高度值
void MS5611_NewAlt(float val) 
{
	Alt_buffer[alt_index] = val;
	alt_index = (alt_index + 1) % MOVAVG_SIZE;
}
//计算平均值
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
//uint32_t ex_Pressure;			//串口读数转换值
//uint8_t  exchange_num[8];
/************************************************************   
* 函数名:SampleANDExchange   
* 描述 : 读取数据并转换串口发送
* 输入  :无   
* 输出  :无    
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
	printf("Temp:%4.3f°C  ",Temperature);
  	
}

*/


