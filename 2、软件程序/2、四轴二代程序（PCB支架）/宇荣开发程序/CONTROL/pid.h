#include "stm32f10x.h"

/*------------------------
	单环PID结构体
------------------------*/
typedef struct
{
	float desPoint;		//目标值
	float atcPoint;		//实际值
	float Error;				//误差
	float PreError;			//上一个误差
	float PrePreError;  //上上一个误差
	float deriv;				//误差增量
	float increment;		//误差积累值
	float iLimit;				//积分最大值
	float Kp;					//比例常数
	float Ki;					//积分常数
	float Kd;					//微分常数
	float kp_out;
	float ki_out;
	float kd_out;
  float pid_out;		//输出
	
}PIDTypdDef;


struct _tache{
    PIDTypdDef shell;			//外环
    PIDTypdDef core;			//内环
          };
typedef struct
{
      struct _tache pitch;    
	    struct _tache roll;  
	    struct _tache yaw;   
	
}CasPIDTypdDef;

extern CasPIDTypdDef ctrl;

typedef struct {
	int M1;
	int M2;
	int M3;
	int M4;
}_PWM;


extern _PWM Motor_PWM;
extern PIDTypdDef alti;
extern int Throttle;
extern int basethrottle;
extern int extrathrottle; 
extern float z_speed;


void Cascade_PID_Calc(float pit, float rol, float yaw);
void detecZspeed(void);
float Altitude_PID( float alti );












