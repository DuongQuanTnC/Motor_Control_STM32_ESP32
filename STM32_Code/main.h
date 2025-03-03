#ifndef __MAIN_H__
#define __MAIN_H__

#include "GPIO.h"
#include "RCC.h"
#include "USART.h"
#include "Timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include <stdio.h>

SemaphoreHandle_t speedMutex;

// Hàng d?i truy?n d? li?u gi?a các task
//extern QueueHandle_t uartQueue;
QueueHandle_t target_speed_queue;
QueueHandle_t encoder_speed_queue;


//Configure USART1
void USART1_Config(uint32_t BAUD_RATE);

#endif /* __MAIN_H__ */