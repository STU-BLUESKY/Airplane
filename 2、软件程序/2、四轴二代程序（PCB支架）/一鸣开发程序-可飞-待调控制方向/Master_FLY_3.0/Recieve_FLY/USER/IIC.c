#include "IIC.h"


unsigned char I2C_ReadByte(unsigned char DeviceAddr,unsigned char address);  //��24c02�ĵ�ַaddress�ж�ȡһ���ֽ�����
void I2C_WriteByte(unsigned char DeviceAddr,unsigned char address,unsigned char info);
void I2C_NoAddr_WriteByte(unsigned char DeviceAddr,unsigned char info);
void I2C_Read_MultiBytes(unsigned char DeviceAddr,unsigned char address,unsigned char BytesNum,unsigned char * OutDate );
uint16_t I2C_Read_2Bytes(unsigned char DeviceAddr,unsigned char address);
uint32_t I2C_Read_3Bytes(unsigned char DeviceAddr,unsigned char address);
void delay_nop(void);
void delay2(unsigned int x);
void iic_start(void);
void iic_stop(void);
void iic_writex(unsigned char j);
unsigned char iic_readx(void);
void iic_check_ACK(void);
void iic_SDA_Set_Dir(unsigned char io_set);
void I2C_GPIO_Configuration(void);




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
*/
void iic_start(void)
{
   //SDA=1;
   GPIO_SetBits(GPIOB,SDA);
   delay_nop();
   //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
   delay_nop();
   //SDA=0;
    GPIO_ResetBits(GPIOB, SDA);
   delay_nop();
   //SCL=0;
   GPIO_ResetBits(GPIOB, SCL);
   delay_nop();
}
/************************************************************   
* ������:iic_stop   
* ���� :ֹͣʱ��
* ���� ����  
* ��� :��  
*/
void iic_stop(void)
{
   //SDA=0;
   GPIO_ResetBits(GPIOB, SDA);
   delay_nop();
   //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
   delay_nop();
   //SDA=1;
   GPIO_SetBits(GPIOB,SDA);
   delay_nop();
}
/************************************************************   
* ������:iic_writex   
* ���� : д�ֽ�
* ���� ��Ҫд���ֽ�   
* ��� :��  
*/
void iic_writex(unsigned char j)

{
   unsigned char i,temp,temp1;

   temp=j;
  //iic_SDA_Set_Dir(0);
   for (i=0;i<8;i++)
   {
      temp1=temp & 0x80;
      temp=temp<<1;
     
      //SCL=0;
   GPIO_ResetBits(GPIOB, SCL);
      delay_nop();

   //SDA=CY;
  if(temp1==0x80)
   {GPIO_SetBits(GPIOB, SDA);}
  else
   {GPIO_ResetBits(GPIOB, SDA);}
     

      delay_nop();
     // SCL=1;
  GPIO_SetBits(GPIOB,SCL);
      delay_nop();
   }
 //iic_SDA_Set_Dir(0);
   //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
   delay_nop();
   //SDA=1;
   GPIO_SetBits(GPIOB,SDA);
   delay_nop();

}
/************************************************************   
* ������:iic_readx   
* ���� : ��ȡ�ֽ�
* ����  :��   
* ���  :��ȡ���ֽ�    
*/
unsigned char iic_readx(void)
{
   unsigned char i,j,k=0;

   //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
    delay_nop(); 
 //SDA=1;
 GPIO_SetBits(GPIOB,SDA);

 iic_SDA_Set_Dir(1);
   for (i=0;i<8;i++)
   {
      delay_nop();
      //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
      delay_nop();
      //if (SDA==1) j=1;
  if( GPIO_ReadInputDataBit(GPIOB,SDA)==1 ) 
   {j=1;}
      else 
   {j=0;}
      k=(k<<1)|j;
      //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
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
*/
void iic_check_ACK(void)//���ӻ�Ӧ���ź�
{
   unsigned int i=0;
      iic_SDA_Set_Dir(1);
   //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
   delay_nop();
   while ((GPIO_ReadInputDataBit(GPIOB,SDA)==1)&&(i<5000))i++;
   //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
   delay_nop();
   iic_SDA_Set_Dir(0);

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
	GPIO_ResetBits(GPIOB,SCL);
  delay_nop();
	GPIO_ResetBits(GPIOB,SDA);
	delay_nop();
	GPIO_SetBits(GPIOB,SCL);
	delay_nop();
	GPIO_ResetBits(GPIOB,SCL);
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
	GPIO_ResetBits(GPIOB,SCL);
	delay_nop();
	GPIO_SetBits(GPIOB,SDA);
	delay_nop();
	GPIO_SetBits(GPIOB,SCL);
	delay_nop();
	GPIO_ResetBits(GPIOB,SCL);
	delay_nop();
} 
/************************************************************   
* ������:I2C_ReadByte   
* ���� : ���ض��豸id���ض���ַ��ȡ����
* ����  :�豸id���ڲ���ַ   
* ���  :��ȡ������   
*/
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
   //delay2(10);
   delay2(50);
   return(i);

}
/************************************************************   
* ������:I2C_WriteByte   
* ���� : ���ض��豸id���ض���ַ��д���ֽ� 
* ����  :�豸id���ڲ���ַ    
* ���  :��    
*/
void I2C_WriteByte(unsigned char DeviceAddr,unsigned char address,unsigned char info)
{

   iic_start();
   iic_writex(DeviceAddr);
   iic_check_ACK();
   iic_writex(address);
   iic_check_ACK();
   iic_writex(info);
   iic_check_ACK();
   iic_stop();
   //delay2(50);
   delay2(250);

}
/************************************************************   
* ������:I2C_NoAddr_WriteByte   
* ���� : ���ض��豸id��д���ֽ� 
* ����  :�豸id������   
* ���  :��    
*/
void I2C_NoAddr_WriteByte(unsigned char DeviceAddr,unsigned char info)
{

   iic_start();
   iic_writex(DeviceAddr);
   iic_check_ACK();
   iic_writex(info);
   iic_check_ACK();
   iic_stop();
   //delay2(50);
   delay2(250);

}
/************************************************************   
* ������:I2C_Read_MultiBytes   
* ���� : ���ض��豸id���ض���ַ��ȡ����ֽ�
* ����  :�豸id���ڲ���ַ����Ҫ�����ֽڸ�����0����ȡ����OutDate������Ŷ�ȡ���ֽ�
* ���  :��ȡ������   
*/
void I2C_Read_MultiBytes(unsigned char DeviceAddr,unsigned char address,unsigned char BytesNum,unsigned char * OutDate )
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
	 for(i=0;i<BytesNum;i++)
	{
   OutDate[i]=iic_readx();
		if(i+1<BytesNum)  I2C_Ack();else	I2C_NoAck();//���һ���ֽ�����Ӧ��
	 }
   iic_stop();
   //delay2(10);
   delay2(250);
  

}
/************************************************************   
* ������:I2C_Read_2Bytes   
* ���� : ���ض��豸id���ض���ַ��ȡ����ֽ�
* ����  :�豸id���ڲ���ַ����Ҫ�����ֽڸ�����0����ȡ����OutDate������Ŷ�ȡ���ֽ�
* ���  :��ȡ������   
*/
uint16_t I2C_Read_2Bytes(unsigned char DeviceAddr,unsigned char address)
{
   unsigned char i,data_temp1,data_temp2;
	 uint16_t data16;
   iic_start();
   iic_writex(DeviceAddr);
   iic_check_ACK();
   iic_writex(address);
   iic_check_ACK();
   iic_start();
   iic_writex(DeviceAddr+1);
   iic_check_ACK();
	 
   data_temp1=iic_readx();
	 I2C_Ack();
   data_temp2=iic_readx();	
	 I2C_NoAck();//���һ���ֽ�����Ӧ��
	 
   iic_stop();
   //delay2(10);
   delay2(250);
	data16=(data_temp1<<8)|data_temp2;
	return data16;
  

}
/************************************************************   
* ������:I2C_Read_3Bytes   
* ���� : ���ض��豸id���ض���ַ��ȡ����ֽ�
* ����  :�豸id���ڲ���ַ
* ���  :��ȡ������   
*/
uint32_t I2C_Read_3Bytes(unsigned char DeviceAddr,unsigned char address)
{
   unsigned char i,data_temp1,data_temp2,data_temp3;
	 uint32_t data32;
   iic_start();
   iic_writex(DeviceAddr);
   iic_check_ACK();
   iic_writex(address);
   iic_check_ACK();
   iic_start();
   iic_writex(DeviceAddr+1);
   iic_check_ACK();
	 
   data_temp1=iic_readx();
	 I2C_Ack();
   data_temp2=iic_readx();	
	 I2C_Ack();
   data_temp3=iic_readx();
	 I2C_NoAck();//���һ���ֽ�����Ӧ��
	 
   iic_stop();
   //delay2(10);
   delay2(250);
	 data32=data_temp1*65535+data_temp2*256+data_temp3;
	 return data32;
  

}

/************************************************************   
* ������:IIC_GPIO_Configuration   
* ���� : ����PB6ΪSCL PB7ΪSDA  
* ����  :��   
* ���  :��    
*/
void I2C_GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 
  
  GPIO_InitStructure.GPIO_Pin = SCL;          //24C02 SCL
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA ��Ϊ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/************************************************************   
* ������:iic_SDA_Set_Dir   
* ���� : SDA��������������� 
* ����  :��   
* ���  :��    
*/
void iic_SDA_Set_Dir(unsigned char io_set) //SDA���������������
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    if(io_set==0)
  {
  GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA ��Ϊ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  }
 else if(io_set==1)
  {
  GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA ��Ϊ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  }
 else
  {;}
}


