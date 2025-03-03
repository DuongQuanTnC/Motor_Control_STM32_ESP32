#ifndef __MOTORTASK_H__
#define __MOTORTASK_H__

#include "stm32f10x.h"




void motor_set_speed(float target_speed, int fre);
float Motor_PID_Controller(float target_speed);
void PID_Task(void *pvParameters);


#endif /* __MAIN_H__ */