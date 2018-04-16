#include "display.h"
#include "mode.h"
#include "param.h"

//********************************************
// ��������: ��ʾ����
//********************************************
void diaplay(void)
{
	switch(CurMode)
	{
		case 1: display_01(); break;	//��ʼ����
		case 2: display_02(); break;	//����ģʽ����
		case 3: display_03(); break;	//����ģʽ����
		case 4: display_04(); break;	//����ģʽ����
		case 5: display_05(); break;	//ҡ�˷���
		case 6: display_06(); break;	//�������� 
		case 7: display_07(); break;	//2.4G����
		case 8: display_08(); break;	//���͵���
		case 9: display_09(); break;	//���յ���
		case 10: display_10(); break; //��������
		case 11: display_11(); break;	//��������
		case 12: display_12(); break;	//��ʾģʽ����
		case 13: display_13(); break; //��������
		default:break;
	}
}
	
void display_01(void)						//��ʼ����
{
	//����ģʽ
	OLED_ShowCHinese(48,0,2);
	OLED_ShowCHinese(64,0,3);
	OLED_ShowCHinese(80,0,0);
	OLED_ShowCHinese(96,0,1);
	//����ģʽ
	OLED_ShowCHinese(48,2,4);
	OLED_ShowCHinese(64,2,5);
	OLED_ShowCHinese(80,2,0);
	OLED_ShowCHinese(96,2,1);
	//����ģʽ
	OLED_ShowCHinese(48,4,6);
	OLED_ShowCHinese(64,4,7);
	OLED_ShowCHinese(80,4,0);
	OLED_ShowCHinese(96,4,1);
	//��ʾģʽ
	OLED_ShowCHinese(48,6,36);
	OLED_ShowCHinese(64,6,37);
	OLED_ShowCHinese(80,6,0);
	OLED_ShowCHinese(96,6,1);
	switch(NextMode)
	{
		case 1:OLED_DrawBMP(16,0,24,2,bmp_arow); break;
		case 2:OLED_DrawBMP(16,2,24,2,bmp_arow); break;
		case 3:OLED_DrawBMP(16,4,24,2,bmp_arow); break;
		case 4:OLED_DrawBMP(16,6,24,2,bmp_arow); break;
		default: break;
	}
}


void display_02(void)						//����ģʽ
{
	OLED_ShowCHinese(0,0,0);
	OLED_ShowCHinese(16,0,1);
	OLED_ShowCHinese(32,0,2);
	OLED_ShowCHinese(48,0,3);
	OLED_DrawBMP(24,4,16,2,bmp_rocker_circle);
	OLED_DrawBMP(80,4,16,2,bmp_rocker_circle);
	OLED_ShowNum(72,0,Throttle,4,16);		//��ʾ����
	
	if(L_Rocker==ROCKER_UP)	OLED_DrawBMP(24,2,16,2,bmp_arow_up);		//��
	else OLED_DrawBMP(24,2,16,2,bmp_arow_blank);
	if(L_Rocker==ROCKER_RIGHT) OLED_DrawBMP(40,4,16,2,bmp_arow_right);	//��
	else OLED_DrawBMP(40,4,16,2,bmp_arow_blank);
	if(L_Rocker==ROCKER_DOWN) OLED_DrawBMP(24,6,16,2,bmp_arow_down);	//��
	else OLED_DrawBMP(24,6,16,2,bmp_arow_blank);
	if(L_Rocker==ROCKER_LEFT) OLED_DrawBMP(8,4,16,2,bmp_arow_left);		//��
	else OLED_DrawBMP(8,4,16,2,bmp_arow_blank);	
	
	if(R_Rocker==ROCKER_UP)	OLED_DrawBMP(80,2,16,2,bmp_arow_up);
	else OLED_DrawBMP(80,2,16,2,bmp_arow_blank);
	if(R_Rocker==ROCKER_RIGHT) OLED_DrawBMP(96,4,16,2,bmp_arow_right);
	else OLED_DrawBMP(96,4,16,2,bmp_arow_blank);
	if(R_Rocker==ROCKER_DOWN) OLED_DrawBMP(80,6,16,2,bmp_arow_down);
	else OLED_DrawBMP(80,6,16,2,bmp_arow_blank);
	if(R_Rocker==ROCKER_LEFT) OLED_DrawBMP(64,4,16,2,bmp_arow_left);
	else OLED_DrawBMP(64,4,16,2,bmp_arow_blank);	
}


void display_03(void)					  //����ģʽ
{
	OLED_ShowCHinese(48,0,8);
	OLED_ShowCHinese(64,0,9);
	OLED_ShowCHinese(80,0,4);
	OLED_ShowCHinese(96,0,5);
	
	OLED_ShowCHinese(48,2,10);
	OLED_ShowCHinese(64,2,11);
	OLED_ShowCHinese(80,2,4);
	OLED_ShowCHinese(96,2,5);
	
	OLED_ShowCHinese(48,4,12);
	OLED_ShowCHinese(64,4,13);
	OLED_ShowCHinese(80,4,4);
	OLED_ShowCHinese(96,4,5);
	
	OLED_ShowCHinese(80,6,14);
	OLED_ShowCHinese(96,6,15);
	
	switch(NextMode)
	{
		case 1:OLED_DrawBMP(16,0,24,2,bmp_arow); break;
		case 2:OLED_DrawBMP(16,2,24,2,bmp_arow); break;
		case 3:OLED_DrawBMP(16,4,24,2,bmp_arow); break;
		case 4:OLED_DrawBMP(16,6,24,2,bmp_arow); break;
		default: break;
	}
	
}


void display_04(void)					  //����ģʽ
{
	OLED_ShowCHinese(48,0,16);
	OLED_ShowCHinese(64,0,17);
	OLED_ShowCHinese(80,0,6);
	OLED_ShowCHinese(96,0,7);
	
	OLED_ShowCHinese(48,2,18);
	OLED_ShowCHinese(64,2,19);
	OLED_ShowCHinese(80,2,6);
	OLED_ShowCHinese(96,2,7);
	
	OLED_ShowCHinese(48,4,20);
	OLED_ShowCHinese(64,4,21);
	OLED_ShowCHinese(80,4,6);
	OLED_ShowCHinese(96,4,7);
	
	OLED_ShowCHinese(80,6,14);
	OLED_ShowCHinese(96,6,15);
	
	switch(NextMode)
	{
		case 1:OLED_DrawBMP(16,0,24,2,bmp_arow); break;
		case 2:OLED_DrawBMP(16,2,24,2,bmp_arow); break;
		case 3:OLED_DrawBMP(16,4,24,2,bmp_arow); break;
		case 4:OLED_DrawBMP(16,6,24,2,bmp_arow); break;
		default: break;
	}
	
}

void display_05(void)		 //ҡ�˷���
{
	LX_adc=get_LX_adc();		//PA1				 
	LY_adc=get_LY_adc();		//PA5
	RX_adc=get_RX_adc();		//PA2	
	RY_adc=get_RY_adc();		//PA3	
	POWER_adc=get_POWER_adc();	//PA2	
	OLED_ShowString(0,0,"LX: ",16);
	OLED_ShowString(0,2,"LY: ",16);
	OLED_ShowString(64,0,"RX:  ",16);
	OLED_ShowString(64,2,"RY:   ",16);
	OLED_ShowString(0,4,"POWER:   ",16);
	OLED_ShowNum(24,0,LX_adc,4,16);
	OLED_ShowNum(24,2,LY_adc,4,16);
	OLED_ShowNum(88,0,RX_adc,4,16);
	OLED_ShowNum(88,2,RY_adc,4,16);
	OLED_ShowNum(48,4,POWER_adc,4,16);					
}

void display_06(void)		//��������
{
	OLED_ShowCHinese(0,0,16);
	OLED_ShowCHinese(16,0,17);
	OLED_ShowCHinese(32,0,6);
	OLED_ShowCHinese(48,0,7);
}

void display_07(void)		//2.4G����
{
	OLED_ShowCHinese(48,0,22);
	OLED_ShowCHinese(64,0,23);
	OLED_ShowCHinese(80,0,4);
	OLED_ShowCHinese(96,0,5);
	
	OLED_ShowCHinese(48,2,24);
	OLED_ShowCHinese(64,2,25);
	OLED_ShowCHinese(80,2,4);
	OLED_ShowCHinese(96,2,5);
	
	OLED_ShowCHinese(48,4,28);
	OLED_ShowCHinese(64,4,29);
	OLED_ShowCHinese(80,4,30);
	OLED_ShowCHinese(96,4,31);
	
	switch(NextMode)
	{
		case 1:OLED_DrawBMP(16,0,24,2,bmp_arow); break;
		case 2:OLED_DrawBMP(16,2,24,2,bmp_arow); break;
		case 3:OLED_DrawBMP(16,4,24,2,bmp_arow); break;
		default: break;
	}
}

void display_08(void)		//���͵���
{
	OLED_ShowCHinese(0,0,22);
	OLED_ShowCHinese(16,0,23);
	OLED_ShowCHinese(32,0,26);
	OLED_ShowCHinese(48,0,27);
	OLED_ShowChar(64,0,':',16);

}

void display_09(void)		//���յ���
{
	OLED_ShowCHinese(0,0,24);
	OLED_ShowCHinese(16,0,25);
	OLED_ShowCHinese(32,0,26);
	OLED_ShowCHinese(48,0,27);
	OLED_ShowChar(64,0,':',16);
}

void display_10(void)	  //��������
{
	if(key==KEY1_PRES)OLED_DrawBMP(56,2,16,2,bmp_key_press);
	else OLED_DrawBMP(56,2,16,2,bmp_key_up);
	if(key==KEY2_PRES)OLED_DrawBMP(72,4,16,2,bmp_key_press);
	else OLED_DrawBMP(72,4,16,2,bmp_key_up);
	if(key==KEY3_PRES)OLED_DrawBMP(40,4,16,2,bmp_key_press);
	else OLED_DrawBMP(40,4,16,2,bmp_key_up);
	if(key==KEY4_PRES)OLED_DrawBMP(56,6,16,2,bmp_key_press);
	else OLED_DrawBMP(56,6,16,2,bmp_key_up);
}

void display_11(void)	  //��ʾ��̬��
{
	OLED_ShowCHinese(0,0,28);
	OLED_ShowCHinese(16,0,29);
	OLED_ShowCHinese(32,0,30);
	OLED_ShowCHinese(48,0,31);
	
	OLED_ShowString(0,2,"Picth:",16);
	OLED_ShowString(0,4,"Roll: ",16);
	OLED_ShowString(0,6,"Yaw:  ",16);
}

void display_12(void)	  //��ʾģʽ����
{	
	OLED_ShowCHinese(48,0,36);
	OLED_ShowCHinese(64,0,37);
	OLED_ShowCHinese(80,0,0);
	OLED_ShowCHinese(96,0,1);
	switch(NextMode)
	{
		case 1:OLED_DrawBMP(16,0,24,2,bmp_arow); break;
		default: break;
	}
}


void display_13(void)		//�������
{
	OLED_ShowCHinese(0,0,49);
	OLED_ShowCHinese(16,0,50);
	OLED_ShowCHinese(32,0,4);
	OLED_ShowCHinese(48,0,5);
	
	
	switch(NextMode)
	{	
		case 1: case 2: case 3:
			OLED_ShowCHinese(16,2,51); OLED_ShowCHinese(32,2,52); OLED_ShowChar(48,2,':',16); OLED_ShowNum(56,2,Throttle,4,16);	OLED_ShowChar(88,2,';',16);//����		
			show_gyro_despoint();
			break;
		case 4: case 5: case 6:
			OLED_ShowString(64,0,"Pitch",16);
			OLED_ShowCHinese(16,2,38); OLED_ShowCHinese(32,2,40); OLED_ShowChar(48,2,'P',16); OLED_ShowChar(56,2,':',16);	//pitch�ڻ�P			
		  OLED_ShowCHinese(16,4,38); OLED_ShowCHinese(32,4,40); OLED_ShowChar(48,4,'I',16);	OLED_ShowChar(56,4,':',16);	//pitch�ڻ�I
			OLED_ShowCHinese(16,6,38); OLED_ShowCHinese(32,6,40);	OLED_ShowChar(48,6,'D',16);	OLED_ShowChar(56,6,':',16);	//pitch�ڻ�D
			show_pid(ctrl.pitch.core.kp, ctrl.pitch.core.ki, ctrl.pitch.core.kd);
			break;
		case 7: case 8: case 9:
			OLED_ShowString(64,0,"Roll",16);
			OLED_ShowCHinese(16,2,38); OLED_ShowCHinese(32,2,40); OLED_ShowChar(48,2,'P',16);	OLED_ShowChar(56,2,':',16);	//roll�ڻ�P
			OLED_ShowCHinese(16,4,38); OLED_ShowCHinese(32,4,40); OLED_ShowChar(48,4,'I',16);	OLED_ShowChar(56,4,':',16);	//roll�ڻ�I
			OLED_ShowCHinese(16,6,38); OLED_ShowCHinese(32,6,40); OLED_ShowChar(48,6,'D',16);	OLED_ShowChar(56,6,':',16);	//roll�ڻ�D
			show_pid(ctrl.roll.core.kp, ctrl.roll.core.ki, ctrl.roll.core.kd);
			break;				
	 case 10: case 11: case 12:
			OLED_ShowString(64,0,"Pitch",16);
			OLED_ShowCHinese(16,2,39); OLED_ShowCHinese(32,2,40); OLED_ShowChar(48,2,'P',16);	OLED_ShowChar(56,2,':',16);	//pitch�⻷P
			OLED_ShowCHinese(16,4,39); OLED_ShowCHinese(32,4,40); OLED_ShowChar(48,4,'I',16);	OLED_ShowChar(56,4,':',16);	//pitch�⻷I
			OLED_ShowCHinese(16,6,39); OLED_ShowCHinese(32,6,40); OLED_ShowChar(48,6,'D',16);	OLED_ShowChar(56,6,':',16);	//pitch�⻷D
			show_pid(ctrl.pitch.shell.kp, ctrl.pitch.shell.ki, ctrl.pitch.shell.kd);
			break;	
	 case 13: case 14: case 15:
			OLED_ShowString(64,0,"Roll",16);
			OLED_ShowCHinese(16,2,39); OLED_ShowCHinese(32,2,40); OLED_ShowChar(48,2,'P',16);	OLED_ShowChar(56,2,':',16);	//roll�⻷P
			OLED_ShowCHinese(16,4,39); OLED_ShowCHinese(32,4,40); OLED_ShowChar(48,4,'I',16);	OLED_ShowChar(56,4,':',16);	//roll�⻷I
			OLED_ShowCHinese(16,6,39); OLED_ShowCHinese(32,6,40); OLED_ShowChar(48,6,'D',16);	OLED_ShowChar(56,6,':',16);	//roll�⻷D
			show_pid(ctrl.roll.shell.kp, ctrl.roll.shell.ki, ctrl.roll.shell.kd);
			break;	
	 case 16: case 17: case 18:
			OLED_ShowString(64,0,"YAW",16);
			OLED_ShowCHinese(16,2,38); OLED_ShowCHinese(32,2,40); OLED_ShowChar(48,2,'P',16);	OLED_ShowChar(56,2,':',16);	//yaw�ڻ�P
			OLED_ShowCHinese(16,4,38); OLED_ShowCHinese(32,4,40); OLED_ShowChar(48,4,'I',16);	OLED_ShowChar(56,4,':',16);	//yaw�ڻ�I
			OLED_ShowCHinese(16,6,38); OLED_ShowCHinese(32,6,40); OLED_ShowChar(48,6,'D',16);	OLED_ShowChar(56,6,':',16);	//yaw�ڻ�D
			show_pid(ctrl.yaw.core.kp, ctrl.yaw.core.ki, ctrl.yaw.core.kd);
			break;
	 case 19: case 20: case 21:
			OLED_ShowString(64,0,"YAW",16);
			OLED_ShowCHinese(16,2,39); OLED_ShowCHinese(32,2,40); OLED_ShowChar(48,2,'P',16);	OLED_ShowChar(56,2,':',16);	//yaw�⻷P
			OLED_ShowCHinese(16,4,39); OLED_ShowCHinese(32,4,40); OLED_ShowChar(48,4,'I',16);	OLED_ShowChar(56,4,':',16);	//yaw�⻷I
			OLED_ShowCHinese(16,6,39); OLED_ShowCHinese(32,6,40); OLED_ShowChar(48,6,'D',16);	OLED_ShowChar(56,6,':',16);	//yaw�⻷D
			show_pid(ctrl.yaw.shell.kp, ctrl.yaw.shell.ki, ctrl.yaw.shell.kd);
			break;
		default: break;
		
	}
	//��ʾ��ͷ
	switch(NextMode)
	{
		case 1:case 4:case 7:case 10:case 13:case 16:case 19: OLED_DrawBMP(0,2,16,2,bmp_rocker_circle); break;
		case 2:case 5:case 8:case 11:case 14:case 17:case 20: OLED_DrawBMP(0,4,16,2,bmp_rocker_circle); break;
		case 3:case 6:case 9:case 12:case 15:case 18:case 21: OLED_DrawBMP(0,6,16,2,bmp_rocker_circle); break;
		default: break;
	}
	
}

void show_gyro_despoint(void)
{
	//��ʾ���ٶ�Ŀ��ֵ
	//��display_13() ����
	int int_part,dec_part;
	
	OLED_ShowString(16,4,"alt.kp",16);
	OLED_ShowChar(64,4,':',16);
	int_part = pitch_core_desPoint;										//��������	
	dec_part = (pitch_core_desPoint	-int_part)*10;			//С������			
	int_part = abs(int_part);							//ȡ����ֵ
	dec_part = abs(dec_part);
	if(pitch_core_desPoint	<0 ) OLED_ShowChar(72,4,'-',16); 	
	else OLED_ShowChar(72,4,' ' ,16);			//��ʾ+-��
	OLED_ShowNum(80,4,int_part,3,16); 		//��ʾ����
	OLED_ShowChar(104,4,'.',16);					//��ʾС����
	OLED_ShowNum(112,4,dec_part,1,16);		//��ʾС��
	
	OLED_ShowString(16,6,"alt.ki",16);
	OLED_ShowChar(64,6,':',16);
	int_part = roll_core_desPoint;										//��������	
	dec_part = (roll_core_desPoint	-int_part)*10;			//С������			
	int_part = abs(int_part);							//ȡ����ֵ
	dec_part = abs(dec_part);
	if(roll_core_desPoint	<0 ) OLED_ShowChar(72,6,'-',16); 	
	else OLED_ShowChar(72,6,' ' ,16);			//��ʾ+-��
	OLED_ShowNum(80,6,int_part,3,16); 		//��ʾ����
	OLED_ShowChar(104,6,'.',16);					//��ʾС����
	OLED_ShowNum(112,6,dec_part,1,16);		//��ʾС��
	
//	OLED_ShowString(16,4,"Gyro_Y",16);OLED_ShowChar(64,4,':',16); OLED_ShowNum(72,4,pitch_core_desPoint,4,16);//Gyro_Pitch
//	OLED_ShowString(16,6,"Gyro_X",16);OLED_ShowChar(64,6,':',16); OLED_ShowNum(72,6,roll_core_desPoint,4,16);//Gyro_Roll
}

void show_pid(float kp, float ki, float kd)
{
	//��display_13() ����
	int int_part,dec_part;
	
	int_part = kp	;												//��������	
	dec_part = (kp	-int_part)*10;				//С������			
	int_part = abs(int_part);							//ȡ����ֵ
	dec_part = abs(dec_part);
	if(kp	<0 ) OLED_ShowChar(64,2,'-',16); 	
	else OLED_ShowChar(64,2,' ' ,16);			//��ʾ+-��
	OLED_ShowNum(80,2,int_part,3,16); 		//��ʾ����
	OLED_ShowChar(104,2,'.',16);					//��ʾС����
	OLED_ShowNum(112,2,dec_part,1,16);		//��ʾС��

	int_part = ki	;												//��������	
	dec_part = (ki	-int_part)*10;				//С������			
	int_part = abs(int_part);							//ȡ����ֵ
	dec_part = abs(dec_part);
	if(ki	<0 ) OLED_ShowChar(64,4,'-',16); 	
	else OLED_ShowChar(64,4,' ' ,16);			//��ʾ+-��
	OLED_ShowNum(80,4,int_part,3,16); 		//��ʾ����
	OLED_ShowChar(104,4,'.',16);					//��ʾС����
	OLED_ShowNum(112,4,dec_part,1,16);		//��ʾС��

	int_part = kd	;												//��������	
	dec_part = (kd	-int_part)*10;				//С������			
	int_part = abs(int_part);							//ȡ����ֵ
	dec_part = abs(dec_part);
	if(kd	<0 ) OLED_ShowChar(64,6,'-',16); 	
	else OLED_ShowChar(64,6,' ' ,16);			//��ʾ+-��
	OLED_ShowNum(80,6,int_part,3,16); 		//��ʾ����
	OLED_ShowChar(104,6,'.',16);					//��ʾС����
	OLED_ShowNum(112,6,dec_part,1,16);		//��ʾС��
	
	
}







