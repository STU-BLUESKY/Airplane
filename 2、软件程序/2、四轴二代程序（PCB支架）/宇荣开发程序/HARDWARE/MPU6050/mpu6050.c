#include "mpu6050.h"
#include "IIC.h"
#include "HMC5883L.h"

struct _sensor sensor;
float z_NED;

/************************************
	��ʼ��MPU6050
	����ֵ:0,�ɹ�
  ����,�������
*******************************************/
u8 MPU_Init(void)
{ 
	u8 res;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ��AFIOʱ�� 
	
	I2C_GPIO_Configuration();			//��ʼ��IIC����
  delay_ms(100);
	I2C_WriteByte( MPU_WRITE,MPU_PWR_MGMT1_REG,0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(2);					//�����Ǵ�����,��1000dps
	MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
	MPU_Set_Rate(50);							//���ò�����50Hz
	I2C_WriteByte( MPU_WRITE,MPU_INT_EN_REG,0X00);	//�ر������ж�
	I2C_WriteByte( MPU_WRITE,MPU_USER_CTRL_REG,0X40);	//I2C��ģʽ�ر�
	I2C_WriteByte( MPU_WRITE,MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	I2C_WriteByte( MPU_WRITE,MPU_INTBP_CFG_REG,0X42);	//INT���ŵ͵�ƽ��Ч		
	res=I2C_ReadByte( MPU_WRITE,MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)
  { 
			I2C_WriteByte( MPU_WRITE,MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
			I2C_WriteByte( MPU_WRITE,MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
			MPU_Set_Rate(50);						//���ò�����Ϊ50Hz
		
		  GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    	return 0;
  }
	else
	{ 
		return 1;
	}	
}


//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return I2C_WriteByte( MPU_WRITE,MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return I2C_WriteByte( MPU_WRITE,MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return I2C_WriteByte( MPU_WRITE,MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}
//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=I2C_WriteByte( MPU_WRITE,MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	I2C_Read_Len(MPU_WRITE,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=I2C_Read_Len(MPU_WRITE,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
  u8 buf[6],res;  
	res=I2C_Read_Len(MPU_WRITE,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}

/*-------------------------------------------------
	 �������ܣ��ɼ�MPU6050ԭʼ����
-------------------------------------------------*/
void MPU6050_Getdata(void)
{
	int16_t aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	int16_t gyrox,gyroy,gyroz;	//������ԭʼ����
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
	
	sensor.acc.origin.x =	aacx-sensor.acc.quiet.x;
	sensor.acc.origin.y =	aacy-sensor.acc.quiet.y;
	sensor.acc.origin.z =	aacz-sensor.acc.quiet.z;												//z�᲻��Ҫ����̬ƫ��
	sensor.gyro.origin.x = gyrox;
	sensor.gyro.origin.y = gyroy;
	sensor.gyro.origin.z = gyroz;
														
	sensor.gyro.cal.x = (sensor.gyro.origin.x-sensor.gyro.quiet.x)/GyroAxis_Sensitive ;
	sensor.gyro.cal.y = (sensor.gyro.origin.y-sensor.gyro.quiet.y)/GyroAxis_Sensitive ;
	sensor.gyro.cal.z = (sensor.gyro.origin.z-sensor.gyro.quiet.z)/GyroAxis_Sensitive ;	
	
	sensor.gyro.radian.x = sensor.gyro.cal.x*AtR;
	sensor.gyro.radian.y = sensor.gyro.cal.y*AtR;
	sensor.gyro.radian.z = sensor.gyro.cal.z*AtR;	
	
}


/*-------------------------------------
		�������ܣ���ȡ��ǰ��̬��
-------------------------------------*/
void Get_Attitude(void)
{
	MPU6050_Getdata();
	
	sensor.acc.averag.x = KalmanFilter_x(sensor.acc.origin.x,KALMAN_Q,KALMAN_R);
	sensor.acc.averag.y = KalmanFilter_y(sensor.acc.origin.y,KALMAN_Q,KALMAN_R);
	sensor.acc.averag.z = KalmanFilter_z(sensor.acc.origin.z,KALMAN_Q,KALMAN_R);
	
	sensor.acc.cal.x  = sensor.acc.averag.x/AccAxis_Sensitive *10.0;
	sensor.acc.cal.y  = sensor.acc.averag.y/AccAxis_Sensitive *10.0;
	sensor.acc.cal.z  = sensor.acc.averag.z/AccAxis_Sensitive *10.0;				
	
	//Read_HMC5883L(&sensor.magnet.x,&sensor.magnet.y,&sensor.magnet.z);	//������
	Angle_Calculate(sensor.gyro.radian.x,
									sensor.gyro.radian.y,
									sensor.gyro.radian.z,
									sensor.acc.averag.x,
									sensor.acc.averag.y,
									sensor.acc.averag.z);		
	z_NED = DCMgb[0][2]* sensor.acc.cal.x + DCMgb[1][2]*sensor.acc.cal.y + DCMgb[2][2]*sensor.acc.cal.z;		
	
}

/*-------------------------------
		�������ܣ��ɼ���������ƫ
--------------------------------*/
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
			  MPU6050_Getdata();	
			 
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
		  MPU6050_Getdata();
	
      tempgx += sensor.gyro.origin.x;
		  tempgy += sensor.gyro.origin.y;
		  tempgz += sensor.gyro.origin.z;
     }

	 sensor.gyro.quiet.x = tempgx/2000;
	 sensor.gyro.quiet.y = tempgy/2000;
	 sensor.gyro.quiet.z = tempgz/2000;	
}
/*-------------------------------
		�������ܣ�У׼���ٶȼ�
--------------------------------*/
void Acc_OFFSET(void)
{
		int32_t	tempax=0,tempay=0,tempaz=0;
		uint16_t cnt_a=200;
		sensor.acc.quiet.x = 0;
		sensor.acc.quiet.y = 0;
		sensor.acc.quiet.z = 0;

		while(cnt_a--)
		{
				MPU6050_Getdata();
				tempax+= sensor.acc.origin.x;
				tempay+= sensor.acc.origin.y;
				tempaz+= sensor.acc.origin.z;						
		}	
		sensor.acc.quiet.x = tempax/200;
		sensor.acc.quiet.y = tempay/200;
		sensor.acc.quiet.z = tempaz/200 - CONSTANTS_ONE_G/10*AccAxis_Sensitive; 	//�����������ٶȳ���10�����Բ���������
			
}

/*-------------------------------------
		�������ܣ�KalmanFilter
		����˵����X��,Y��,Z�� ԭʼ���ݿ������˲�
-------------------------------------*/
static double KalmanFilter_x(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
   double R = MeasureNoise_R;
   double Q = ProcessNiose_Q;
   static double x_last;
   double x_mid = x_last;
   double x_now;
   static double p_last;
   double p_mid ;
   double p_now;
   double kg;        

   x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
   p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
   kg=p_mid/(p_mid+R); //kgΪkalman filter��RΪ����
   x_now=x_mid+kg*(ResrcData-x_mid);//���Ƴ�������ֵ
                
   p_now=(1-kg)*p_mid;//����ֵ��Ӧ��covariance       
   p_last = p_now; //����covarianceֵ
   x_last = x_now; //����ϵͳ״ֵ̬
   return x_now;                
}

static double KalmanFilter_y(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
   double R = MeasureNoise_R;
   double Q = ProcessNiose_Q;
   static double x_last;
   double x_mid = x_last;
   double x_now;
   static double p_last;
   double p_mid ;
   double p_now;
   double kg;        

   x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
   p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
   kg=p_mid/(p_mid+R); //kgΪkalman filter��RΪ����
   x_now=x_mid+kg*(ResrcData-x_mid);//���Ƴ�������ֵ
                
   p_now=(1-kg)*p_mid;//����ֵ��Ӧ��covariance       
   p_last = p_now; //����covarianceֵ
   x_last = x_now; //����ϵͳ״ֵ̬
   return x_now;                
}

static double KalmanFilter_z(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
   double R = MeasureNoise_R;
   double Q = ProcessNiose_Q;
   static double x_last = CONSTANTS_ONE_G/10*AccAxis_Sensitive; //z�ḳ���ʼֵ���ӿ�z���ʼ���ı仯
   double x_mid = x_last;
   double x_now;
   static double p_last;
   double p_mid ;
   double p_now;
   double kg;        

   x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
   p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
   kg=p_mid/(p_mid+R); //kgΪkalman filter��RΪ����
   x_now=x_mid+kg*(ResrcData-x_mid);//���Ƴ�������ֵ
                
   p_now=(1-kg)*p_mid;//����ֵ��Ӧ��covariance       
   p_last = p_now; //����covarianceֵ
   x_last = x_now; //����ϵͳ״ֵ̬
   return x_now;                
}

