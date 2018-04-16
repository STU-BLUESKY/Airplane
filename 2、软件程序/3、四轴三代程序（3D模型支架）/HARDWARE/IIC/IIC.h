#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f10x_conf.h"
#include "sys.h"
/*��������----------------------------------------------------------------*/

#define GPIO_Pin_SCL     GPIO_Pin_6 // SCL
#define GPIO_Pin_SDA     GPIO_Pin_7 // SDA 

//IO��������																																
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}			//��8������ȡCRL������4*7λ

//IO��������	 
#define IIC_SCL    PBout(6) 		//SCL				������ӷ�����
#define IIC_SDA    PBout(7) 		//SDA	 
#define SDA_READ   PBin(7) 		//����SDA 

void delay_nop(void);
void delay2(unsigned int x);
void iic_start(void);
void iic_stop(void);
void iic_writex(unsigned char j);
unsigned char iic_readx(void);
u8 iic_check_ACK(void);
void iic_SDA_Set_Dir(unsigned char io_set);

void I2C_GPIO_Configuration(void);
unsigned char I2C_ReadByte(unsigned char DeviceAddr,unsigned char address); //��24c02�ĵ�ַaddress�ж�ȡһ���ֽ�����
u8 I2C_WriteByte(unsigned char DeviceAddr,unsigned char address,unsigned char info);
void I2C_NoAddr_WriteByte(unsigned char DeviceAddr,unsigned char info);
u8 I2C_Read_Len(unsigned char DeviceAddr,unsigned char address,unsigned char len,unsigned char * buf);
u8 I2C_Write_Len(unsigned char DeviceAddr,unsigned char address,unsigned char len,unsigned char * buf);
u8 IIC_Read_Byte(unsigned char ack);


#endif

//------------------End of File----------------------------

