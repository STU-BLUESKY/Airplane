#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************

**************************************************************************/
struct _pid{
        float kp;
			  float ki;
	      float kd;
	      float increment;
	      float increment_max;
	      float kp_out;
			  float ki_out;
	      float kd_out;
	      float pid_out;
          };

struct _tache{
    struct _pid shell;
    struct _pid core;	
          };
	

struct _ctrl{
		      u8  ctrlRate;
      struct _tache pitch;    
	    struct _tache roll;  
	    struct _tache yaw;   
            };

extern struct _ctrl ctrl;
/////////////////////////////
void TIM3_IRQHandler(void) ;
void CONTROL(float rol, float pit, float yaw);

#endif
