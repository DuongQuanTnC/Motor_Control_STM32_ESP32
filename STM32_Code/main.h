#ifndef __MAIN_H__
#define __MAIN_H__

#include "GPIO.h"
#include "RCC.h"
#include "USART.h"
#include "Timer.h"
#include "encoder_task.h"
#include "motor_task.h"
#include "uart_task.h"

#include <stdio.h>




//Configure USART1
void USART1_Config(uint32_t BAUD_RATE);

#endif /* __MAIN_H__ */