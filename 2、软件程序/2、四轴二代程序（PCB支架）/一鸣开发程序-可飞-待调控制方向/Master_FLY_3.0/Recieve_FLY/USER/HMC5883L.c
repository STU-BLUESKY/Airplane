/*包含头------------------------------------------------------------------*/
#include "stm32f10x_conf.h"
#include "usart.h"
#include "IIC.h"
#include  "delay.h"

#include  <math.h> 

/*宏定义------------------------------------------------------------------*/
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

/*变量声明----------------------------------------------------------------*/
int Magn_x,Magn_y,Magn_z,Magn_x_Max,Magn_y_Max,Magn_z_Max,Magn_x_Min,Magn_y_Min,Magn_z_Min;    //5883用到
float angle_D;   //5883 的指南针
float X_Offset,Y_Offset,Z_Offset,X_scale,Y_scale,Z_scale;
int Xmax, Xmin, Ymax, Ymin, Zmax, Zmin;
int Mag_UserCal_Offset_X,Mag_UserCal_Offset_Y,Mag_UserCal_Offset_Z;
/*函数声明----------------------------------------------------------------*/
extern void delay_init( );
extern void delay_ms(uint16_t nms);
extern void delay_us(uint32_t nus);

extern void USART1_Conf(void);
extern void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...) ;
extern void Usart_Send(u8 *p,uint16_t len) ;


extern void I2C_GPIO_Configuration(void);
extern unsigned char I2C_ReadByte(unsigned char DeviceAddr,unsigned char address);  //从24c02的地址address中读取一个字节数据
extern void I2C_WriteByte(unsigned char DeviceAddr,unsigned char address,unsigned char info);
extern void I2C_NoAddr_WriteByte(unsigned char DeviceAddr,unsigned char info);


void Read_HMC5883L(void);
void Initialize_Cal_Variables(int MagX, int MagY, int  MagZ); 
void Calibrate(int MagX, int MagY, int  MagZ);
void Compute_and_Save(void);
void Hard_Iron_Correction(int MagX, int MagY, int  MagZ);
/************************************************************   
* 函数名:Identify_HMC5883L  
* 描述 : 设备识别 
* 输入  :无   
* 输出  :无    
*/
void Identify_HMC5883L(void)
{
	u8 ID_A,ID_B,ID_C;
	ID_A=I2C_ReadByte(HMC5883L_Addr,HMC5883L_ID_A);
	ID_B=I2C_ReadByte(HMC5883L_Addr,HMC5883L_ID_B);
	ID_C=I2C_ReadByte(HMC5883L_Addr,HMC5883L_ID_C);
	if(ID_A=='H'&&ID_B=='4'&&ID_C=='3')
	{
		printf("\r设备HMC5883L设别成功!\r\n\r");
	}else
	{
		printf("\r错误!无法识别设备HMC5883L!\r\n\r");
	}
}
/************************************************************   
* 函数名:Read_HMC5883L   
* 描述 : 读取磁场强度 
* 输入  :无   
* 输出  :无    
*/
//void Init_HMC5883L(void)//读取
//{
//	 
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterA,0x14);   //配置寄存器A：采样平均数1 输出速率30Hz 正常测量
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterB,0x20);   //配置寄存器B：增益控制
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister,0x00);   //模式寄存器：连续测量模式
//}
/************************************************************   
* 函数名:HMC58X3_Offset 
* 描述 : 计算X,Y,Z轴偏移,被HMC5883L_Self_Test（）调用
* 输入  :无   
* 输出  :无    
*/
void HMC58X3_Offset(void)					 //计算零偏
{
	X_Offset   = (Magn_x_Max+Magn_x_Min)/2;
	Y_Offset   = (Magn_y_Max+Magn_y_Min)/2;
	Z_Offset   = (Magn_z_Max+Magn_z_Min)/2;		
}
/************************************************************   
* 函数名:Calibrate 
* 描述 : 计算X,Y,Z轴最值,被HMC5883L_Self_Test（）调用
* 输入  :无   
* 输出  :无    
*/
void Initialize_Cal_Variables(int MagX, int MagY, int  MagZ)
{
// set Max and Min values of the mag output to the current values
			Xmax=MagX;
			Xmin=MagX;
			Ymax=MagY;
			Ymin=MagY;
			Zmax=MagZ;
			Zmin=MagZ;
}
void Calibrate(int MagX, int MagY, int  MagZ)
{
			//  this routine will capture the max and min values of the mag X, Y, and Z data while the
			//  compass is being rotated 360 degrees through the level plane and the upright plane.  
			//  i.e. horizontal and vertical circles.
			// This function should be invoked while making continuous measurements 
			//on the magnetometers
			int  MagXreading,  MagYreading,  MagZreading;
			MagXreading=MagX;  // just for clarification...  can remove these lines
			MagYreading=MagY;
			MagZreading=MagZ;
			if (MagXreading > Xmax )   Xmax = MagXreading;
			if (MagXreading < Xmin )   Xmin = MagXreading;
			if(MagYreading > Ymax ) Ymax = MagYreading;
			if(MagYreading < Ymin ) Ymin = MagYreading;
			if(MagZreading > Zmax ) Zmax = MagZreading;
			if(MagZreading < Zmin ) Zmin = MagZreading;
}
void Compute_and_Save(void)
		{
		if(ABS(Xmax - Xmin) > CalThreshold )
		{
		Mag_UserCal_Offset_X = (Xmax + Xmin)/2;   
		// Save parameters in EE
		}
		if(ABS(Ymax - Ymin) > CalThreshold )
		{
		 
		Mag_UserCal_Offset_Y=  (Ymax + Ymin)/2;
		//Save parameters in EE
		}
		if(ABS(Zmax - Zmin) > CalThreshold )
		{
		Mag_UserCal_Offset_Z =  (Zmax +Zmin)/2;   
		// Save parameters in EE
		}
}
void Hard_Iron_Correction( int MagX, int MagY, int  MagZ)   // call this function for correction
		{
		MagX -= Mag_UserCal_Offset_X;
		MagY -= Mag_UserCal_Offset_Y;
		MagZ -= Mag_UserCal_Offset_Z;
}
/************************************************************   
* 函数名:HMC5883L_Self_Test   
* 描述 : 自测磁场强度求比例因子 
* 输入  :无   
* 输出  :无    
*/
void HMC5883L_Self_Test(void)//读取
{
  u8 BUF1[7]={0};
	char i=0;
	int	x1[10],y1[10],z1[10];
	
	
	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterA,0x15);   //配置寄存器A：采样平均数1 输出速率30Hz 自测模式
	//I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterB,0x20);   //配置寄存器B：增益控制
	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister,0x01);   //模式寄存器：单一测量模式
	
	delay_ms(5);
	
  BUF1[1]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_MSB);//OUT_X_H
	BUF1[2]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_LSB);//OUT_X_L

	BUF1[3]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_MSB);//OUT_Y_L_A
	BUF1[4]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_LSB);//OUT_Y_H_A
	
  BUF1[5]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_MSB);//OUT_Z_L_A
	BUF1[6]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_LSB);//OUT_Z_H_A
	
	Magn_x=(BUF1[1] << 8) | BUF1[2]; //Combine MSB and LSB of X Data output register
	Magn_y=(BUF1[3] << 8) | BUF1[4]; //Combine MSB and LSB of Z Data output register
  Magn_z=(BUF1[5] << 8) | BUF1[6]; //Combine MSB and LSB of Z Data output register
	if(Magn_x>0x7fff) Magn_x-=0xffff;	  
  if(Magn_y>0x7fff) Magn_y-=0xffff;
  if(Magn_z>0x7fff) Magn_z-=0xffff;		
  angle_D= atan2((double)Magn_y,(double)Magn_x) * (180 / 3.14159265) + 180; // angle in degrees
	
	printf("\r纯净的地磁测量值：x=%d,y=%d,z=%d ",Magn_x,Magn_y,Magn_z);
	printf("\r angle=%f \r\n",angle_D);
//   while(i<10)
//	{
//	//Read_HMC5883L();
//		delay_ms(5);

//	BUF1[1]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_MSB);//OUT_X_H
//	BUF1[2]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_LSB);//OUT_X_L

//	BUF1[3]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_MSB);//OUT_Y_L_A
//	BUF1[4]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_LSB);//OUT_Y_H_A
//	
//  BUF1[5]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_MSB);//OUT_Z_L_A
//	BUF1[6]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_LSB);//OUT_Z_H_A
//	
//	Magn_x=(BUF1[1] << 8) | BUF1[2]; //Combine MSB and LSB of X Data output register
//	Magn_y=(BUF1[3] << 8) | BUF1[4]; //Combine MSB and LSB of Z Data output register
//  Magn_z=(BUF1[5] << 8) | BUF1[6]; //Combine MSB and LSB of Z Data output register
//		
//		
//	x1[i] = Magn_x;
//	y1[i] = Magn_y;
//	z1[i] = Magn_z;
//	Calibrate(Magn_x,Magn_y,Magn_z);
//	i++;
//	}
//	printf("\r Magn_x_Min=%d,Magn_y_Min=%d,Magn_z_Min=%d /r/n Magn_x_Max=%d,Magn_y_Max=%d,Magn_z_Max=%d",Magn_x_Min,Magn_y_Min,Magn_z_Min,Magn_x_Max,Magn_y_Max,Magn_z_Max);
//	HMC58X3_Offset();//计算偏移
//	 printf("\r X_Offset=%f,Y_Offset=%f,Z_Offset=%f ",X_Offset,Y_Offset,Z_Offset);
//	for(i=0;i<10;i++)
//	{
//	X_scale += x1[i]; 							   
//	Y_scale += y1[i];							   
//	Z_scale += z1[i];
//	}
//	X_scale /=10;							   //1.16Ga
//	Y_scale /=10;							   //1.16Ga
//	Z_scale /=10;							   //1.08Ga
//	
	
}

/************************************************************   
* 函数名:Read_HMC5883L   
* 描述 : 读取磁场强度 
* 输入  :无   
* 输出  :无    
*/
void Read_HMC5883L(void)//读取
{
	u8 BUF1[7]={0};
	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterA,0x14);   //配置寄存器A：采样平均数1 输出速率30Hz 正常测量
	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterB,0x20);   //配置寄存器B：增益控制
	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister,0x00);   //模式寄存器：连续测量模式
	
	delay_ms(5);

	BUF1[1]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_MSB);//OUT_X_H
	BUF1[2]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_LSB);//OUT_X_L

	BUF1[3]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_MSB);//OUT_Y_L_A
	BUF1[4]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_LSB);//OUT_Y_H_A
	
  BUF1[5]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_MSB);//OUT_Z_L_A
	BUF1[6]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_LSB);//OUT_Z_H_A
	
	Magn_x=(BUF1[1] << 8) | BUF1[2]; //Combine MSB and LSB of X Data output register
	Magn_y=(BUF1[3] << 8) | BUF1[4]; //Combine MSB and LSB of Z Data output register
  Magn_z=(BUF1[5] << 8) | BUF1[6]; //Combine MSB and LSB of Z Data output register
//	Magn_x=Magn_x-X_Offset;
//	Magn_y=Magn_y-Y_Offset;
//	Magn_z=Magn_z-Z_Offset;
  Magn_x=Magn_x;
	Magn_y=Magn_y;
	//Magn_y=(Magn_y*HMC5883L_GAIN_Y)/10000;
  if(Magn_x>0x7fff) Magn_x-=0xffff;	  
  if(Magn_y>0x7fff) Magn_y-=0xffff;
  if(Magn_z>0x7fff) Magn_z-=0xffff;		
  angle_D= atan2((double)Magn_y,(double)Magn_x) * (180 / 3.14159265) + 180; // angle in degrees
	angle_D=angle_D+100;
	if(angle_D>360)
		angle_D=angle_D-360;
  printf("   x=%d,y=%d,z=%d ",Magn_x,Magn_y,Magn_z);
	printf("   angle=%f ",angle_D);
}



