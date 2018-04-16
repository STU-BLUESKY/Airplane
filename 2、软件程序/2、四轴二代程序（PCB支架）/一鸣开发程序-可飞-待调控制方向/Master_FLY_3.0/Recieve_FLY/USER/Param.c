/******************** (C) COPYRIGHT 2014 Air Nano Team ***************************
 * 文件名  ：PARAM.c
 * 描述    ：参数加载         
 * 实验平台：Air Nano四轴飞行器
 * 库版本  ：ST3.5.0
 * 作者    ：Air Nano Team 
 * 淘宝    ：http://byd2.taobao.com
**********************************************************************************/
#include "param.h"
extern int RE[4];  /////+++++++++++++++++++++++++++++++++++////
extern u16 adcx;
uint16_t VirtAddVarTab[NumbOfVar] = {0xAA00, 0xAA01, 0xAA02, 0xAA03, 0xAA04, 0xAA05, 0xAA06, 0xAA07, 0xAA08, 0xAA09, 
																		 0xAA0A,0xAA0B, 0xAA0C, 0xAA0D, 0xAA0E,};

																																	 
//static void EE_READ_ACC_OFFSET(void)
//{
//	EE_ReadVariable(VirtAddVarTab[0], &sensor.acc.quiet.x);
//	EE_ReadVariable(VirtAddVarTab[1], &sensor.acc.quiet.y);
//	EE_ReadVariable(VirtAddVarTab[2], &sensor.acc.quiet.z);
//}

//void EE_SAVE_ACC_OFFSET(void)
//{
//  EE_WriteVariable(VirtAddVarTab[0],sensor.acc.quiet.x);
//  EE_WriteVariable(VirtAddVarTab[1],sensor.acc.quiet.y);
//	EE_WriteVariable(VirtAddVarTab[2],sensor.acc.quiet.z);
//}	


//**************************************************************************
//参数加载
//**************************************************************************
void	paramLoad(void)
{
	//EE_READ_ACC_OFFSET(); //读取加速度零偏
	float k_ADC=0.0009;
	
	//The data of pitch
	ctrl.pitch.shell.kp = 8;// RE[0]/100.0;//0;  //8;  //修改范围：+-10    
	ctrl.pitch.shell.ki = 0;  //0.04;//RE[1]/5000;   //0.04;         +-1
	ctrl.pitch.shell.kd = 0;  //4;//RE[2]/500;    //4;            +-10
	// 内环
	ctrl.pitch.core.kp = 3.75;//6.66;//RE[0]/100.0;    //9;  best : 375/100.0
	ctrl.pitch.core.kd = 1.26; // //3.5;// best 126/100.0    best : 126/100.0
	// 外环
	//The data of roll
	ctrl.roll.shell.kp = 8;//RE[0]/100.0;//0;  //8;    //8;
	ctrl.roll.shell.ki = 0;  //0.04;
	ctrl.roll.shell.kd = 0;  //4;
  // 内环
	ctrl.roll.core.kp = 3.75;//100.0;//6.66;// RE[0]/100.0;  //9;
	ctrl.roll.core.kd = 1.26; //0;  //3.5;////// 
	
	//The data of yaw
	ctrl.yaw.shell.kp = 0;  //2.21;
	ctrl.yaw.shell.kd = 0;  //0.04;
	
	ctrl.yaw.core.kp = -2.28;//0;//RE[1]/100.0;//0;  //8;  best : 228/100.0
	ctrl.yaw.core.kd = 1.10;//RE[0]/100.0;//0;  //4.5;                   best: 110/100.0
	//limit for the max increment
	ctrl.pitch.shell.increment_max = 200;
	ctrl.roll.shell.increment_max = 200;
	
	ctrl.ctrlRate = 0;
//++++++++++++++++++++++++++++++++
	ctrl.pitch.core.kp=ctrl.pitch.core.kp*(5000-adcx)*k_ADC;
	ctrl.roll.core.kp=ctrl.roll.core.kp*(5000-adcx)*k_ADC;
	ctrl.yaw.core.kp=ctrl.yaw.core.kp*(5000-adcx)*k_ADC;
//++++++++++++++++++++++++++++++++
//	RC_Data.pitch_offset = 1900;
//	RC_Data.roll_offset = 2000;
//	RC_Data.yaw_offset = 2050;
}
