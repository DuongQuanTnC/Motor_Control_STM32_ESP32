#ifndef __MOTORTASK_H__
#define __MOTORTASK_H__

#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "USART.h"
#include <stdio.h>
#include "encoder_task.h"

void motor_set_speed(float target_speed, int fre);
float Motor_PID_Controller(float target_speed);


#endif /* __MAIN_H__ */