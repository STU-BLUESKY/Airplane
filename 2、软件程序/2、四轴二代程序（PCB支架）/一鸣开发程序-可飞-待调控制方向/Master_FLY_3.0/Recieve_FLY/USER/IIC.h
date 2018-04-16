#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f10x_conf.h"
#include "sys.h"
/*函数声明----------------------------------------------------------------*/

#define SCL     GPIO_Pin_6 //24C02 SCL
#define SDA     GPIO_Pin_7 //24C02 SDA 

void delay_nop(void);
void delay2(unsigned int x);
void iic_start(void);
void iic_stop(void);
void iic_writex(unsigned char j);
unsigned char iic_readx(void);
void iic_check_ACK(void);
void iic_SDA_Set_Dir(unsigned char io_set);

void I2C_GPIO_Configuration(void);
unsigned char I2C_ReadByte(unsigned char DeviceAddr,unsigned char address);  //从24c02的地址address中读取一个字节数据
void I2C_WriteByte(unsigned char DeviceAddr,unsigned char address,unsigned char info);
void I2C_NoAddr_WriteByte(unsigned char DeviceAddr,unsigned char info);
void I2C_Read_MultiBytes(unsigned char DeviceAddr,unsigned char address,unsigned char BytesNum,unsigned char * OutDate );
uint16_t I2C_Read_2Bytes(unsigned char DeviceAddr,unsigned char address);
uint32_t I2C_Read_3Bytes(unsigned char DeviceAddr,unsigned char address);
#endif

//------------------End of File----------------------------

