#include "pid.h"
#include "imu.h"
#include "mpu6050.h"
 
CasPIDTypdDef ctrl;

int Throttle = 0;		//油门输入量 

_PWM Motor_PWM;

void Cascade_PID_Calc(float pit, float rol, float yaw)
{
	static float errorPit,errorRol,errorYaw;
	
	errorPit = pit-ctrl.pitch.shell.desPoint;					//误差
	errorRol = rol-ctrl.roll.shell.desPoint;					//误差
	errorYaw = yaw-ctrl.yaw.shell.desPoint;
	//外环(角度环)PID计算
	ctrl.pitch.shell.increment += errorPit;		//外环积分
	ctrl.roll.shell.increment += errorRol;
	//积分限幅
	if(ctrl.pitch.shell.increment > ctrl.pitch.shell.iLimit)  				ctrl.pitch.shell.increment = ctrl.pitch.shell.iLimit;		
	else if(ctrl.pitch.shell.increment < -ctrl.pitch.shell.iLimit)		ctrl.pitch.shell.increment = -ctrl.pitch.shell.iLimit;
	ctrl.pitch.shell.pid_out = ctrl.pitch.shell.Kp * errorPit + ctrl.pitch.shell.Ki * ctrl.pitch.shell.increment + ctrl.pitch.shell.Kd * sensor.gyro.cal.y;
	ctrl.pitch.shell.Error = errorPit;
	//积分限幅
	if(ctrl.roll.shell.increment > ctrl.roll.shell.iLimit)  				ctrl.roll.shell.increment = ctrl.roll.shell.iLimit;		
	else if(ctrl.roll.shell.increment < -ctrl.roll.shell.iLimit)		ctrl.roll.shell.increment = -ctrl.roll.shell.iLimit;
	ctrl.roll.shell.pid_out  = ctrl.roll.shell.Kp * errorRol + ctrl.roll.shell.Ki * ctrl.roll.shell.increment + ctrl.roll.shell.Kd * sensor.gyro.cal.x;
	ctrl.roll.shell.Error = errorRol;
	//Yaw角
	ctrl.yaw.shell.pid_out = ctrl.yaw.shell.Kp * errorYaw;
	
	//内环(角速度环)PD计算
	ctrl.roll.core.increment += (sensor.gyro.cal.x - ctrl.roll.core.desPoint);
	if(ctrl.roll.core.increment > ctrl.roll.core.iLimit) 					ctrl.roll.core.increment = ctrl.roll.core.iLimit;
	else if(ctrl.roll.core.increment < -ctrl.roll.core.iLimit)		ctrl.roll.core.increment = -ctrl.roll.core.iLimit;
	
	ctrl.pitch.core.increment += (sensor.gyro.cal.y - ctrl.pitch.core.desPoint);
	if(ctrl.pitch.core.increment > ctrl.pitch.core.iLimit) 					ctrl.pitch.core.increment = ctrl.pitch.core.iLimit;
	else if( ctrl.pitch.core.increment < -ctrl.pitch.core.iLimit)		ctrl.pitch.core.increment = -ctrl.pitch.core.iLimit;
	
	ctrl.roll.core.kp_out = ctrl.roll.core.Kp * (sensor.gyro.cal.x - ctrl.roll.core.desPoint ) + ctrl.roll.shell.pid_out;
	ctrl.roll.core.ki_out = ctrl.roll.core.Ki * ctrl.roll.core.increment;
	ctrl.roll.core.kd_out = ctrl.roll.core.Kd * (sensor.gyro.cal.x - sensor.gyro.histor.x) ;
		
	ctrl.pitch.core.kp_out = ctrl.pitch.core.Kp * (sensor.gyro.cal.y - ctrl.pitch.core.desPoint) + ctrl.pitch.shell.pid_out ;
	ctrl.pitch.core.ki_out = ctrl.pitch.core.Ki * ctrl.pitch.core.increment;
	ctrl.pitch.core.kd_out = ctrl.pitch.core.Kd * (sensor.gyro.cal.y - sensor.gyro.histor.y);
	
	ctrl.yaw.core.kp_out = ctrl.yaw.core.Kp * (ctrl.yaw.shell.pid_out + sensor.gyro.cal.z );
	ctrl.yaw.core.kd_out = ctrl.yaw.core.Kd * (sensor.gyro.cal.z - sensor.gyro.histor.z);
	
	//储存历史值
	sensor.gyro.histor.x = sensor.gyro.cal.x;   
	sensor.gyro.histor.y = sensor.gyro.cal.y;
  sensor.gyro.histor.z = sensor.gyro.cal.z;
	
	ctrl.roll.core.pid_out = ctrl.roll.core.kp_out + ctrl.roll.core.ki_out + ctrl.roll.core.kd_out;
	ctrl.pitch.core.pid_out = ctrl.pitch.core.kp_out + ctrl.pitch.core.ki_out + ctrl.pitch.core.kd_out;
	ctrl.yaw.core.pid_out = ctrl.yaw.core.kp_out + ctrl.yaw.core.ki_out + ctrl.yaw.core.kd_out;

	Motor_PWM.M1 = Throttle + ctrl.pitch.core.pid_out + ctrl.roll.core.pid_out - ctrl.yaw.core.pid_out;		//自调PWM	
	Motor_PWM.M2 = Throttle + ctrl.pitch.core.pid_out - ctrl.roll.core.pid_out + ctrl.yaw.core.pid_out;
	Motor_PWM.M3 = Throttle - ctrl.pitch.core.pid_out - ctrl.roll.core.pid_out - ctrl.yaw.core.pid_out;
	Motor_PWM.M4 = Throttle - ctrl.pitch.core.pid_out + ctrl.roll.core.pid_out + ctrl.yaw.core.pid_out;

}

