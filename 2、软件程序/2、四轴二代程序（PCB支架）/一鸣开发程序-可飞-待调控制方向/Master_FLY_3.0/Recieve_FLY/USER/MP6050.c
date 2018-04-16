/*����ͷ------------------------------------------------------------------*/
#include "MP6050.h"
#include "math.h"
#include "sys.h"
#define PI 3.14159265
/*�궨��------------------------------------------------------------------*/

#define	MPU6050_Addr   0xD0	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	M_CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)  0x1B
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)

#define INT_PIN_CFG     0x37    //������·��Ч ��ֵ��0x42 AUX_DA�ĸ���I2C
#define USER_CTRL       0x6A    //�û����üĴ��� ��ֵ��0x40  AUX_DA�ĸ���I2C

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)  
//QYQ ORZ_ORZ_ORZ_OrZ_//
#define Single_Read I2C_ReadByte 
#define MPU6050_ADDRESS MPU6050_Addr 
struct _sensor sensor;
extern uint32_t Pressure,qqp;
extern int32_t SpeedZ;


//QYQ ORZ_ORZ_ORZ_OrZ_//
//***************************************************************/


/*��������----------------------------------------------------------------*/
u8 TX_DATA[4];  	 //��ʾ�ݻ�����
u8 BUF[14];       //�������ݻ�����		 		
float GYRO_XOUT,GYRO_YOUT,GYRO_ZOUT,ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT,MP6050_Temperature;//X,Y,Z�ᣬ�¶�	
float Accel_Angle_X,Accel_Angle_Y,Accel_Angle_Z;
u8 accCorrect_flag=1;  // ���ݽ��ձ�־  ����Ϊ1=ok




/*��������----------------------------------------------------------------*/
//extern void delay_init();
extern void delay_ms(uint16_t nms);
extern void delay_us(uint32_t nus);


extern void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...) ;

extern void I2C_GPIO_Configuration(void);
extern unsigned char I2C_ReadByte(unsigned char DeviceAddr,unsigned char address);  //��24c02�ĵ�ַaddress�ж�ȡһ���ֽ�����
extern void I2C_WriteByte(unsigned char DeviceAddr,unsigned char address,unsigned char info);
extern void I2C_Read_MultiBytes(unsigned char DeviceAddr,unsigned char address,unsigned char BytesNum,unsigned char * OutDate );

void Init_MPU6050(void);
void READ_MPU6050_Gyro(void);
void READ_MPU6050_Accel(void);
void MPU6050_WHO_AM_I(void);

/* Private functions------------------------------------------------------*/


/************************************************************   
* ������:Init_MPU6050   
* ���� ����ʼ��MPU6050��������Ҫ��ο�pdf�����޸�
* ���� :��   
* ��� :��    
*/
void Init_MPU6050(void)
{

  I2C_WriteByte(MPU6050_Addr,PWR_MGMT_1, 0x00);	//�������״̬
	
	
	I2C_WriteByte(MPU6050_Addr,SMPLRT_DIV, 0x07);    //�����ǲ�����
	I2C_WriteByte(MPU6050_Addr,M_CONFIG, 0x03);        //5Hz 
	
	I2C_WriteByte(MPU6050_Addr,INT_PIN_CFG, 0x42);   //ʹ����·I2C
	I2C_WriteByte(MPU6050_Addr,USER_CTRL, 0x40);     //ʹ����·I2C
	
	I2C_WriteByte(MPU6050_Addr,GYRO_CONFIG, 0x10);   
	I2C_WriteByte(MPU6050_Addr,ACCEL_CONFIG, 0x09);
	
}
	
/************************************************************   
* ������:MP6050_WHO_AM_I   
* ���� ����ȡMPU6050�豸��Ϣ
* ���� :��   
* ��� :��    
*/
void MPU6050_WHO_AM_I(void)
{
	u8 dev=0;
		if(dev=I2C_ReadByte(MPU6050_Addr,WHO_AM_I),dev==0x68)
  { 
		  GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    	//printf("\r�豸MP6050ʶ��ɹ���id=0x%x\r\n\r",dev);
  }
	  else{ //printf("\r����!�޷�����豸MP6050��id=0x%x\r\n\r",dev);
			}
}

	
/************************************************************   
* ������:READ_MPU6050_Gyro   
* ���� : ��ȡMPU6050����������
* ����  :��   
* ���  :��    

void READ_MPU6050_Gyro(void)
{
   BUF[0]=I2C_ReadByte(MPU6050_Addr,GYRO_XOUT_L); 
   BUF[1]=I2C_ReadByte(MPU6050_Addr,GYRO_XOUT_H);
   GYRO_XOUT=	(BUF[1]<<8)|BUF[0];
	 if(GYRO_XOUT>32768)  GYRO_XOUT-=65536;
   GYRO_XOUT/=16.4; 						   //��ȡ����X������

   BUF[2]=I2C_ReadByte(MPU6050_Addr,GYRO_YOUT_L);
   BUF[3]=I2C_ReadByte(MPU6050_Addr,GYRO_YOUT_H);
   GYRO_YOUT=	(BUF[3]<<8)|BUF[2];
	 if(GYRO_YOUT>32768)  GYRO_YOUT-=65536;
   GYRO_YOUT/=16.4; 						   //��ȡ����Y������
   BUF[4]=I2C_ReadByte(MPU6050_Addr,GYRO_ZOUT_L);
   BUF[5]=I2C_ReadByte(MPU6050_Addr,GYRO_ZOUT_H);
   GYRO_ZOUT=	(BUF[5]<<8)|BUF[4];
	 if(GYRO_ZOUT>32768)  GYRO_ZOUT-=65536;
   GYRO_ZOUT/=16.4; 					       //��ȡ����Z������

	
  // BUF[6]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_L); 
  // BUF[7]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_H); 
  // T_T=(BUF[7]<<8)|BUF[6];
  // T_T = 35+ ((double) (T_T + 13200)) / 280;// ��ȡ������¶�
}
*/
/************************************************************   
* ������:READ_MPU6050_Accel   
* ���� : ��ȡMPU6050���ٶ�����
* ����  :��   
* ���  :��    
*/
/*
void READ_MPU6050_Accel(void)
{
	BUF[0]=I2C_ReadByte(MPU6050_Addr,ACCEL_XOUT_L); 
	BUF[1]=I2C_ReadByte(MPU6050_Addr,ACCEL_XOUT_H);
	ACCEL_XOUT=	(BUF[1]<<8)|BUF[0];
	if(ACCEL_XOUT>32768)  ACCEL_XOUT-=65536;
 
	//ACCEL_XOUT+=(float)((float)ACCEL_XOUT/16384)*10; 		//����100��	       //��ȡ����X������

	BUF[2]=I2C_ReadByte(MPU6050_Addr,ACCEL_YOUT_L);
	BUF[3]=I2C_ReadByte(MPU6050_Addr,ACCEL_YOUT_H);
	ACCEL_YOUT=	(BUF[3]<<8)|BUF[2];
	if(ACCEL_YOUT>32768)  ACCEL_YOUT-=65536;
	//Accel_Angle=atan2(Accel_Y,Accel_Z)*180/PI; 
	//ACCEL_YOUT+=(float)((float)ACCEL_YOUT/16384)*10; 						   //��ȡ����Y������
   
	BUF[4]=I2C_ReadByte(MPU6050_Addr,ACCEL_ZOUT_L);
	BUF[5]=I2C_ReadByte(MPU6050_Addr,ACCEL_ZOUT_H);
	ACCEL_ZOUT=	(BUF[5]<<8)|BUF[4];
	if(ACCEL_ZOUT>32768)  ACCEL_ZOUT-=65536;
	//Accel_Angle=atan2(Accel_Y,Accel_Z)*180/PI; 
	//ACCEL_ZOUT+=(float)((float)ACCEL_ZOUT/16384)*10; 					       //��ȡ����Z������
  Accel_Angle_X=atan2(ACCEL_YOUT,ACCEL_ZOUT)*180/PI;
	Accel_Angle_Y=atan2(ACCEL_XOUT,ACCEL_ZOUT)*180/PI;
	Accel_Angle_Z=atan2(ACCEL_XOUT,ACCEL_YOUT)*180/PI;
	
	BUF[6]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_L); 
	BUF[7]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_H); 
	MP6050_Temperature=(BUF[7]<<8)|BUF[6];
//	MP6050_Temperature = 35+ ((double) (T_T + 13200)) / 280;// ��ȡ������¶�
	MP6050_Temperature = (((double) MP6050_Temperature )/340+36.53)*10 ;//+36.53;  // ��ȡ������¶�
}
*/
///////////////////////////////////
/////////QYQ////////////////////



void MPU6050_Read(void)
{ 
	BUF[0]=Single_Read(MPU6050_ADDRESS, 0x3B);
	BUF[1]=Single_Read(MPU6050_ADDRESS, 0x3C);
	BUF[2]=Single_Read(MPU6050_ADDRESS, 0x3D);
	BUF[3]=Single_Read(MPU6050_ADDRESS, 0x3E);
	BUF[4]=Single_Read(MPU6050_ADDRESS, 0x3F);
	BUF[5]=Single_Read(MPU6050_ADDRESS, 0x40);
	BUF[8]=Single_Read(MPU6050_ADDRESS, 0x43);
	BUF[9]=Single_Read(MPU6050_ADDRESS, 0x44);
	BUF[10]=Single_Read(MPU6050_ADDRESS, 0x45);
	BUF[11]=Single_Read(MPU6050_ADDRESS, 0x46);
	BUF[12]=Single_Read(MPU6050_ADDRESS, 0x47);
	BUF[13]=Single_Read(MPU6050_ADDRESS, 0x48);
	
}




//��iic��ȡ�������ݷֲ�,������Ӧ�Ĵ���
void MPU6050_Dataanl(void)
{
	MPU6050_Read();
	
	sensor.acc.origin.x = ((((int16_t)BUF[0]) << 8) | BUF[1]) - sensor.acc.quiet.x;
	sensor.acc.origin.y = ((((int16_t)BUF[2]) << 8) | BUF[3]) - sensor.acc.quiet.y;
	sensor.acc.origin.z = ((((int16_t)BUF[4]) << 8) | BUF[5]);

	sensor.gyro.origin.x = ((((int16_t)BUF[8]) << 8) | BUF[9]);
	sensor.gyro.origin.y = ((((int16_t)BUF[10]) << 8)| BUF[11]);
	sensor.gyro.origin.z = ((((int16_t)BUF[12]) << 8)| BUF[13]);
  
	sensor.gyro.radian.x = sensor.gyro.origin.x * Gyro_Gr - sensor.gyro.quiet.x * Gyro_Gr;
	sensor.gyro.radian.y = sensor.gyro.origin.y * Gyro_Gr - sensor.gyro.quiet.y * Gyro_Gr;
	sensor.gyro.radian.z = sensor.gyro.origin.z * Gyro_Gr - sensor.gyro.quiet.z * Gyro_Gr;

////////////////////////////////////////////////////
//    	The calibration  of  acc        //
////////////////////////////////////////////////////	
	 if(accCorrect_flag)
	 {
		 static int32_t	tempax=0,tempay=0,tempaz=0;
		 static uint8_t cnt_a=0;
     //LED_ALLON();
		 if(cnt_a==0)
		 {
				sensor.acc.quiet.x = 0;
				sensor.acc.quiet.y = 0;
				sensor.acc.quiet.z = 0;
				tempax = 0;
				tempay = 0;
				tempaz = 0;
				cnt_a = 1;
		 }
				tempax+= sensor.acc.origin.x;
				tempay+= sensor.acc.origin.y;
				tempaz+= sensor.acc.origin.z;
				if(cnt_a==200)
				{
					sensor.acc.quiet.x = tempax/cnt_a;
					sensor.acc.quiet.y = tempay/cnt_a;
					sensor.acc.quiet.z = tempaz/cnt_a;
					cnt_a = 0;
					accCorrect_flag = 0;
					//EE_SAVE_ACC_OFFSET();//��������
					return;
				}
				cnt_a++;		
			}	
}


// QYQ QYQ
void Gyro_OFFSET(void)
{
   uint16_t cnt_g=0;
	 int32_t tempgx=100,tempgy=100,tempgz=100;
	 int16_t gx_last=0,gy_last=0,gz_last=0;
	 sensor.gyro.quiet.x=0;
	 sensor.gyro.quiet.y=0;
	 sensor.gyro.quiet.z=0;

	 while(tempgx>=100 || tempgy>=100 || tempgz>=100)	//��ѭ����ȷ�����ᴦ����ȫ��ֹ״̬
	 {
		 tempgx=0;tempgy=0;tempgz=0;cnt_g=100;
		 while(cnt_g--)
		 {
			  MPU6050_Read();
			 
			  sensor.gyro.origin.x = ((((int16_t)BUF[8]) << 8) | BUF[9]);
		    sensor.gyro.origin.y = ((((int16_t)BUF[10]) << 8)| BUF[11]);
		    sensor.gyro.origin.z = ((((int16_t)BUF[12]) << 8)| BUF[13]);
		
	      tempgx += FL_ABS(sensor.gyro.origin.x - gx_last);
			  tempgy += FL_ABS(sensor.gyro.origin.y - gy_last);
			  tempgz += FL_ABS(sensor.gyro.origin.z - gz_last);
	
			  gx_last = sensor.gyro.origin.x;
			  gy_last = sensor.gyro.origin.y;
			  gz_last =	sensor.gyro.origin.z;
	     }	 
	 }
	 tempgx=0;tempgy=0;tempgz=0;cnt_g=2000;

 	 while(cnt_g--)	 //��ѭ�����������Ǳ궨��ǰ���������Ѿ�������ȫ��ֹ״̬
	 {
		  MPU6050_Read();
		 
		  sensor.gyro.origin.x = ((((int16_t)BUF[8]) << 8) | BUF[9]);
	      sensor.gyro.origin.y = ((((int16_t)BUF[10]) << 8)| BUF[11]);
	      sensor.gyro.origin.z = ((((int16_t)BUF[12]) << 8)| BUF[13]);
	
          tempgx += sensor.gyro.origin.x;
		  tempgy += sensor.gyro.origin.y;
		  tempgz += sensor.gyro.origin.z;
     }

	 sensor.gyro.quiet.x = tempgx/2000;
	 sensor.gyro.quiet.y = tempgy/2000;
	 sensor.gyro.quiet.z = tempgz/2000;	
}



