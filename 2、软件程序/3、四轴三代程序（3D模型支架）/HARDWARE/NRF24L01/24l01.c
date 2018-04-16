#include "24l01.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x11,0x43,0x10,0x10,0x01};		//���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x11,0x43,0x10,0x10,0x01};
//��ʼ�� 24L01��IO��
// CSN -->PB1	IRQ -->PB0
// CE --> PA3	 SCK --> PA5		
// MISO --> PA6  MOSI -->PA7(SPI1)
void NRF24L01_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);   //ʹ��PB,G�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PB1���� ��ֹW25X�ĸ��� NSS
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	//��ʼ��ָ��IO		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);		

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);	 
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);							
	
	
	SPI1_Init();							//��ʼ��SPI
	SPI_Cmd(SPI1,DISABLE);		//SPI���費ʹ��
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//����SPI�����˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;				//����SPI����ģʽ: ����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;					//ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;				//���ݲ����ڵ�һ��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;			//���岨����Ԥ��Ƶ��ֵ��������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//MSB λ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1,&SPI_InitStructure);		//����ָ���Ĳ�����ʼ������SPIx
	
	SPI_Cmd(SPI1,ENABLE);		//ʹ��SPI����
	NRF24L01_CE=0;					//ʹ��24L01
	NRF24L01_CSN=1;					//SPIƬѡȡ��
	
}
//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);								//SPI�ٶ�Ϊ9MHZ
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR, buf, 5);		//д��5���ֽڵĵ�ַ
	NRF24L01_Read_Buf(TX_ADDR, buf, 5);			//����д��ĵ�ַ
	for( i=0; i<5; i++)
		if(buf[i]!=0xA5) break;
	if(i!=5) return 1;					//���24L01����
	return 0;										//��⵽24L01

}
//SPI д�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg, u8 value)
{
	u8 status;
	NRF24L01_CSN = 0;											//ʹ��SPI����
	status = SPI1_ReadWriteByte(reg);			//���ͼĴ�����
	SPI1_ReadWriteByte(value);						//���ͼĴ�����ֵ
	NRF24L01_CSN = 1;											//��ֹSPI����
	return(status);												//����״ֵ̬

}
//��ȡSPI�Ĵ�����ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;
	NRF24L01_CSN = 0;											//ʹ��SPI����
	SPI1_ReadWriteByte(reg);							//���ͼĴ�����
	reg_val=SPI1_ReadWriteByte(0xFF);			//��ȡ�Ĵ���������
	NRF24L01_CSN = 1;											//��ֹSPI����
	return(reg_val);											//����״ֵ̬
	
}
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ���˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Read_Buf(u8 reg,u8*pBuf,u8 len)									
{
	u8 status,u8_ctr;
	NRF24L01_CSN = 0;											//ʹ��SPI����
	status = SPI1_ReadWriteByte(reg);			//���ͼĴ���ֵ(λ��)������ȡ״ֵ̬
	for(u8_ctr=0; u8_ctr<len; u8_ctr++) pBuf[u8_ctr]=SPI1_ReadWriteByte(0xFF);		//��ȡSPI����
	NRF24L01_CSN = 1;											//�ر�SPI����
	return status;												//���ض�����״ֵ̬
}

//��ָ��Ϊֵд��ָ�����ȵ�����
//reg���Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ���˴�д���״̬�Ĵ�����ֵ
u8 NRF24L01_Write_Buf(u8 reg,u8* pBuf,u8 len)
{
	u8 status,u8_ctr;
	NRF24L01_CSN = 0;											//ʹ��SPI����
	status = SPI1_ReadWriteByte(reg);			//���ͼĴ���ֵ(λ��)������ȡ״ֵ̬
	for(u8_ctr=0; u8_ctr<len; u8_ctr++) SPI1_ReadWriteByte(*pBuf++);						//д�����ݵ�SPI
	NRF24L01_CSN = 1;											//�ر�SPI����
	return status;												//���ض�����״ֵ̬	
}

//����24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);									//SPI�ٶ�Ϊ9MHz  NRF2401_CE=0
	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);		//д���ݵ�TX BUF 32���ֽ�
	NRF24L01_CE=1;																	//��������
	while(NRF24L01_IRQ!=0);													//�ȴ��������
	sta = NRF24L01_Read_Reg(STATUS);								//��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta);		//���TX_DS ��MAX_RT �жϱ�־
	if(sta&MAX_TX)													//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0XFF);		//���TX FIFO�Ĵ���
		return MAX_TX;	
	}
	if(sta&TX_OK)					//�������
	{
		return TX_OK;
	}
	return 0xff;	//����ԭ����ʧ��

}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0,�������;�������������
u8 NRF24L01_RxPacket(u8* rxbuf)
{
	u8 sta;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);					//SPI�ٶ�9MHz
	sta = NRF24L01_Read_Reg(STATUS);								//��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta);		//���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)						//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);		//��ȡ����	 
		NRF24L01_Write_Reg(FLUSH_RX,0xff);											//���RX FIFO �Ĵ���				
		return 0;
	}
	return 1;				//û���յ��κ�����
	
}
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ��дRX���ݿ�ȣ�ѡ��RFƵ���������ʺ�LNA HCURR
//��CE��ߺ󣬼�����RXģʽ�������Խ���������
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE=0;
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);
	
//	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x00);										//ʧ���Զ�Ӧ��
	
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);										//ʹ��ͨ��0���Զ�Ӧ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);								//ʹ��ͨ��0�Ľ��յ�ַ
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);											//����RFͨ��Ƶ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);			//ѡ��ͨ��0����Ч���ݿ��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);								//����TX���������0db���棬2Mbps�����������濪��
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0f);									//���û�������ģʽ�Ĳ�����PWR_UP, EN_CRC, 16BIT_CRC,����ģʽ
	NRF24L01_CE=1;			//CEΪ�ߣ��������ģʽ
	
}
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ��дTX���ݿ�ȣ�����RX�Զ�Ӧ��ĵ�ַ
//���TX�������ݣ�ѡ��RFƵ���������ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ󣬼�����RXģʽ�������Խ���������
//CEΪ�ߴ���10us������������
void NRF24L01_TX_Mode(void)
{
	NRF24L01_CE = 0;
	NRF24L01_Write_Buf( NRF_WRITE_REG+TX_ADDR, (u8*)TX_ADDRESS, TX_ADR_WIDTH);
	NRF24L01_Write_Buf( NRF_WRITE_REG+RX_ADDR_P0, (u8*)RX_ADDRESS, RX_ADR_WIDTH);			//����TX�ڵ��ַ����ҪΪ��ʹ��ACK
	
//	NRF24L01_Write_Reg(	NRF_WRITE_REG+EN_AA, 0x00);						//ʧ���Զ�Ӧ��
//	NRF24L01_Write_Reg( NRF_WRITE_REG+EN_RXADDR, 0x00);				//ʧ�ܽ��ܵ�ַ
//	NRF24L01_Write_Reg( NRF_WRITE_REG+SETUP_RETR, 0x00);			//ʧ���Զ��ط�
	
	NRF24L01_Write_Reg(	NRF_WRITE_REG+EN_AA, 0x01);					//ʹ��ͨ��0���Զ�Ӧ��
	NRF24L01_Write_Reg( NRF_WRITE_REG+EN_RXADDR, 0x01);			//ʹ��ͨ��0�Ľ��յ�ַ	
	NRF24L01_Write_Reg( NRF_WRITE_REG+SETUP_RETR, 0x1a);		//�����Զ��ط����ʱ�䣺500us+86us�������Զ��ط���� 10��
	NRF24L01_Write_Reg( NRF_WRITE_REG+RF_CH, 40);						//����RFͨ��Ϊ40
	NRF24L01_Write_Reg( NRF_WRITE_REG+RF_SETUP, 0x0f);			//����TX���������0db���棬2Mbps�����������濪��
	NRF24L01_Write_Reg( NRF_WRITE_REG+CONFIG, 0x0e);				//���û�������ģʽ�Ĳ�����PWR_UP, EN_CRC, 16BIT_CRC,����ģʽ
	NRF24L01_CE = 1;     //CEΪ�ߣ�10us����������
	
}


