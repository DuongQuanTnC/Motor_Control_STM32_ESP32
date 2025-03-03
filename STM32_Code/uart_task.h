#ifndef UARTTASK_H
#define UARTTASK_H

#include "stm32f10x.h"


#include <stdio.h>

void UART_Recieve_Task(void *pvParameters);
void UART_Transmit_Task(void *pvParameters);
#endif
