#include "adc.h"
#include "delay.h"
#include "mode.h"

//����

u8 L_Rocker=0,R_Rocker=0;
u8 L_Center=0, R_Center=0;
u16 LX_adc, LY_adc, RX_adc, RY_adc, POWER_adc;


//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	ADC_DeInit(ADC1);  //��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;												//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;															//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);																	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);									//ʹ�ܸ�λУ׼   
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����	
	ADC_StartCalibration(ADC1);								 	//����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));		 	//�ȴ�У׼����

}				

//���ADCֵ
//ch:ͨ��ֵ 0~3
//way����ͨ�� 0~13
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			     
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������		 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

//ȡƽ�����ģ���ѹ��
u16 Get_Adc_Average(u8 ch, u8 times)
{
	u32 temp_val=0;
	u8 t;
	for( t=0; t<times; t++)
	{
		temp_val += Get_Adc(ch);
		delay_ms(5);			
	}
	
	return temp_val/times;
} 	


void Rocker_scan(void)
{
	
	LX_adc=get_LX_adc();		//PA3				 
	LY_adc=get_LY_adc();		//PB0
	RX_adc=get_RX_adc();		//PA1
	RY_adc=get_RY_adc();		//PA2
	
	if(LX_adc>1000&&LX_adc<3000&&LY_adc>3000) L_Rocker= ROCKER_UP;	
	else if(LX_adc>1000&&LX_adc<3000&&LY_adc<1000) L_Rocker= ROCKER_DOWN;	
	else if(LX_adc<1000&&LY_adc>1000&&LY_adc<3000) L_Rocker= ROCKER_RIGHT;
	else if(LX_adc>3000&&LY_adc>1000&&LY_adc<3000)	L_Rocker= ROCKER_LEFT;
	else { L_Rocker=ROCKER_CENTER; L_Center=1;}
	
	if(RX_adc>1000&&RX_adc<3000&&RY_adc>3000) R_Rocker= ROCKER_UP;	
	else if(RX_adc>1000&&RX_adc<3000&&RY_adc<1000) R_Rocker= ROCKER_DOWN;	
	else if(RX_adc<1000&&RY_adc>1000&&RY_adc<3000) R_Rocker= ROCKER_RIGHT;
	else if(RX_adc>3000&&RY_adc>1000&&RY_adc<3000)	R_Rocker= ROCKER_LEFT;
	else { R_Rocker=ROCKER_CENTER; R_Center=1;}

}





