#include "param.h"


CasPIDTypdDef ctrl;
int Throttle = 0;	
float pitch_core_desPoint;
float roll_core_desPoint;

void paramLoad(void)
{
	//内环参数
	ctrl.pitch.core.kp = 0.0;			//1.3			
	ctrl.pitch.core.kd = 0.0;			//2.4
	
	ctrl.roll.core.kp = 0.0;				//1.5	
	ctrl.roll.core.kd = 0.0;				//2.0
	
	ctrl.yaw.core.kp = 0.0;					
	ctrl.yaw.core.kd = 0.0;
	
	//外环参数
	ctrl.pitch.shell.kp = 0.0;		//1.2
	ctrl.pitch.shell.ki = 0.0;		
	ctrl.pitch.shell.kd = 0.0;    	
	ctrl.roll.shell.kp = 0.0;		  //1.0
	ctrl.roll.shell.ki = 0.0;			
	ctrl.roll.shell.kd = 0.0;			
	
	ctrl.yaw.shell.kp = 0.0;						
	ctrl.yaw.shell.ki = 0.0;
	ctrl.yaw.shell.kd = 0.0;
	
	//目标值
	pitch_core_desPoint = 0.0;
	roll_core_desPoint =  0.0;
}


//PID自增1
void PID_inc(u8 nextMode)
{
	switch(nextMode)
	{
		case 1: Throttle += 100; break;
		case 2: pitch_core_desPoint+=10.00; break;
		case 3: roll_core_desPoint +=10.00; break;
		case 4: ctrl.pitch.core.kp++; break;
		case 5: ctrl.pitch.core.ki++; break;
		case 6: ctrl.pitch.core.kd++; break;
		case 7: ctrl.roll.core.kp++; break;
		case 8: ctrl.roll.core.ki++; break;
		case 9: ctrl.roll.core.kd++; break;
		case 10: ctrl.pitch.shell.kp++; break;
		case 11: ctrl.pitch.shell.ki++; break;
		case 12: ctrl.pitch.shell.kd++; break;
		case 13: ctrl.roll.shell.kp++; break;
		case 14: ctrl.roll.shell.ki++; break;
		case 15: ctrl.roll.shell.kd++; break;	
		case 16: ctrl.yaw.core.kp++; break;
		case 17: ctrl.yaw.core.ki++; break;
		case 18: ctrl.yaw.core.kd++; break;	
		case 19: ctrl.yaw.shell.kp++; break;
		case 20: ctrl.yaw.shell.ki++; break;
		case 21: ctrl.yaw.shell.kd++; break;	
		default: break;
	}
}

//PID自减1
void PID_dec(u8 nextMode)
{
	switch(nextMode)
	{
		case 1: Throttle -= 100; if(Throttle<0) Throttle=0; break;
		case 2: pitch_core_desPoint-=10.000; if(pitch_core_desPoint<0) pitch_core_desPoint=0; break;
		case 3: roll_core_desPoint -=10.000; if(roll_core_desPoint<0)  roll_core_desPoint=0; break;
		case 4: ctrl.pitch.core.kp--; if(ctrl.pitch.core.kp<0) ctrl.pitch.core.kp=0; break;
		case 5: ctrl.pitch.core.ki--; if(ctrl.pitch.core.ki<0) ctrl.pitch.core.ki=0;break;
		case 6: ctrl.pitch.core.kd--; if(ctrl.pitch.core.kd<0) ctrl.pitch.core.kd=0;break;
		case 7: ctrl.roll.core.kp--; if(ctrl.roll.core.kp<0) ctrl.roll.core.kp=0;break;
		case 8: ctrl.roll.core.ki--; if(ctrl.roll.core.ki<0) ctrl.roll.core.ki=0;break;
		case 9: ctrl.roll.core.kd--; if(ctrl.roll.core.kd<0) ctrl.roll.core.kd=0;break;
		case 10: ctrl.pitch.shell.kp--; if(ctrl.pitch.shell.kp<0) ctrl.pitch.shell.kp=0;break;
		case 11: ctrl.pitch.shell.ki--; if(ctrl.pitch.shell.ki<0) ctrl.pitch.shell.ki=0;break;
		case 12: ctrl.pitch.shell.kd--; if(ctrl.pitch.shell.kd<0) ctrl.pitch.shell.kd=0;break;
		case 13: ctrl.roll.shell.kp--; if(ctrl.roll.shell.kp<0) ctrl.roll.shell.kp=0;break;
		case 14: ctrl.roll.shell.ki--; if(ctrl.roll.shell.ki<0) ctrl.roll.shell.ki=0;break;
		case 15: ctrl.roll.shell.kd--; if(ctrl.roll.shell.kd<0) ctrl.roll.shell.kd=0;break;
		case 16: ctrl.yaw.core.kp--; break; //if(ctrl.yaw.core.kp<0) ctrl.yaw.core.kp=0;
		case 17: ctrl.yaw.core.ki--; break; //if(ctrl.yaw.core.ki<0) ctrl.yaw.core.ki=0;
		case 18: ctrl.yaw.core.kd--; break; //if(ctrl.yaw.core.kd<0) ctrl.yaw.core.kd=0;
		case 19: ctrl.yaw.shell.kp--; break; //if(ctrl.yaw.shell.kp<0) ctrl.yaw.shell.kp=0;
		case 20: ctrl.yaw.shell.ki--; break; //if(ctrl.yaw.shell.ki<0) ctrl.yaw.shell.ki=0;
		case 21: ctrl.yaw.shell.kd--; break; //if(ctrl.yaw.shell.kd<0) ctrl.yaw.shell.kd=0;
		default: break;
	}
}

//PID自增0.1
void PID_inc_point(u8 nextMode)
{
	switch(nextMode)
	{
	  case 1: Throttle+=10; break;
		case 2: pitch_core_desPoint++; break;
		case 3: roll_core_desPoint ++; break;
		case 4: ctrl.pitch.core.kp+=0.100; break;
		case 5: ctrl.pitch.core.ki+=0.100; break;
		case 6: ctrl.pitch.core.kd+=0.100; break;
		case 7: ctrl.roll.core.kp+=0.100; break;
		case 8: ctrl.roll.core.ki+=0.100; break;
		case 9: ctrl.roll.core.kd+=0.100; break;
		case 10: ctrl.pitch.shell.kp+=0.100; break;
		case 11: ctrl.pitch.shell.ki+=0.100; break;
		case 12: ctrl.pitch.shell.kd+=0.100; break;
		case 13: ctrl.roll.shell.kp+=0.100; break;
		case 14: ctrl.roll.shell.ki+=0.100; break;
		case 15: ctrl.roll.shell.kd+=0.100; break;	
		case 16: ctrl.yaw.core.kp+=0.100; break;
		case 17: ctrl.yaw.core.ki+=0.100; break;
		case 18: ctrl.yaw.core.kd+=0.100; break;	
		case 19: ctrl.yaw.shell.kp+=0.100; break;
		case 20: ctrl.yaw.shell.ki+=0.100; break;
		case 21: ctrl.yaw.shell.kd+=0.100; break;	
		default: break;
	}
}


//PID自减0.1
void PID_dec_point(u8 nextMode)
{
	switch(nextMode)
	{
		case 1: Throttle-=10; if(Throttle<0) Throttle=0; break;
		case 2: pitch_core_desPoint--; if(pitch_core_desPoint<0) pitch_core_desPoint=0; break;
		case 3: roll_core_desPoint --; if(roll_core_desPoint<0)  roll_core_desPoint=0; break;
		case 4: ctrl.pitch.core.kp-=0.100; if(ctrl.pitch.core.kp<0) ctrl.pitch.core.kp=0; break;
		case 5: ctrl.pitch.core.ki-=0.100; if(ctrl.pitch.core.ki<0) ctrl.pitch.core.ki=0;break;
		case 6: ctrl.pitch.core.kd-=0.100; if(ctrl.pitch.core.kd<0) ctrl.pitch.core.kd=0;break;
		case 7: ctrl.roll.core.kp-=0.100; if(ctrl.roll.core.kp<0) ctrl.roll.core.kp=0;break;
		case 8: ctrl.roll.core.ki-=0.100; if(ctrl.roll.core.ki<0) ctrl.roll.core.ki=0;break;
		case 9: ctrl.roll.core.kd-=0.100; if(ctrl.roll.core.kd<0) ctrl.roll.core.kd=0;break;
		case 10: ctrl.pitch.shell.kp-=0.100; if(ctrl.pitch.shell.kp<0) ctrl.pitch.shell.kp=0;break;
		case 11: ctrl.pitch.shell.ki-=0.100; if(ctrl.pitch.shell.ki<0) ctrl.pitch.shell.ki=0;break;
		case 12: ctrl.pitch.shell.kd-=0.100; if(ctrl.pitch.shell.kd<0) ctrl.pitch.shell.kd=0;break;
		case 13: ctrl.roll.shell.kp-=0.100; if(ctrl.roll.shell.kp<0) ctrl.roll.shell.kp=0;break;
		case 14: ctrl.roll.shell.ki-=0.100; if(ctrl.roll.shell.ki<0) ctrl.roll.shell.ki=0;break;
		case 15: ctrl.roll.shell.kd-=0.100; if(ctrl.roll.shell.kd<0) ctrl.roll.shell.kd=0;break;
		case 16: ctrl.yaw.core.kp-=0.100; break; //if(ctrl.yaw.core.kp<0) ctrl.yaw.core.kp=0;
		case 17: ctrl.yaw.core.ki-=0.100; break; //if(ctrl.yaw.core.ki<0) ctrl.yaw.core.ki=0;
		case 18: ctrl.yaw.core.kd-=0.100; break; //if(ctrl.yaw.core.kd<0) ctrl.yaw.core.kd=0;
		case 19: ctrl.yaw.shell.kp-=0.100; break; //if(ctrl.yaw.shell.kp<0) ctrl.yaw.shell.kp=0;
		case 20: ctrl.yaw.shell.ki-=0.100; break; //if(ctrl.yaw.shell.ki<0) ctrl.yaw.shell.ki=0;
		case 21: ctrl.yaw.shell.kd-=0.100; break; //if(ctrl.yaw.shell.kd<0) ctrl.yaw.shell.kd=0;
		default: break;
	}
	
}

