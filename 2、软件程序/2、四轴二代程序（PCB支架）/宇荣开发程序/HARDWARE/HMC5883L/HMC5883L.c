#include "HMC5883L.h"
#include "IIC.h"
#include "delay.h"
#include "math.h"
#include "mpu6050.h"


extern float Mx,My,Mz;


/*��������----------------------------------------------------------------*/
int Magn_x_Max,Magn_y_Max,Magn_z_Max,Magn_x_Min,Magn_y_Min,Magn_z_Min;    //5883�õ�
float angle_D;   //5883 ��ָ����
float X_Offset,Y_Offset,Z_Offset,X_scale,Y_scale,Z_scale;
int Xmax=-10000, Xmin=10000, Ymax=-10000, Ymin=10000, Zmax=-10000, Zmin=10000;
int Mag_UserCal_Offset_X,Mag_UserCal_Offset_Y,Mag_UserCal_Offset_Z;
/*----------------------------------------------------------   
  ������:HMC58X3_Offset 
  ���� : ����X,Y,Z��ƫ��,��HMC5883L_Self_Test��������
  ����  :��   
  ���  :��    
-----------------------------------------------------------*/
void HMC58X3_Offset(void)					 //������ƫ
{
//	X_Offset   = (Magn_x_Max+Magn_x_Min)/2.0;
//	Y_Offset   = (Magn_y_Max+Magn_y_Min)/2.0;
//	Z_Offset   = (Magn_z_Max+Magn_z_Min)/2.0;		
	
	X_Offset   = (Xmax+Xmin)/2.0;
	Y_Offset   = (Ymax+Ymin)/2.0;
	Z_Offset   = (Zmax+Zmin)/2.0;	
}

/************************************************************   
* ������:Calibrate 
* ���� : ����X,Y,Z����ֵ,��HMC5883L_Self_Test��������
* ����  :��   
* ���  :��    
*************************************************************/
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
			if (MagXreading > Xmax) Xmax = MagXreading;
			if (MagXreading < Xmin) Xmin = MagXreading;
			if (MagYreading > Ymax ) Ymax = MagYreading;
			if (MagYreading < Ymin ) Ymin = MagYreading;
			if (MagZreading > Zmax ) Zmax = MagZreading;
			if (MagZreading < Zmin ) Zmin = MagZreading;
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
* ������:HMC5883L_Self_Test   
* ���� : �Բ�ų�ǿ����������� 
* ����  :��   
* ���  :��    
************************************************************/
void HMC5883L_Self_Test(void)//��ȡ
{
	int Magn_x,Magn_y,Magn_z;
	
  u8 BUF1[7]={0};
	char i=0;
	int	x1[10],y1[10],z1[10];
		
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterA,0x15);   //���üĴ���A��MS0��1�������Բ�ģʽ
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterB,0x20);   //���üĴ���B���������
//	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister,0x01);   //ģʽ�Ĵ�������һ����ģʽ	
//	delay_ms(5);
//	
//  BUF1[1]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_MSB);//OUT_X_H
//	BUF1[2]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_LSB);//OUT_X_L
//	BUF1[3]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_MSB);//OUT_Y_L_A
//	BUF1[4]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_LSB);//OUT_Y_H_A
//  BUF1[5]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_MSB);//OUT_Z_L_A
//	BUF1[6]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_LSB);//OUT_Z_H_A
//	
//	Magn_x=(BUF1[1] << 8) | BUF1[2]; //Combine MSB and LSB of X Data output register
//	Magn_y=(BUF1[3] << 8) | BUF1[4]; //Combine MSB and LSB of Z Data output register
//  Magn_z=(BUF1[5] << 8) | BUF1[6]; //Combine MSB and LSB of Z Data output register
//	if(Magn_x>0x7fff) Magn_x-=0xffff;	  
//  if(Magn_y>0x7fff) Magn_y-=0xffff;
//  if(Magn_z>0x7fff) Magn_z-=0xffff;		

//  angle_D= atan2((double)Magn_y,(double)Magn_x) * (180 / 3.14159265) + 180; // angle in degrees
//	printf("\r�����ĵشŲ���ֵ��x=%d,y=%d,z=%d ",Magn_x,Magn_y,Magn_z);
//	printf("\r angle=%f \r\n",angle_D);

	for(i=0;i<10;i++)
	{
		delay_ms(5);
		
		I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterA,0x14);   //���üĴ���A������ƽ����1 �������30Hz ��������
		I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterB,0xA0);   //���üĴ���B���������
		I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister,0x01);  
		
		BUF1[1]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_MSB);//OUT_X_H
		BUF1[2]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_LSB);//OUT_X_L
		BUF1[3]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_MSB);//OUT_Y_L_A
		BUF1[4]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_LSB);//OUT_Y_H_A
		BUF1[5]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_MSB);//OUT_Z_L_A
		BUF1[6]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_LSB);//OUT_Z_H_A
		
		Magn_x=(BUF1[1] << 8) | BUF1[2]; //Combine MSB and LSB of X Data output register
		Magn_y=(BUF1[3] << 8) | BUF1[4]; //Combine MSB and LSB of Z Data output register
		Magn_z=(BUF1[5] << 8) | BUF1[6]; //Combine MSB and LSB of Z Data output register
			
//		if(Magn_x>0x7fff) Magn_x-=0xffff;	  
//		if(Magn_y>0x7fff) Magn_y-=0xffff;
//		if(Magn_z>0x7fff) Magn_z-=0xffff;		

		if(Magn_x>32768) Magn_x = -(0xFFFF - Magn_x + 1);
		if(Magn_z>32768) Magn_z = -(0xFFFF - Magn_z + 1);
		if(Magn_y>32768) Magn_y = -(0xFFFF - Magn_y + 1);
		
		x1[i] = Magn_x;
		y1[i] = Magn_y;
		z1[i] = Magn_z;
		Calibrate(Magn_x,Magn_y,Magn_z);
		
	}
	
//	printf("\r Magn_x_Min=%d,Magn_y_Min=%d,Magn_z_Min=%d /r/n Magn_x_Max=%d,Magn_y_Max=%d,Magn_z_Max=%d",Magn_x_Min,Magn_y_Min,Magn_z_Min,Magn_x_Max,Magn_y_Max,Magn_z_Max);
	HMC58X3_Offset();//����ƫ��
//	printf("\r X_Offset=%f,Y_Offset=%f,Z_Offset=%f ",X_Offset,Y_Offset,Z_Offset);
	
	for(i=0;i<10;i++)
	{
		X_scale += x1[i]; 							   
		Y_scale += y1[i];							   
		Z_scale += z1[i];
	}
	X_scale /=10;							   //1.16Ga
	Y_scale /=10;							   //1.16Ga
	Z_scale /=10;							   //1.08Ga
	
	Mx = (float)X_Offset;		//460		//954
	My = (float)Y_Offset;		//439		//838
	Mz = (float)Z_Offset;		//465		//766
	

}



/************************************************************   
* ������:Read_HMC5883L   
* ���� : ��ȡ�ų�ǿ�� 
* ����  :��   
* ���  :��    
************************************************************/
void Read_HMC5883L(float *mx,float *my,float *mz)//��ȡ
{
	float Magn_x,Magn_y,Magn_z;
	u8 BUF1[7]={0};	

	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterA,0x14);   //���üĴ���A������ƽ����1 �������30Hz ��������
	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterB,0xA0);   //���üĴ���B���������
	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister,0x00);   //ģʽ�Ĵ�������������ģʽ

	BUF1[1]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_MSB);//OUT_X_H
	BUF1[2]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_LSB);//OUT_X_L

	BUF1[3]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_MSB);//OUT_Y_L_A
	BUF1[4]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_LSB);//OUT_Y_H_A
	
  BUF1[5]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_MSB);//OUT_Z_L_A
	BUF1[6]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_LSB);//OUT_Z_H_A
	
	Magn_x=(BUF1[1] << 8) | BUF1[2]; //Combine MSB and LSB of X Data output register
	Magn_y=(BUF1[3] << 8) | BUF1[4]; //Combine MSB and LSB of Z Data output register
  Magn_z=(BUF1[5] << 8) | BUF1[6]; //Combine MSB and LSB of Z Data output register

//	if(Magn_x>0x7fff) Magn_x-=0xffff;	  
//  if(Magn_y>0x7fff) Magn_y-=0xffff;
//  if(Magn_z>0x7fff) Magn_z-=0xffff;		

		if(Magn_x>32768) Magn_x = -(0xFFFF - Magn_x + 1);
		if(Magn_z>32768) Magn_z = -(0xFFFF - Magn_z + 1);
		if(Magn_y>32768) Magn_y = -(0xFFFF - Magn_y + 1);
		
  
	Magn_x = Magn_x+875;
	Magn_y = Magn_x+921;
	Magn_z = Magn_z+40;
	
	slide_filter(&Magn_x,&Magn_y,&Magn_z);
	
	*mx = (float)Magn_y;		//������
	*my = (float)Magn_x;
	*mz = (float)Magn_z;

	Mx = (float)Magn_x;
	My = (float)Magn_y;
	Mz = (float)Magn_z;
	
	angle_D= atan2((double)Magn_y,(double)Magn_x) * (180 / 3.14159265)+ 180 ; // angle in degrees
	angle_D= angle_D+100;
	if(angle_D>360)	angle_D=angle_D-360;
}

/*---------------------------------
//	�������ܣ�HMC5883���
//	����0, �ɹ�
//	����1, ʧ��
-----------------------------------*/
u8 HMC5883L_Check(void)
{
	char ID_temp[3];
	
	//��ȡоƬID
  ID_temp[0]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_ID_A);  
  ID_temp[1]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_ID_B);
  ID_temp[2]=I2C_ReadByte(HMC5883L_Addr,HMC5883L_ID_C);
	
  //HMC�Ĺ̶�ID��Ϊ�����ֽڣ�16���Ʊ�ʾ�ֱ�Ϊ0x48,0x34,0x33
  if((ID_temp[0]==0x48)&&(ID_temp[1]==0x34)&&(ID_temp[2]==0x33)) return 0;
	else return 1;
	
}

/**************************************************
//	����HMC5883�Ĺ���ģʽ
***************************************************/
void HMC58X3_setMode(unsigned char mode) {
  if (mode > 2) {
    return;
  }
  I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister, mode);
  delay_us(100);
}

/*----------------------------------------
  ��������: ����5883L�������������
 	�������������ֵ
		0 -> 0.75Hz 	| 	1 -> 1.5Hz	|	2 -> 3Hz  
	  3 -> 7.5Hz		|		4 -> 15Hz   |	5 -> 30Hz
		6 -> 75Hz  
	�����������
-------------------------------------------*/
void HMC58X3_setDOR(unsigned char DOR) {
  if (DOR>6) return;
  I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister,DOR<<2);
}

/*-------------------------------------
 	�������ܣ���ʼ��������
  �����������
  ���������0,��ʼ���ɹ�; 1,��ʼ��ʧ��
---------------------------------------*/
u8 HMC5883L_Init(void)
{
	u8 t;
	I2C_WriteByte( MPU_WRITE,MPU_INTBP_CFG_REG,0X42);			//���ô����ƶ�ȡΪpassbyģʽ������оƬֱ�Ӷ�ȡ������
	if(HMC5883L_Check()) return 1;	//��������
//	I2C_WriteByte(HMC5883L_Addr, HMC5883L_ConfigurationRegisterA, 0x70); // 8 samples averaged, 75Hz frequency, no artificial bias.
//  I2C_WriteByte(HMC5883L_Addr, HMC5883L_ConfigurationRegisterB, 0xA0);
//  I2C_WriteByte(HMC5883L_Addr, HMC5883L_ModeRegister,  0x00);
	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterA,0x14);   //���üĴ���A������ƽ����1 �������30Hz ��������
	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterB,0xA0);   //���üĴ���B���������		
	I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister,0x00);   //ģʽ�Ĵ�������������ģʽ
	HMC58X3_setMode(0);
	HMC58X3_setDOR(6);
	
	for(t=0; t<100; t++)		//ȥ��ǰ��������
	{
		I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterA,0x14);   
		I2C_WriteByte(HMC5883L_Addr,HMC5883L_ConfigurationRegisterB,0xA0);  
		I2C_WriteByte(HMC5883L_Addr,HMC5883L_ModeRegister,0x01);  
		I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_MSB);//OUT_X_H
		I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_X_LSB);//OUT_X_L
		I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_MSB);//OUT_Y_L_A
		I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Y_LSB);//OUT_Y_H_A
		I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_MSB);//OUT_Z_L_A
		I2C_ReadByte(HMC5883L_Addr,HMC5883L_Output_Z_LSB);//OUT_Z_H_A
	}
	return 0;
}

void slide_filter( float *magn_x, float *magn_y, float *magn_z)
{
	static float magn[3][10];
	static int count=0;
	static float sum[3];
	u8 t;
	
	magn[0][count] = *magn_x;
	magn[1][count] = *magn_y;
	magn[2][count] = *magn_z;
	
	count++;
	if(count==10) count=0;
	
	sum[0] = 0;
	sum[1] = 0;
	sum[2] = 0;
	
	for(t=0; t<10; t++)
	{
		sum[0] += magn[0][t];
		sum[1] += magn[1][t];
		sum[2] += magn[2][t];
	}
	
	*magn_x = sum[0]/10.0;
	*magn_y = sum[1]/10.0;
	*magn_z = sum[2]/10.0;
	
}





