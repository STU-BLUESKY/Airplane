#include "IIC.h"


unsigned char I2C_ReadByte(unsigned char DeviceAddr,unsigned char address);  //从24c02的地址address中读取一个字节数据
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
* 函数名:delay2,delay_nop   
* 描述 :iic时序延时用
* 输入 :无  
* 输出 :无  
*/
void delay2(unsigned int x)
{
   unsigned int i;
   for(i=0;i<x;i++);
}

void delay_nop(void)
{
  unsigned int i=10; //i=10延时1.5us//这里可以优化速度 ，经测试最低到5还能写入
   while(i--);

}

/************************************************************   
* 函数名:iic_start   
* 描述 :开始时序
* 输入 :无  
* 输出 :无  
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
* 函数名:iic_stop   
* 描述 :停止时序
* 输入 ：无  
* 输出 :无  
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
* 函数名:iic_writex   
* 描述 : 写字节
* 输入 ：要写的字节   
* 输出 :无  
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
* 函数名:iic_readx   
* 描述 : 读取字节
* 输入  :无   
* 输出  :读取的字节    
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
* 函数名:iic_check_ACK   
* 描述 : 检测从机应答信号
* 输入  :无   
* 输出  :无    
*/
void iic_check_ACK(void)//检测从机应答信号
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
* 函数名:I2C_ReadByte   
* 描述 : 从特定设备id的特定地址读取内容
* 输入  :设备id，内部地址   
* 输出  :读取的内容   
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
* 函数名:I2C_WriteByte   
* 描述 : 向特定设备id的特定地址，写入字节 
* 输入  :设备id，内部地址    
* 输出  :无    
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
* 函数名:I2C_NoAddr_WriteByte   
* 描述 : 向特定设备id，写入字节 
* 输入  :设备id，内容   
* 输出  :无    
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
* 函数名:I2C_Read_MultiBytes   
* 描述 : 从特定设备id的特定地址读取多个字节
* 输入  :设备id，内部地址，需要读的字节个数（0不读取），OutDate用来存放读取的字节
* 输出  :读取的内容   
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
		if(i+1<BytesNum)  I2C_Ack();else	I2C_NoAck();//最后一个字节无需应答
	 }
   iic_stop();
   //delay2(10);
   delay2(250);
  

}
/************************************************************   
* 函数名:I2C_Read_2Bytes   
* 描述 : 从特定设备id的特定地址读取多个字节
* 输入  :设备id，内部地址，需要读的字节个数（0不读取），OutDate用来存放读取的字节
* 输出  :读取的内容   
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
	 I2C_NoAck();//最后一个字节无需应答
	 
   iic_stop();
   //delay2(10);
   delay2(250);
	data16=(data_temp1<<8)|data_temp2;
	return data16;
  

}
/************************************************************   
* 函数名:I2C_Read_3Bytes   
* 描述 : 从特定设备id的特定地址读取多个字节
* 输入  :设备id，内部地址
* 输出  :读取的内容   
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
	 I2C_NoAck();//最后一个字节无需应答
	 
   iic_stop();
   //delay2(10);
   delay2(250);
	 data32=data_temp1*65535+data_temp2*256+data_temp3;
	 return data32;
  

}

/************************************************************   
* 函数名:IIC_GPIO_Configuration   
* 描述 : 配置PB6为SCL PB7为SDA  
* 输入  :无   
* 输出  :无    
*/
void I2C_GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 
  
  GPIO_InitStructure.GPIO_Pin = SCL;          //24C02 SCL
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA 作为输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/************************************************************   
* 函数名:iic_SDA_Set_Dir   
* 描述 : SDA引脚输入输出设置 
* 输入  :无   
* 输出  :无    
*/
void iic_SDA_Set_Dir(unsigned char io_set) //SDA引脚输入输出设置
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    if(io_set==0)
  {
  GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA 作为输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  }
 else if(io_set==1)
  {
  GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA 作为输入
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  }
 else
  {;}
}


