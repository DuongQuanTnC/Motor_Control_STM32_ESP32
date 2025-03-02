#ifndef ENCODERTASK_H
#define ENCODERTASK_H

#include "stm32f10x.h"
#include "GPIO.h"



void Encoder_Init(void);
int32_t Encoder_GetCount(void);
float Encoder_GetSpeed(void);
float getSpeed();
#endif
