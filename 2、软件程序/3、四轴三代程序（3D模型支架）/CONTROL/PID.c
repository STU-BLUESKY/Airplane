#include "pid.h"
#include "imu.h"
#include "mpu6050.h"
 
CasPIDTypdDef ctrl;

int Throttle = 0;		//���������� 

_PWM Motor_PWM;

void Cascade_PID_Calc(float pit, float rol, float yaw)
{
	static float errorPit,errorRol,errorYaw;
	
	errorPit = pit-ctrl.pitch.shell.desPoint;					//���
	errorRol = rol-ctrl.roll.shell.desPoint;					//���
	errorYaw = yaw-ctrl.yaw.shell.desPoint;
	//�⻷(�ǶȻ�)PID����
	ctrl.pitch.shell.increment += errorPit;		//�⻷����
	ctrl.roll.shell.increment += errorRol;
	//�����޷�
	if(ctrl.pitch.shell.increment > ctrl.pitch.shell.iLimit)  				ctrl.pitch.shell.increment = ctrl.pitch.shell.iLimit;		
	else if(ctrl.pitch.shell.increment < -ctrl.pitch.shell.iLimit)		ctrl.pitch.shell.increment = -ctrl.pitch.shell.iLimit;
	ctrl.pitch.shell.pid_out = ctrl.pitch.shell.Kp * errorPit + ctrl.pitch.shell.Ki * ctrl.pitch.shell.increment + ctrl.pitch.shell.Kd * sensor.gyro.cal.y;
	ctrl.pitch.shell.Error = errorPit;
	//�����޷�
	if(ctrl.roll.shell.increment > ctrl.roll.shell.iLimit)  				ctrl.roll.shell.increment = ctrl.roll.shell.iLimit;		
	else if(ctrl.roll.shell.increment < -ctrl.roll.shell.iLimit)		ctrl.roll.shell.increment = -ctrl.roll.shell.iLimit;
	ctrl.roll.shell.pid_out  = ctrl.roll.shell.Kp * errorRol + ctrl.roll.shell.Ki * ctrl.roll.shell.increment + ctrl.roll.shell.Kd * sensor.gyro.cal.x;
	ctrl.roll.shell.Error = errorRol;
	//Yaw��
	ctrl.yaw.shell.pid_out = ctrl.yaw.shell.Kp * errorYaw;
	
	//�ڻ�(���ٶȻ�)PD����
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
	
	//������ʷֵ
	sensor.gyro.histor.x = sensor.gyro.cal.x;   
	sensor.gyro.histor.y = sensor.gyro.cal.y;
  sensor.gyro.histor.z = sensor.gyro.cal.z;
	
	ctrl.roll.core.pid_out = ctrl.roll.core.kp_out + ctrl.roll.core.ki_out + ctrl.roll.core.kd_out;
	ctrl.pitch.core.pid_out = ctrl.pitch.core.kp_out + ctrl.pitch.core.ki_out + ctrl.pitch.core.kd_out;
	ctrl.yaw.core.pid_out = ctrl.yaw.core.kp_out + ctrl.yaw.core.ki_out + ctrl.yaw.core.kd_out;

	Motor_PWM.M1 = Throttle + ctrl.pitch.core.pid_out + ctrl.roll.core.pid_out - ctrl.yaw.core.pid_out;		//�Ե�PWM	
	Motor_PWM.M2 = Throttle + ctrl.pitch.core.pid_out - ctrl.roll.core.pid_out + ctrl.yaw.core.pid_out;
	Motor_PWM.M3 = Throttle - ctrl.pitch.core.pid_out - ctrl.roll.core.pid_out - ctrl.yaw.core.pid_out;
	Motor_PWM.M4 = Throttle - ctrl.pitch.core.pid_out + ctrl.roll.core.pid_out + ctrl.yaw.core.pid_out;

}

