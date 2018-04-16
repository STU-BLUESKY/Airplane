#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED0 PBout(5)// PB5
#define LED1 PBout(12)// PB12
#define LED2 PBout(13)// PB14
#define LED3 PBout(14)// PB13
#define LED4 PBout(15)// PB15

extern void Init_LEDpin(void);
extern void LED_Flash(u8 type, u16 on_time, u16 off_time);

#endif


