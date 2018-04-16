#include "IIC.h"
#include "delay.h"

/************************************************************   
* ������:IIC_GPIO_Configuration   
* ���� : ����PB6ΪSCL PB7ΪSDA  
* ����  :��   
* ���  :��    
*/
void I2C_GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//��ʹ������IO PORTBʱ�� 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SCL|GPIO_Pin_SDA;	 // �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //��©���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIO 
  GPIO_SetBits(GPIOB,GPIO_Pin_SCL|GPIO_Pin_SDA);						 //PB10,PB11 �����	
 
}

/************************************************************   
* ������:delay2,delay_nop   
* ���� :iicʱ����ʱ��
* ���� :��  
* ��� :��  
*/
void delay2(unsigned int x)
{
   unsigned int i;
   for(i=0;i<x;i++);
}

void delay_nop(void)
{
  unsigned int i=10; //i=10��ʱ1.5us//��������Ż��ٶ� ����������͵�5����д��
   while(i--);

}

/************************************************************   
* ������:iic_start   
* ���� :��ʼʱ��
* ���� :��  
* ��� :��  
************************************************************/
void iic_start(void)
{
   //SDA=1;
   IIC_SDA = 1;
   delay_nop();
   //SCL=1;
   IIC_SCL = 1;
   delay_nop();
   //SDA=0;
   IIC_SDA = 0;
   delay_nop();
   //SCL=0;
   IIC_SCL = 0;
   delay_nop();
}
/************************************************************   
* ������:iic_stop   
* ���� :ֹͣʱ��
* ���� ����  
* ��� :��  
************************************************************/

void iic_stop(void)
{
	IIC_SCL = 0;
	delay_nop();
	IIC_SDA = 0;
	delay_nop();
	IIC_SCL = 1;
	delay_nop();
	IIC_SDA = 1;
	delay_nop();
}

/************************************************************   
* ������:iic_writex   
* ���� : д�ֽ�
* ���� ��Ҫд���ֽ�   
* ��� :��  
*************************************************************/
void iic_writex(unsigned char j)
{
   unsigned char i,temp,temp1;

   temp=j;
   for (i=0;i<8;i++)
   {
      temp1=temp & 0x80;
      temp=temp<<1; 
			//SCL=0;
			IIC_SCL = 0;
			delay_nop();
			//SDA=CY;
			if(temp1==0x80) IIC_SDA = 1;
			else IIC_SDA = 0;
			delay_nop();
			// SCL=1;
			IIC_SCL = 1;
			delay_nop();
   }
   //SCL=0;
   IIC_SCL = 0;
   delay_nop();
   //SDA=1;
   IIC_SDA = 1;
   delay_nop();
	 
}
/************************************************************   
* ������:iic_readx   
* ���� : ��ȡ�ֽ�
* ����  :��   
* ���  :��ȡ���ֽ�    
*************************************************************/
unsigned char iic_readx(void)
{
	unsigned char i,j,k=0;

	
	//SCL=0;
	IIC_SCL = 0;
	delay_nop(); 
	//SDA=1;
	IIC_SDA = 1;
	iic_SDA_Set_Dir(1);
	for (i=0;i<8;i++)
	{
		delay_nop();
		//SCL=1;
		IIC_SCL = 1;
		delay_nop();
		//if (SDA==1) j=1;
		if( SDA_READ==1 ) 
		{j=1;}
		else 
		{j=0;}
		k=(k<<1)|j;
		//SCL=0;
		IIC_SCL = 0; 
		}
		iic_SDA_Set_Dir(0);
		delay_nop();
		return(k);

}
/************************************************************   
* ������:iic_check_ACK   
* ���� : ���ӻ�Ӧ���ź�
* ����  :��   
* ���  :��    
*************************************************************/
u8 iic_check_ACK(void)//���ӻ�Ӧ���ź�
{
   unsigned int i=0;
   iic_SDA_Set_Dir(1);
   //SCL=1;
   IIC_SCL = 1;
   delay_nop();
   while (SDA_READ==1)
	 {
		 i++;
		 if(i>250)
		 {
			 iic_stop();
			 return 1;
		 }
	 }
   //SCL=0;
   IIC_SCL = 0;
   delay_nop();
   iic_SDA_Set_Dir(0);
	 return 0;  

}
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack(void)
{	
	IIC_SCL = 0;
  delay_nop();
	IIC_SDA = 0;
	delay_nop();
	IIC_SCL = 1;
	delay_nop();
	IIC_SCL = 0;
	delay_nop();
}   
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck(void)
{	
	IIC_SCL = 0;
	delay_nop();
	IIC_SDA = 1;
	delay_nop();
	IIC_SCL = 1;
	delay_nop();
	IIC_SCL = 0;
	delay_nop();
} 
/************************************************************   
* ������:I2C_ReadByte   
* ���� : ���ض��豸id���ض���ַ��ȡ����
* ����  :�豸id���ڲ���ַ   
* ���  :��ȡ������   
************************************************************/
unsigned char I2C_ReadByte(unsigned char DeviceAddr,unsigned char address)
{
   unsigned char i;
   iic_start();
   iic_writex(DeviceAddr);
   iic_check_ACK();
   iic_writex(address);
   iic_check_ACK();
   iic_start();
   iic_writex(DeviceAddr+1);
   iic_check_ACK();
   i=iic_readx();
   iic_stop();
   delay2(10);   	
   return(i);

}
/************************************************************   
* ������:I2C_WriteByte   
* ���� : ���ض��豸id���ض���ַ��д���ֽ� 
* ����  :�豸id���ڲ���ַ    
* ���  :��    
************************************************************/
u8 I2C_WriteByte(unsigned char DeviceAddr,unsigned char address,unsigned char info)
{

	iic_start();
	iic_writex(DeviceAddr);
	if(iic_check_ACK())		//�ȴ�ACK
	{
		iic_stop();	 
		return 1;		 
	}
	iic_writex(address);
	iic_check_ACK();
	iic_writex(info);
	if(iic_check_ACK())		//�ȴ�ACK
	{
		iic_stop();	 
		return 1;		 
	}
	iic_stop();
	//delay2(50);
	delay2(250);
	return 0;
}


//�������
void I2C_Noaddr_sendByte(u8 txd)
{
	u8 t;      
	IIC_SCL=0;//??????????
	for(t=0;t<8;t++)
	{              
	IIC_SDA=(txd&0x80)>>7;
	txd<<=1; 	  
	delay_us(1);   
	IIC_SCL=1;
	delay_us(1); 
	IIC_SCL=0;	
	delay_us(1);
	}	 
}
/************************************************************   
* ������:I2C_NoAddr_WriteByte   
* ���� : ���ض��豸id��д���ֽ� 
* ����  :�豸id������   
* ���  :��    
************************************************************/
void I2C_NoAddr_WriteByte(unsigned char DeviceAddr,unsigned char info)
{
   iic_start();
//   iic_writex(DeviceAddr);
	 I2C_Noaddr_sendByte(DeviceAddr);
	 iic_check_ACK();
//   iic_writex(info);
	 I2C_Noaddr_sendByte(info);
	 iic_check_ACK();
   iic_stop();
   //delay2(50);
   delay2(250);
}

/************************************************************   
* ������:iic_SDA_Set_Dir   
* ���� : SDA��������������� 
* ����  :��   
* ���  :��    
************************************************************/
void iic_SDA_Set_Dir(unsigned char io_set)
{
	if(io_set==0)		//�������
  {		
		SDA_OUT();		
  }
	else if(io_set==1)	//��������
  {
		SDA_IN();
  }

}

/************************************************************   
* ������:I2C_Read_Len   
* IIC������
* DeviceAddr:������ַ
* address:Ҫ��ȡ�ļĴ�����ַ
* len:Ҫ��ȡ�ĳ���(0����ȡ)
* buf:��ȡ�������ݴ洢��
*************************************************************/
//��ȡmpu6050���������������
u8 I2C_Read_Len(unsigned char DeviceAddr,unsigned char address,unsigned char len,unsigned char * buf )
{
   unsigned char i;
   iic_start();
   iic_writex(DeviceAddr);
   if(iic_check_ACK())		//�ȴ�ACK
		{
			iic_stop();	 
			return 1;		 
		}
   iic_writex(address);
   iic_check_ACK();
   iic_start();
   iic_writex(DeviceAddr+1);
   iic_check_ACK();
	 for(i=0;i<len;i++)
	 {
    buf[i]=iic_readx();
		if(i+1<len)  I2C_Ack();
		else	I2C_NoAck();//���һ���ֽ�����Ӧ��
	 }
   iic_stop();
   //delay2(10);
   delay2(250);
	 return 0;	
}
/************************************************************   
* ������:I2C_Write_Len   
* IIC����д
* DeviceAddr:������ַ 
* address:�Ĵ�����ַ
* len:д�볤��
* buf:������
* ����ֵ:0,����
* ����,�������
*************************************************************/
u8 I2C_Write_Len(unsigned char DeviceAddr,unsigned char address,unsigned char len,unsigned char *buf)
{
	u8 i; 
  iic_start(); 
	iic_writex(DeviceAddr);	
	if(iic_check_ACK())		//�ȴ�ACK
	{
			iic_stop();	 
			return 1;		 
	}
  iic_writex(address);
  iic_check_ACK();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		iic_writex(buf[i]);	//��������
		if(iic_check_ACK())		//�ȴ�ACK
		{
			iic_stop();	 
			return 1;		 
		}
	}    
  iic_stop(); 
	return 0;
} 

//ΪMS5611д
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA?????
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(1);
				IIC_SCL=1;
        receive<<=1;
        if(SDA_READ)receive++;   
		delay_us(1); 
    }					 
    if (ack)
        I2C_Ack(); 
    else
        I2C_NoAck();
    return receive;
}

