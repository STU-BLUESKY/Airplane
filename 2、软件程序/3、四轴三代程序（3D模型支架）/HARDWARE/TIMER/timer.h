#include "stm32f10x.h"

extern uint8_t  loop200HzFlag, loop100HzFlag, loop50HzFlag, loop10HzFlag;
extern volatile uint16_t loop200HzCnt, loop100HzCnt, loop50HzCnt, loop10HzCnt;

void TIM3_Config(unsigned short int Period,unsigned short int Prescaler);
void TIM2_Config(unsigned short int Period,unsigned short int Prescaler);

