#include "control.h"
#include "param.h"
#include "pid.h"
#include "altitude.h"

u8 fly_allow=0;	//开始不运行
u8 f_hold = 0;

void NRF24L01_MsgAnalysis(u8* msg)
{
	if( msg[0]=='A' && msg[1]=='0' )
	{
//		if(msg[2]=='M') f_hold = 0;
//		else if(msg[2]=='A') f_hold = 1;
		memcpy(&basethrottle,&msg[4], sizeof(basethrottle));
		memcpy(&ctrl.pitch.shell.desPoint,&msg[8], sizeof(ctrl.pitch.shell.desPoint));
		memcpy(&ctrl.roll.shell.desPoint,&msg[12], sizeof(ctrl.roll.shell.desPoint));
		memcpy(&ctrl.yaw.shell.desPoint,&msg[16], sizeof(ctrl.yaw.shell.desPoint));
		alti.desPoint = altitude;
		LED_on();
		fly_allow=1;	
	}
	if( msg[0]=='A' && msg[1]=='1' )
	{
		LED_off();
	}
	if( msg[0]=='A' && msg[1]=='2' )
	{
		LED_rev();
	}
	if( msg[0]=='A' && msg[1]=='3' )
	{
		LED_rev();
	}
	
	if( msg[0]=='D' && msg[1]=='0' )
	{
		LED_off();
		clr_int();
		fly_allow= 0;
	}
	else if( msg[0]=='D' && msg[1]=='1' && msg[26]=='E' )
	{
		LED_on();
		memcpy(&ctrl.pitch.core.Kp,&msg[2], sizeof(ctrl.pitch.core.Kp));	
		memcpy(&ctrl.pitch.core.Ki,&msg[6], sizeof(ctrl.pitch.core.Ki));
		memcpy(&ctrl.pitch.core.Kd,&msg[10], sizeof(ctrl.pitch.core.Kd));
		memcpy(&ctrl.roll.core.Kp,&msg[14], sizeof(ctrl.roll.core.Kp));	
		memcpy(&ctrl.roll.core.Ki,&msg[18], sizeof(ctrl.roll.core.Ki));
		memcpy(&ctrl.roll.core.Kd,&msg[22], sizeof(ctrl.roll.core.Kd));	
		clr_int();
		fly_allow= 1;	
		
//		if(ctrl.pitch.core.Kp>10.0)LED_on();
//		if(ctrl.pitch.core.Ki>10.0)LED_on();
//		if(ctrl.pitch.core.Kd>10.0)LED_on();
//		if(ctrl.roll.core.Kp>5.0)LED_on();
//		if(ctrl.roll.core.Ki>5.0)LED_on();
//		if(ctrl.roll.core.Kd>5.0)LED_on();	
    	
	}
	else if( msg[0]=='D' && msg[1]=='2' && msg[26]=='E' )
	{
		LED_on();
		memcpy(&ctrl.pitch.shell.Kp,&msg[2], sizeof(ctrl.pitch.shell.Kp));	
		memcpy(&ctrl.pitch.shell.Ki,&msg[6], sizeof(ctrl.pitch.shell.Ki));
		memcpy(&ctrl.pitch.shell.Kd,&msg[10], sizeof(ctrl.pitch.shell.Kd));
		memcpy(&ctrl.roll.shell.Kp,&msg[14], sizeof(ctrl.roll.shell.Kp));	
		memcpy(&ctrl.roll.shell.Ki,&msg[18], sizeof(ctrl.roll.shell.Ki));
		memcpy(&ctrl.roll.shell.Kd,&msg[22], sizeof(ctrl.roll.shell.Kd));		
		clr_int();
		fly_allow= 1;			
    
	}
	else if( msg[0]=='D' && msg[1]=='3')
	{
		LED_on();
		memcpy(&basethrottle,&msg[2], sizeof(basethrottle));
		memcpy(&alti.Kp,&msg[6], sizeof(alti.Kp));	
		memcpy(&alti.Kd,&msg[10], sizeof(alti.Kd));	
		clr_int();
		fly_allow= 1;	
	}
	else if( msg[0]=='D' && msg[1]=='4')
	{
		LED_on();
		memcpy(&ctrl.yaw.core.Kp,&msg[2], sizeof(ctrl.yaw.core.Kp));
		memcpy(&ctrl.yaw.core.Ki,&msg[6], sizeof(ctrl.yaw.core.Ki));	
		memcpy(&ctrl.yaw.core.Kd,&msg[10], sizeof(ctrl.yaw.core.Kd));	
		memcpy(&ctrl.yaw.shell.Kp,&msg[14], sizeof(ctrl.yaw.shell.Kp));
		memcpy(&ctrl.yaw.shell.Ki,&msg[18], sizeof(ctrl.yaw.shell.Ki));	
		memcpy(&ctrl.yaw.shell.Kd,&msg[22], sizeof(ctrl.yaw.shell.Kd));	
		clr_int();
		fly_allow= 1;	
		
	}
}




















