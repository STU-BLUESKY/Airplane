#include "stm32f10x.h"

/*------------------------
	����PID�ṹ��
------------------------*/
typedef struct
{
	float desPoint;		//Ŀ��ֵ
	float atcPoint;		//ʵ��ֵ
	float Error;				//���
	float PreError;			//��һ�����
	float PrePreError;  //����һ�����
	float deriv;				//�������
	float increment;		//������ֵ
	float iLimit;				//�������ֵ
	float Kp;					//��������
	float Ki;					//���ֳ���
	float Kd;					//΢�ֳ���
	float kp_out;
	float ki_out;
	float kd_out;
  float pid_out;		//���
	
}PIDTypdDef;


struct _tache{
    PIDTypdDef shell;			//�⻷
    PIDTypdDef core;			//�ڻ�
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












