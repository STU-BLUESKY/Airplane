#include "pid.h"
#include "imu.h"
#include "mpu6050.h"
#include "control.h"
#include "imu.h"

CasPIDTypdDef ctrl;

int Throttle = 0;		//油门输入量 
int basethrottle=0;
int extrathrottle=0; 
int holdthrottle=0;
float z_speed;


_PWM Motor_PWM;
PIDTypdDef alti;

void Cascade_PID_Calc(float pit, float rol, float yaw)
{
	static float errorPit,errorRol,errorYaw;
	
	errorPit = pit-ctrl.pitch.shell.desPoint;					//误差
	errorRol = rol-ctrl.roll.shell.desPoint;					//误差
	errorYaw = yaw-ctrl.yaw.shell.desPoint;
	//外环(角度环)PID计算
	ctrl.pitch.shell.increment += errorPit;		//外环积分
	ctrl.roll.shell.increment += errorRol;
	ctrl.yaw.shell.increment += errorYaw;
	//Pitch角
	if(ctrl.pitch.shell.increment > ctrl.pitch.shell.iLimit)  				ctrl.pitch.shell.increment = ctrl.pitch.shell.iLimit;		
	else if(ctrl.pitch.shell.increment < -ctrl.pitch.shell.iLimit)		ctrl.pitch.shell.increment = -ctrl.pitch.shell.iLimit;
	ctrl.pitch.shell.pid_out = ctrl.pitch.shell.Kp * errorPit + ctrl.pitch.shell.Ki * ctrl.pitch.shell.increment + ctrl.pitch.shell.Kd *(errorPit-ctrl.pitch.shell.Error); //ctrl.pitch.shell.Kd * sensor.gyro.cal.y; 
	ctrl.pitch.shell.Error = errorPit;
	//Roll角
	if(ctrl.roll.shell.increment > ctrl.roll.shell.iLimit)  				ctrl.roll.shell.increment = ctrl.roll.shell.iLimit;		
	else if(ctrl.roll.shell.increment < -ctrl.roll.shell.iLimit)		ctrl.roll.shell.increment = -ctrl.roll.shell.iLimit;
	ctrl.roll.shell.pid_out  = ctrl.roll.shell.Kp * errorRol + ctrl.roll.shell.Ki * ctrl.roll.shell.increment +  ctrl.roll.shell.Kd *(errorRol-ctrl.roll.shell.Error); //ctrl.roll.shell.Kd * sensor.gyro.cal.x;		
	ctrl.roll.shell.Error = errorRol;
	//Yaw角
	if(ctrl.yaw.shell.increment > ctrl.yaw.shell.iLimit)  				ctrl.yaw.shell.increment = ctrl.yaw.shell.iLimit;		
	else if(ctrl.yaw.shell.increment < -ctrl.yaw.shell.iLimit)		ctrl.yaw.shell.increment = -ctrl.yaw.shell.iLimit;
	ctrl.yaw.shell.pid_out = ctrl.yaw.shell.Kp * errorYaw + ctrl.yaw.shell.Ki * ctrl.yaw.shell.increment + ctrl.yaw.shell.Kd * (errorYaw - ctrl.yaw.shell.Error);
	ctrl.yaw.shell.Error = errorYaw;
	
	//内环(角速度环)PD计算
	ctrl.roll.core.increment += (sensor.gyro.cal.x - ctrl.roll.core.desPoint);
	if(ctrl.roll.core.increment > ctrl.roll.core.iLimit) 					ctrl.roll.core.increment = ctrl.roll.core.iLimit;
	else if(ctrl.roll.core.increment < -ctrl.roll.core.iLimit)		ctrl.roll.core.increment = -ctrl.roll.core.iLimit;
	
	ctrl.pitch.core.increment += (sensor.gyro.cal.y - ctrl.pitch.core.desPoint);
	if(ctrl.pitch.core.increment > ctrl.pitch.core.iLimit) 					ctrl.pitch.core.increment = ctrl.pitch.core.iLimit;
	else if( ctrl.pitch.core.increment < -ctrl.pitch.core.iLimit)		ctrl.pitch.core.increment = -ctrl.pitch.core.iLimit;
	
	ctrl.yaw.core.increment += (sensor.gyro.cal.z - ctrl.yaw.core.desPoint);
	if(ctrl.yaw.core.increment > ctrl.yaw.core.iLimit) 					ctrl.yaw.core.increment = ctrl.yaw.core.iLimit;
	else if( ctrl.yaw.core.increment < -ctrl.yaw.core.iLimit)		ctrl.yaw.core.increment = -ctrl.yaw.core.iLimit;
	
	ctrl.roll.core.kp_out = ctrl.roll.core.Kp * (sensor.gyro.cal.x) + ctrl.roll.shell.pid_out;
	ctrl.roll.core.ki_out = ctrl.roll.core.Ki * ctrl.roll.core.increment;
	ctrl.roll.core.kd_out = ctrl.roll.core.Kd * (sensor.gyro.cal.x - sensor.gyro.histor.x) ;
		
	ctrl.pitch.core.kp_out = ctrl.pitch.core.Kp * (sensor.gyro.cal.y) + ctrl.pitch.shell.pid_out ;
	ctrl.pitch.core.ki_out = ctrl.pitch.core.Ki * ctrl.pitch.core.increment;
	ctrl.pitch.core.kd_out = ctrl.pitch.core.Kd * (sensor.gyro.cal.y - sensor.gyro.histor.y);
	
	ctrl.yaw.core.kp_out = ctrl.yaw.core.Kp * (sensor.gyro.cal.z) + ctrl.yaw.shell.pid_out ;
	ctrl.yaw.core.ki_out = ctrl.yaw.core.Ki * ctrl.yaw.core.increment;
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


float Altitude_PID( float a )
{
	static float errorZ;
	float pid_out;
	
	errorZ = a-alti.desPoint;
	if(f_hold)
	{
		alti.increment += errorZ;
		if(alti.increment > alti.iLimit)  				alti.increment =  alti.iLimit;		
		else if(alti.increment < -alti.iLimit)		alti.increment = -alti.iLimit;
		pid_out = alti.Kp*errorZ + alti.Ki*alti.increment + alti.Kd*(errorZ - alti.Error);
		alti.Error = errorZ;	
		extrathrottle = pid_out;
		detecZspeed();
		if(holdthrottle != 0)
		{
			Throttle = holdthrottle + extrathrottle;
		}
		else Throttle = basethrottle + extrathrottle;
	}
	else Throttle = basethrottle;
	
	return 0;
}

void detecZspeed(void)
{
	float accz,error;
//	static u8 t=0;
	accz = z_NED;
	error = accz - CONSTANTS_ONE_G;
	z_speed += error*0.1;
	
}
