#include "param.h"
#include "pid.h"
#include "imu.h"

void paramLoad(void)
{
	//Pitch
	ctrl.pitch.core.Kp = 1.0;				
	ctrl.pitch.core.Ki = 0.0;	
	ctrl.pitch.core.Kd = 8.0;				
	ctrl.pitch.shell.Kp = 0.10;		
	ctrl.pitch.shell.Ki = 0.00;
	ctrl.pitch.shell.Kd = 0.30;   

	//Roll
	ctrl.roll.core.Kp = 1.00;				
	ctrl.roll.core.Ki = 0.00;	
	ctrl.roll.core.Kd = 8.00;			
	ctrl.roll.shell.Kp = 0.10;				
	ctrl.roll.shell.Ki = 0.00;	
	ctrl.roll.shell.Kd = 0.30;
	
	//Yaw
	ctrl.yaw.core.Kp = 6.00;						
	ctrl.yaw.core.Ki = 0.00;
	ctrl.yaw.core.Kd = 0.00;
	ctrl.yaw.shell.Kp = 0.00;						
	ctrl.yaw.shell.Ki = 0.00;
	ctrl.yaw.shell.Kd = 0.00;
	
	alti.Kp = 0.0;
	alti.Ki = 0.0;
	alti.Kd = 0.0;
	//目标归零
	ctrl.pitch.core.desPoint = 0.0;
	ctrl.roll.core.desPoint = 0.0;
	ctrl.yaw.core.desPoint = 0.0;
	ctrl.pitch.shell.desPoint = 0.0;
	ctrl.roll.shell.desPoint = 0.0;
	ctrl.yaw.shell.desPoint = 0.0;
	//积分限幅
	ctrl.pitch.core.iLimit  = 10;
	ctrl.roll.core.iLimit 	= 10;
	ctrl.yaw.core.iLimit    = 10;
	ctrl.pitch.shell.iLimit = 5;
	ctrl.roll.shell.iLimit 	= 5;
	ctrl.yaw.shell.iLimit    = 5;
	alti.iLimit = 100;
	ctrl.yaw.shell.iLimit 	= 30;
	
	//积分清零
	clr_int();
	
}

extern u8 f_hold;

void clr_int(void)
{
	//积分清零
	ctrl.pitch.shell.increment = 0.0;
	ctrl.roll.shell.increment = 0.0;
	ctrl.yaw.shell.increment = 0.0;
	ctrl.pitch.core.increment = 0.0;
	ctrl.roll.core.increment = 0.0;
	ctrl.yaw.core.increment = 0.0;
	alti.increment = 0.0;
	alti.desPoint = 0.0;
	f_hold = 0;
}







