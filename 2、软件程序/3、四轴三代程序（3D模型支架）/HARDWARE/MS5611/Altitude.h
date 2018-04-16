#ifndef ALT_H
#define ALT_H

#include "MS5611.h"
#include "stm32f10x_it.h"

typedef struct NAV_tt
{
    float x;
    float y;
    float z;
    float vx;
    float vy;
    float vz;
    float ax;
    float ay;
    float az;
} nav_t;


#define ALT_THREAD_PRD  5000	//us . store error when sensor updates, but correct on each time step to avoid jumps in estimated value 

extern nav_t nav;
extern float z_est[3];	// estimate z Vz  Az
extern uint8_t landed;
extern float dt;
extern uint32_t t_pre;
extern float z_NED_offset;
	
static void inertial_filter_predict(float dt, float x[3], float acc);
static void inertial_filter_correct(float e, float dt, float x[3], int i, float w);

void AltitudeCombineThread(void);

#endif
