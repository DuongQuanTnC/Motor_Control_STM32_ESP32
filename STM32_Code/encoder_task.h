#ifndef ENCODERTASK_H
#define ENCODERTASK_H

#include "stm32f10x.h"




void Encoder_Init(void);
int32_t Encoder_GetCount(void);
float Encoder_GetSpeed(void);
float get_actual_speed();
float get_Encoder_Speed(int32_t encoder_count);
void Encoder_Task(void *pvParameters);

#endif
