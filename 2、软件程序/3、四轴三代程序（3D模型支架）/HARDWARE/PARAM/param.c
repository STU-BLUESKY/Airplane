#include "param.h"
#include "pid.h"
#include "imu.h"

void paramLoad(void)
{
	//内环参数
	ctrl.pitch.core.Kp = 0.0;				//1.3
	ctrl.pitch.core.Kp = 0.0;	
	ctrl.pitch.core.Kd = 0.0;				//2.4
	
	ctrl.roll.core.Kp = 0.00;					//1.6
	ctrl.roll.core.Kp = 0.00;	
	ctrl.roll.core.Kd = 0.00;					//2.00
	
	ctrl.yaw.core.Kp = 0.0;
	ctrl.yaw.core.Kd = 0.0;
	
	//外环参数
	ctrl.pitch.shell.Kp = 0.0;				//1.3
	ctrl.pitch.shell.Ki = 0.0;
	ctrl.pitch.shell.Kd = 0.0;    
	
	ctrl.roll.shell.Kp = 0.0;				
	ctrl.roll.shell.Ki = 0.0;	
	ctrl.roll.shell.Kd = 0.0;
	
	ctrl.yaw.shell.Kp = 0.0;						
	ctrl.yaw.shell.Ki = 0.0;
	ctrl.yaw.shell.Kd = 0.0;
	
	//目标归零
	ctrl.pitch.core.desPoint = 0.0;
	ctrl.roll.core.desPoint = 0.0;
	
	ctrl.yaw.shell.desPoint = EulerAngle.Yaw;
	
	//积分限幅
	ctrl.pitch.core.iLimit  = 10;
	ctrl.roll.core.iLimit 	= 10;
	ctrl.pitch.shell.iLimit = 5;
	ctrl.roll.shell.iLimit 	= 5;
	//积分清零
	clr_int();
	
}

void clr_int(void)
{
	//积分清零
	ctrl.pitch.shell.increment = 0.0;
	ctrl.roll.shell.increment = 0.0;
	ctrl.pitch.core.increment = 0.0;
	ctrl.roll.core.increment = 0.0;
}







