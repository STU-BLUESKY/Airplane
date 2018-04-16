#include "IIC.h"
#include "delay.h"

/************************************************************   
* 函数名:IIC_GPIO_Configuration   
* 描述 : 配置PB6为SCL PB7为SDA  
* 输入  :无   
* 输出  :无    
*/
void I2C_GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//先使能外设IO PORTB时钟 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SCL|GPIO_Pin_SDA;	 // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIO 
  GPIO_SetBits(GPIOB,GPIO_Pin_SCL|GPIO_Pin_SDA);						 //PB10,PB11 输出高	
 
}

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
* 函数名:iic_stop   
* 描述 :停止时序
* 输入 ：无  
* 输出 :无  
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
* 函数名:iic_writex   
* 描述 : 写字节
* 输入 ：要写的字节   
* 输出 :无  
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
* 函数名:iic_readx   
* 描述 : 读取字节
* 输入  :无   
* 输出  :读取的字节    
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
* 函数名:iic_check_ACK   
* 描述 : 检测从机应答信号
* 输入  :无   
* 输出  :无    
*************************************************************/
u8 iic_check_ACK(void)//检测从机应答信号
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
* 函数名:I2C_ReadByte   
* 描述 : 从特定设备id的特定地址读取内容
* 输入  :设备id，内部地址   
* 输出  :读取的内容   
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
* 函数名:I2C_WriteByte   
* 描述 : 向特定设备id的特定地址，写入字节 
* 输入  :设备id，内部地址    
* 输出  :无    
************************************************************/
u8 I2C_WriteByte(unsigned char DeviceAddr,unsigned char address,unsigned char info)
{

	iic_start();
	iic_writex(DeviceAddr);
	if(iic_check_ACK())		//等待ACK
	{
		iic_stop();	 
		return 1;		 
	}
	iic_writex(address);
	iic_check_ACK();
	iic_writex(info);
	if(iic_check_ACK())		//等待ACK
	{
		iic_stop();	 
		return 1;		 
	}
	iic_stop();
	//delay2(50);
	delay2(250);
	return 0;
}


//特殊添加
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
* 函数名:I2C_NoAddr_WriteByte   
* 描述 : 向特定设备id，写入字节 
* 输入  :设备id，内容   
* 输出  :无    
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
* 函数名:iic_SDA_Set_Dir   
* 描述 : SDA引脚输入输出设置 
* 输入  :无   
* 输出  :无    
************************************************************/
void iic_SDA_Set_Dir(unsigned char io_set)
{
	if(io_set==0)		//输出配置
  {		
		SDA_OUT();		
  }
	else if(io_set==1)	//输入配置
  {
		SDA_IN();
  }

}

/************************************************************   
* 函数名:I2C_Read_Len   
* IIC连续读
* DeviceAddr:器件地址
* address:要读取的寄存器地址
* len:要读取的长度(0不读取)
* buf:读取到的数据存储区
*************************************************************/
//读取mpu6050以外的数据有问题
u8 I2C_Read_Len(unsigned char DeviceAddr,unsigned char address,unsigned char len,unsigned char * buf )
{
   unsigned char i;
   iic_start();
   iic_writex(DeviceAddr);
   if(iic_check_ACK())		//等待ACK
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
		else	I2C_NoAck();//最后一个字节无需应答
	 }
   iic_stop();
   //delay2(10);
   delay2(250);
	 return 0;	
}
/************************************************************   
* 函数名:I2C_Write_Len   
* IIC连续写
* DeviceAddr:器件地址 
* address:寄存器地址
* len:写入长度
* buf:数据区
* 返回值:0,正常
* 其他,错误代码
*************************************************************/
u8 I2C_Write_Len(unsigned char DeviceAddr,unsigned char address,unsigned char len,unsigned char *buf)
{
	u8 i; 
  iic_start(); 
	iic_writex(DeviceAddr);	
	if(iic_check_ACK())		//等待ACK
	{
			iic_stop();	 
			return 1;		 
	}
  iic_writex(address);
  iic_check_ACK();		//等待应答
	for(i=0;i<len;i++)
	{
		iic_writex(buf[i]);	//发送数据
		if(iic_check_ACK())		//等待ACK
		{
			iic_stop();	 
			return 1;		 
		}
	}    
  iic_stop(); 
	return 0;
} 

//为MS5611写
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

