#include "mode.h"
#include "oled.h"
#include "24l01.h"
#include "delay.h"
#include "led.h"
#include "string.h"
#include "adc.h"
#include "key.h"
#include "param.h"
#include "usart.h"

u8 CurMode =1;
u8 NextMode=1;



//********************************************
// 函数功能: 模式选择
//********************************************
void chooseMode(void)
{
	switch(CurMode)
	{
		case 2: mode_2();		break;		//模式控制
		case 6: mode_6();  	break;		//电量反馈
		case 8: mode_8();		break;  	//发送调试		
		case 9: mode_9();		break;  	//接收调试
		case 11: mode_11(); break;		//姿态角接收
		case 13: mode_13(); break;		//参数调试
		default:break;
	}

}
//*********************** 	不同模式类型  ******************************
void mode_2(void)	
{
	static u8 send_buf[33]={"A0MM"};
	static float pitch=0.0, roll=0.0, yaw=0.0;
	if(key==KEY5_PRES){LED1=!LED1; send_buf[2]='M';}
	else if(key==KEY6_PRES){LED2=!LED2;	send_buf[2] = 'A';}
	else if(key==KEY1_PRES){LED1=!LED1;	send_buf[0] = 'D';}
	else if(key==KEY4_PRES){LED2=!LED2; send_buf[0] = 'A';}
	
	OLED_ShowChar(112,0,send_buf[2],16);
	
	//读取AD读数
	LX_adc=get_LX_adc();		//PA1				 
	LY_adc=get_LY_adc();		//PA5
	RX_adc=get_RX_adc();		//PA2	
	RY_adc=get_RY_adc();		//PA3
	
	if(LX_adc>1000&&LX_adc<3000&&LY_adc>2500) L_Rocker= ROCKER_UP;	
	else if(LX_adc>1000&&LX_adc<3000&&LY_adc<1500) L_Rocker= ROCKER_DOWN;	
	else if(LX_adc<1500&&LY_adc>1000&&LY_adc<3000) L_Rocker= ROCKER_RIGHT;
	else if(LX_adc>2500&&LY_adc>1000&&LY_adc<3000)	L_Rocker= ROCKER_LEFT;
	else { L_Rocker=ROCKER_CENTER; L_Center=1;}
	
	if(RX_adc>1000&&RX_adc<3000&&RY_adc>3000) R_Rocker= ROCKER_UP;	
	else if(RX_adc>1000&&RX_adc<3000&&RY_adc<1000) R_Rocker= ROCKER_DOWN;	
	else if(RX_adc<1000&&RY_adc>1000&&RY_adc<3000) R_Rocker= ROCKER_RIGHT;
	else if(RX_adc>3000&&RY_adc>1000&&RY_adc<3000)	R_Rocker= ROCKER_LEFT;
	else { R_Rocker=ROCKER_CENTER; R_Center=1;}
	
	if(L_Rocker==ROCKER_UP ){ Throttle=600+(LY_adc-2500.0)/1500.0*200.0;}
	else if(L_Rocker==ROCKER_DOWN ) {	Throttle=600-(1500.0-LY_adc)/1500.0*100.0;}
	else Throttle = 600;
	if(L_Rocker==ROCKER_RIGHT ){ yaw=-20.0;}
	else if(L_Rocker==ROCKER_LEFT ){ yaw=20.0;}
	else yaw = 0.0;
	
//	if(R_Rocker==ROCKER_UP ){ pitch=(RY_adc-2500.0)/1500.0*5.0;}
//	else if(R_Rocker==ROCKER_DOWN ){ pitch= -(1500.0-RY_adc)/1500.0*5.0;}
//	else pitch = 0.0;
//	if(R_Rocker==ROCKER_RIGHT ){ roll=(1500.0-RX_adc)/1500.0*5.0;}
//	else if(R_Rocker==ROCKER_LEFT ){ roll = -(RX_adc-2500.0)/1500.0*5.0;}
//	else roll = 0.0;
	
	if(R_Rocker==ROCKER_UP ) pitch=10.0;
	else if(R_Rocker==ROCKER_DOWN)  pitch=-10.0;
	else pitch = 0.0;
	if(R_Rocker==ROCKER_RIGHT ) roll=10.0;
	else if(R_Rocker==ROCKER_LEFT ) roll =-10.0;
	else roll = 0.0;
	
	if(Throttle<0 ) Throttle=0;
	memcpy(&send_buf[4], &Throttle, sizeof(Throttle));	
	memcpy(&send_buf[8], &pitch, sizeof(pitch));	
	memcpy(&send_buf[12],&roll, sizeof(roll));	
	memcpy(&send_buf[16],&yaw, sizeof(yaw));	
	NRF24L01_TxPacket(send_buf);
	
	
}


//电量反馈
void mode_6(void)
{
	float power;
	int int_part,dec_part;
	u8 temp=24,i=0,percent,T;
	
	POWER_adc=get_POWER_adc();	//PA2	
	power = POWER_adc/4096.0*3*3.3;				//电池电压值
	percent = (power-2.7)/(4.7-2.7)*100;  		//4.7为最大值  2.7V为0点
	if(percent>100) percent=100;				//限值
	T=(power-2.7)/(4.7-2.7)*BATTERY_MAX;		//4.7为最大值  2.7V为0点
	if(T>BATTERY_MAX) T=BATTERY_MAX;			//限值
	int_part=power;					//电压正数部分
	dec_part=(power-int_part)*100;	//电压小数部分
	
	OLED_DrawBMP(temp,2,80,1,bmp_battery_top);	//电池样式
	OLED_DrawBMP(temp,4,80,1,bmp_battery_bottom);
	OLED_DrawBMP(temp+74,3,4,1,bmp_battery_head);
	OLED_DrawBMP(temp+1,3,1,1,bmp_battery_line);
	OLED_DrawBMP(temp+2,3,1,1,bmp_battery_line);
	for(i=0;i<T;i++) OLED_DrawBMP(temp+6+i,3,1,1,bmp_battery_line);
	for(i=T;i<BATTERY_MAX;i++) OLED_DrawBMP(temp+6+i,3,1,1,bmp_battery_no);	
	OLED_ShowCHinese(0,6,32);			//电池电压
	OLED_ShowCHinese(16,6,33);
	OLED_ShowCHinese(32,6,34);
	OLED_ShowCHinese(48,6,35);
	OLED_ShowChar(64,6,':',16);	
	
	OLED_ShowNum(80,6,int_part,1,16);	//电池电压
	OLED_ShowChar(88,6,'.',16);
	OLED_ShowNum(96,6,dec_part,2,16);	
	
	OLED_ShowNum(72,0,percent,3,16);	//电量百分比
	OLED_ShowChar(96,0,'%',16);
	
}
/*--------------------------------
	函数功能：模式8，发送调试
	函数说明：发送数据
---------------------------------*/
void mode_8(void)
{
	u8 result;
	u8 tmp_buf[DATA_LEN+1] ={"STOP"};	//发送的消息
	tmp_buf[DATA_LEN]=0;		//加入结束符																				
	if(key==KEY1_PRES)	
	{
		LED1=!LED1;
		LED2=!LED2;
		result = NRF24L01_TxPacket(tmp_buf);
		if(result == TX_OK)		//发送成功 显示发送数据
		{			
			OLED_ShowString(0,6,"Success!        ",16);		
			OLED_ShowString(0,2,tmp_buf,16);	
		}
		else if(result == MAX_TX)	OLED_ShowString(0,6,"MAX_TX!         ",16);	//达到最大重发次数
		else OLED_ShowString(0,6,"Send Failed!    ",16);		//发送失败	
	}

}
/*--------------------------------
	函数功能：模式9，接收调试
	函数说明：显示接收到的所有数据v
---------------------------------*/
void mode_9(void)
{
		u8 tmp_buf[DATA_LEN+1];						
		if( NRF24L01_RxPacket(tmp_buf)==0 )		//一旦接收消息就显示出来
		{
			tmp_buf[DATA_LEN] = 0;										//加入字符串结束符
			OLED_ShowString(80,0,"OK!",16);	
			OLED_ShowString(0,2,tmp_buf,16);	
			
		}
}

/*--------------------------------
	函数功能：模式3，姿态角接收
	函数说明：显示姿态角
	发送协议：u8 buf_tmp[33]={"MPU"};
	memcpy(&buf_tmp[3],&sendPitch, sizeof(sendPitch));
	memcpy(&buf_tmp[7],&sendRoll,  sizeof(sendRoll));	
	memcpy(&buf_tmp[11],&sendYaw,  sizeof(sendYaw));	
	NRF24L01_TxPacket(buf_tmp);		
---------------------------------*/
void mode_11(void)			
{

	u8 tmp_buf[DATA_LEN+1];		
	int int_part,dec_part;
	static float DATA_pitch, DATA_roll, DATA_yaw;
	
	if(NRF24L01_RxPacket(tmp_buf)==0)
	{
		//MPU为标志位
		if(tmp_buf[0]== 'M'&&tmp_buf[1]== 'P'&&tmp_buf[2]== 'U')		//一个float 为4个u8					
		{
			memcpy(&DATA_pitch,&tmp_buf[3], sizeof(DATA_pitch));	
			memcpy(&DATA_roll,&tmp_buf[7], sizeof(DATA_roll));
			memcpy(&DATA_yaw,&tmp_buf[11], sizeof(DATA_yaw));
			
			printf("%f,%f,%f\n",DATA_pitch,DATA_roll,DATA_yaw);
			
			//显示数据，最大显示3位数的数据
			int_part = DATA_pitch	;															//整数部分	
			dec_part = (DATA_pitch-int_part)*1000;							//小数部分			
			int_part = abs(int_part);														//取绝对值
			dec_part = abs(dec_part);
			if(DATA_pitch<0 ) OLED_ShowChar(48,2,'-',16); 	
			else OLED_ShowChar(48,2,' ' ,16);			//显示+-号
			OLED_ShowNum(56,2,int_part,3,16); 		//显示整数
			OLED_ShowChar(80,2,'.',16);						//显示小数点
			OLED_ShowNum(88,2,dec_part,3,16);			//显示小数
			
			int_part = DATA_roll	;
			dec_part = (DATA_roll-int_part)*1000;
			int_part = abs(int_part);														
			dec_part = abs(dec_part);
			if(DATA_roll<0 ) OLED_ShowChar(48,4,'-',16); 
			else OLED_ShowChar(48,4,' ' ,16);
			OLED_ShowNum(56,4,int_part,3,16); 
			OLED_ShowChar(80,4,'.',16);	
			OLED_ShowNum(88,4,dec_part,3,16);
						
			
			int_part = DATA_yaw	;
			dec_part = (DATA_yaw-int_part)*1000;
			int_part = abs(int_part);														
			dec_part = abs(dec_part);
			if(DATA_yaw<0 ) OLED_ShowChar(48,6,'-',16); 
			else OLED_ShowChar(48,6,' ' ,16);
			OLED_ShowNum(56,6,int_part,3,16); 
			OLED_ShowChar(80,6,'.',16);
			OLED_ShowNum(88,6,dec_part,3,16);
			
			
		}
	}
	
}
//四轴参数调试
void mode_13(void)
{
	static u8 buf_pid[33]={"D0"};	
	
	if(key==KEY5_PRES)	
	{
		buf_pid[1]='0'; 
		LED1=!LED1;
		NRF24L01_TxPacket(buf_pid); 
	}
	else if(key==KEY6_PRES)	
	{
		LED2=!LED2;	
		
		if(NextMode>=1&&NextMode<=3)
		{
			buf_pid[1]='3';
			memcpy(&buf_pid[2],&Throttle, sizeof(Throttle));	
			memcpy(&buf_pid[6],&pitch_core_desPoint, sizeof(pitch_core_desPoint));	
			memcpy(&buf_pid[10],&roll_core_desPoint, sizeof(roll_core_desPoint));	
			NRF24L01_TxPacket(buf_pid);	
		}
		else if(NextMode>=4&&NextMode<=9)
		{
			buf_pid[1]='1'; 
			memcpy(&buf_pid[2],&ctrl.pitch.core.kp,  sizeof(ctrl.pitch.core.kp));	
			memcpy(&buf_pid[6],&ctrl.pitch.core.ki,  sizeof(ctrl.pitch.core.ki));	
			memcpy(&buf_pid[10],&ctrl.pitch.core.kd, sizeof(ctrl.pitch.core.kd));
			memcpy(&buf_pid[14],&ctrl.roll.core.kp, sizeof(ctrl.roll.core.kp));
			memcpy(&buf_pid[18],&ctrl.roll.core.ki, sizeof(ctrl.roll.core.ki));	
			memcpy(&buf_pid[22],&ctrl.roll.core.kd, sizeof(ctrl.roll.core.kd));	
			buf_pid[26]='E'; 
			NRF24L01_TxPacket(buf_pid);		
		}	
		else if(NextMode>=10&&NextMode<=15)
		{
			buf_pid[1]='2'; 
			memcpy(&buf_pid[2],&ctrl.pitch.shell.kp,  sizeof(ctrl.pitch.shell.kp));	
			memcpy(&buf_pid[6],&ctrl.pitch.shell.ki,  sizeof(ctrl.pitch.shell.ki));	
			memcpy(&buf_pid[10],&ctrl.pitch.shell.kd, sizeof(ctrl.pitch.shell.kd));
			memcpy(&buf_pid[14],&ctrl.roll.shell.kp, sizeof(ctrl.roll.shell.kp));
			memcpy(&buf_pid[18],&ctrl.roll.shell.ki, sizeof(ctrl.roll.shell.ki));	
			memcpy(&buf_pid[22],&ctrl.roll.shell.kd, sizeof(ctrl.roll.shell.kd));	
			buf_pid[26]='E'; 
			NRF24L01_TxPacket(buf_pid);		
		}
		else if(NextMode>=16&&NextMode<=21)
		{
			buf_pid[1]='4'; 
			memcpy(&buf_pid[2],&ctrl.yaw.core.kp,  sizeof(ctrl.yaw.core.kp));	
			memcpy(&buf_pid[6],&ctrl.yaw.core.ki,  sizeof(ctrl.yaw.core.ki));	
			memcpy(&buf_pid[10],&ctrl.yaw.core.kd, sizeof(ctrl.yaw.core.kd));
			memcpy(&buf_pid[14],&ctrl.yaw.shell.kp,  sizeof(ctrl.yaw.shell.kp));	
			memcpy(&buf_pid[18],&ctrl.yaw.shell.ki,  sizeof(ctrl.yaw.shell.ki));	
			memcpy(&buf_pid[22],&ctrl.yaw.shell.kd, sizeof(ctrl.yaw.shell.kd));
			buf_pid[26]='E'; 
			NRF24L01_TxPacket(buf_pid);		
		}
		
	}	
	
	Rocker_scan();
	if(L_Rocker==ROCKER_UP && L_Center==1){	PID_inc(NextMode); L_Center=0;}		//PID加1
	else if(L_Rocker==ROCKER_DOWN && L_Center==1){	PID_dec(NextMode); L_Center=0;}		//PID减1
	else if(L_Rocker==ROCKER_LEFT && L_Center==1){	PID_dec_point(NextMode); L_Center=0;}		//PID减0.1
	else if(L_Rocker==ROCKER_RIGHT && L_Center==1){	PID_inc_point(NextMode); L_Center=0;}		//PID加0.1
	
}


//********************************************
// 函数功能: 模式转换
//********************************************
void changeMode(void)
{
	switch(CurMode)
	{
		case 1:
			if(NextMode==1) changeMode_2();
			break;
		case 7:
			if(NextMode==1)  changeMode_8();	//转到发送调试
			if(NextMode==2)  changeMode_9();	//转到接收调试
			if(NextMode==3)  changeMode_11();	//转到姿态角接收
			break;
		case 3:
			if(NextMode==1) changeMode_13();
			break;
		default: break;		
	}		
		
}

void changeMode_2(void)
{
	changeModeTx();
}

void changeMode_8(void)
{	
	changeModeTx();
}
void changeMode_9(void)
{
	changeModeRx();
}
void changeMode_11(void)
{
	changeModeRx();
}

void changeMode_13(void)
{
	changeModeTx();
}








