#include "Altitude.h"
#include "MS5611.h"
#include "imu.h"
#include "mpu6050.h"
#include "usart.h"

nav_t nav;	//NED frame in earth
float z_est[3]={ 0.0f, 0.0f, 0.0f };	// estimate z Vz  Az	z�� λ�� �ٶ� ���ٶ�
static float w_z_baro=0.7f;				//��ѹ��У��ϵ��
static float w_acc_bias=0.05f;		
float corr_acc = 0.0;
float z_NED_offset = 0.0;

uint32_t t_pre=0;
float dt;
float accel_NED[3] = { 0.0f, 0.0f, -CONSTANTS_ONE_G };	//���������µļ��ٶ�ƫ���� �������ٶȲ���
float acc_bias[] = { 0.0f, 0.0f, 0.0f };	// ���������µļ��ٶ�ƫ����
float corr_baro = 0.0f;					//��ѹ��У����

int altitude;
int altiOffset;


//Combine Filter to correct err
static void inertial_filter_predict(float dt, float x[3], float acc)		//���ٶȻ���Ԥ���ٶȺ�λ��
{
	  x[2] = acc;
		x[1] += x[2] * dt;		//�ٶȹ�ʽ
    x[0] += x[1] * dt + x[2] * dt * dt / 2.0f;		//λ�ƹ�ʽ	

}

static void inertial_filter_correct(float e, float dt, float x[3], int i, float w)		//λ��������
{
    float ewdt = e * w * dt;		//ƫ���� * У��ϵ�� * ʱ��
    x[i] += ewdt;		//����У����

    if (i == 0) {
        x[1] += w * ewdt;
    }  
}


void AltitudeCombineThread(void)
{   
		
		uint32_t t;
		uint8_t i,j;

    float accel_bias_corr[3] = { 0.0f, 0.0f, 0.0f };	//���ٶ�ƫ������У�� 
		
		t=micros();
		dt = (t_pre>0)?((t-t_pre)/1000000.0f):0;	//(s)
		t_pre=t;
							
			sensor.acc.cal.x -= acc_bias[0];		//���ٶ����ݼ�ȥƫ����
			sensor.acc.cal.y -= acc_bias[1];
			sensor.acc.cal.z -= acc_bias[2];

			//ת������ϵ
			//�����������µļ��ٶ�ת��������������
			accel_NED[0]=DCMgb[0][0]* sensor.acc.cal.x + DCMgb[1][0]*sensor.acc.cal.y + DCMgb[2][0]*sensor.acc.cal.z;	//�����������µļ��ٶ�ת��������������
			accel_NED[1]=DCMgb[0][1]* sensor.acc.cal.x + DCMgb[1][1]*sensor.acc.cal.y + DCMgb[2][1]*sensor.acc.cal.z;
			accel_NED[2]=DCMgb[0][2]* sensor.acc.cal.x + DCMgb[1][2]*sensor.acc.cal.y + DCMgb[2][2]*sensor.acc.cal.z;		
		
			accel_NED[2] -= CONSTANTS_ONE_G;
			//������ѹ�ƺͼ��ٶȻ��ֵ����
			corr_baro = MS5611_Altitude - z_est[0];	
        				
			//���ٶ�ƫ����У��
			accel_bias_corr[2] -= corr_baro * w_z_baro * w_z_baro;		
			
			//��ƫ����ת������������
			for (i = 0; i < 3; i++)
			{
				float c = 0.0f;
				for (j = 0; j < 3; j++) c += DCMgb[i][j] * accel_bias_corr[j];	
				acc_bias[i] = c * w_acc_bias * dt;				//ƫ��������ƫ��ϵ��		
			}
					
			inertial_filter_predict(dt, z_est, accel_NED[2]);
			inertial_filter_correct(corr_baro, dt, z_est, 0, w_z_baro);		//0.5f
			
			nav.z=z_est[0]*100;
			nav.vz=z_est[1]*100;
			nav.az=z_est[2]*100;
			
}

u8 isRCWLInit = 0;
void RCWL0801_Get_Altitude(void)
{
	
	if(READING_ALTI==0)
	{	
		READING_ALTI = 1;
		USART_SendData(USART3, 0XA0);
//		USART_RX_BUF[0]=0;
//		USART_RX_BUF[1]=0;
	}
	
}

void RCWL0801_Init(void)
{
	//RCWL-0801-V1.0
	READING_ALTI=0;
	delay_ms(500);
	USART_SendData(USART3, 0XB0);
	delay_ms(200);
	USART_SendData(USART3, 0XC1);
	delay_ms(200);
}


void recordHeight(void)	//Not used
{
	static u8 cnt=0;
	static uint16_t height;
	height += altitude;
	cnt++;
	if(cnt==10)
	{
		height = height/10;
		altiOffset = height;
		isRCWLInit = 1;
	}
	
}

