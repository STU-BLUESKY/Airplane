#include "control.h"	
#include "sys.h"
#include "LED.h"
#include "delay.h"
#include "IMU.h"
//RC_GETDATA RC_Data; // 控制变量数据
struct _ctrl ctrl;
extern float GY;
extern int RE[5];

int16_t Moto_duty[4];

//定时器3中断服务程序
void TIM3_IRQHandler(void)		    //??ms中断一次
{	
	static u16 ms1=0;	//中断次数计数器
	
	if(TIM3->SR & TIM_IT_Update)	
	{     
		ms1++;
	  paramLoad();  //重载和更新数据值
		
		Prepare_Data();
	  Get_Attitude();
		CONTROL(angle.roll,angle.pitch,0); 
		if(ms1==4)
		{
			ms1 = 0;
		 }		
	 }
	 TIM3->SR = ~TIM_FLAG_Update;//清除中断标志	
}



//void Get_angle(void)
//{
////	READ_MPU6050_Gyro();
////	READ_MPU6050_Accel();




//}






/***************************************************/
/*void CONTROL(float rol, float pit, float yaw)    */
/*输入：rol   横滚角                               */
/*      pit   俯仰角                               */
/*			yaw   航向                                 */
/*输出：                                           */
/*备注：串级PID 控制   外环（角度环）采用PID调节    */
/*                     内环（角速度环）采用PD调节  */
/***************************************************/
void CONTROL(float rol, float pit, float yaw)   
{
	static float roll_old,pitch_old;
	int date_throttle	= RE[1]*10;
	if(ctrl.ctrlRate >= 2)
	{
		//*****************外环(角度环)PID**************************//
		//横滚计算///////////////
	  // pit = pit + RC_Data.PITCH/1.4;
		pit = pit+RE[2];
		ctrl.pitch.shell.increment += pit;
		
		//limit for the max increment
		if(ctrl.pitch.shell.increment > ctrl.pitch.shell.increment_max)  	ctrl.pitch.shell.increment = ctrl.pitch.shell.increment_max;
		else if(ctrl.pitch.shell.increment < -ctrl.pitch.shell.increment_max)		ctrl.pitch.shell.increment = -ctrl.pitch.shell.increment_max;
		
		ctrl.pitch.shell.pid_out = ctrl.pitch.shell.kp * pit + ctrl.pitch.shell.ki * ctrl.pitch.shell.increment + ctrl.pitch.shell.kd * (pit - pitch_old);
		pitch_old = pit;
		
		//俯仰计算//////////////
		// rol = rol + RC_Data.ROLL/1.4;
		rol = rol+RE[3];
		ctrl.roll.shell.increment += rol;
		
		//limit for the max increment
		if(ctrl.roll.shell.increment > ctrl.roll.shell.increment_max)  	ctrl.roll.shell.increment = ctrl.roll.shell.increment_max;
		else if(ctrl.roll.shell.increment < -ctrl.roll.shell.increment_max)		ctrl.roll.shell.increment = -ctrl.roll.shell.increment_max;

		ctrl.roll.shell.pid_out  = ctrl.roll.shell.kp * rol + ctrl.roll.shell.ki * ctrl.roll.shell.increment + ctrl.roll.shell.kd * (rol - roll_old);
		roll_old = rol;
		
		//航向计算/////////////
		//        1= RC_Data.YAW;
	  ctrl.yaw.shell.pid_out = RE[4]+ctrl.yaw.shell.kp *1 + ctrl.yaw.shell.kd * sensor.gyro.origin.z;
    ctrl.ctrlRate = 0; 
	}
	ctrl.ctrlRate ++;
  //********************内环(角速度环)PD*********************************//
	ctrl.roll.core.kp_out = ctrl.roll.core.kp * (ctrl.roll.shell.pid_out + sensor.gyro.radian.x * RtA);
	ctrl.roll.core.kd_out = ctrl.roll.core.kd * (sensor.gyro.origin.x - sensor.gyro.histor.x);
	
	ctrl.pitch.core.kp_out = ctrl.pitch.core.kp * (ctrl.pitch.shell.pid_out + sensor.gyro.radian.y * RtA);
	ctrl.pitch.core.kd_out = ctrl.pitch.core.kd * (sensor.gyro.origin.y - sensor.gyro.histor.y);
	
	ctrl.yaw.core.kp_out = ctrl.yaw.core.kp * (ctrl.yaw.shell.pid_out + sensor.gyro.radian.z * RtA);
	ctrl.yaw.core.kd_out = ctrl.yaw.core.kd * (sensor.gyro.origin.z - sensor.gyro.histor.z);
	
	ctrl.roll.core.pid_out = ctrl.roll.core.kp_out + ctrl.roll.core.kd_out;
	ctrl.pitch.core.pid_out = ctrl.pitch.core.kp_out + ctrl.pitch.core.kd_out;
	ctrl.yaw.core.pid_out = ctrl.yaw.core.kp_out + ctrl.yaw.core.kd_out;

	sensor.gyro.histor.x = sensor.gyro.origin.x;   //储存历史值
	sensor.gyro.histor.y = sensor.gyro.origin.y;
  sensor.gyro.histor.z = sensor.gyro.origin.z;
	
	
	if(date_throttle>40)
	{
		         
		date_throttle=date_throttle/cos(angle.roll/RtA)/cos(angle.pitch/RtA);
		Moto_duty[0] = date_throttle + ctrl.roll.core.pid_out - ctrl.pitch.core.pid_out - ctrl.yaw.core.pid_out;
		Moto_duty[1] = date_throttle + ctrl.roll.core.pid_out + ctrl.pitch.core.pid_out + ctrl.yaw.core.pid_out;
		Moto_duty[2] = date_throttle - ctrl.roll.core.pid_out + ctrl.pitch.core.pid_out - ctrl.yaw.core.pid_out;
		Moto_duty[3] = date_throttle - ctrl.roll.core.pid_out - ctrl.pitch.core.pid_out + ctrl.yaw.core.pid_out;
	}
	else
	{	
    Moto_duty[0] = Moto_duty[1] = Moto_duty[2] = Moto_duty[3] =0;	
		ctrl.pitch.shell.increment = 0;
		ctrl.roll.shell.increment= 0;		

	}
//	
//	if(RC_Data.ARMED)  moto_PwmRflash(&Moto_duty[0]);	
		motor(Moto_duty[2],Moto_duty[1],Moto_duty[3],Moto_duty[0] );
//		motor(50,50,50,50);
//	else       moto_STOP();		
}














