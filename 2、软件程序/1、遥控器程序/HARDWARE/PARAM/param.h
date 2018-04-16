#include "stm32f10x.h"

typedef struct {
	float kp;
	float ki;
	float kd;
}_pid;

struct _tache
{ 
	_pid shell;
	_pid core;
};
typedef struct
{
      struct _tache pitch;    
	    struct _tache roll;  
	    struct _tache yaw;   
	
}CasPIDTypdDef;

extern CasPIDTypdDef ctrl;
extern int Throttle;
extern float pitch_core_desPoint;
extern float roll_core_desPoint;


void paramLoad(void);
void PID_inc(u8 nextMode);
void PID_dec(u8 nextMode);
void PID_inc_point(u8 nextMode);
void PID_dec_point(u8 nextMode);










