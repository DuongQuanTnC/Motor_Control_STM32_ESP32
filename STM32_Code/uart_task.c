#include "uart_task.h"
#include "FreeRTOS.h"
#include "USART.h"
#include "timers.h"

#include "queue.h"
#include "semphr.h"

extern QueueHandle_t target_speed_queue;
extern SemaphoreHandle_t speedMutex;

extern volatile int32_t actual_speed;

void UART_Recieve_Task(void *pvParameters) {
    volatile char rx_buffer[32];
    int16_t target_speed = 0;
    while (1) {
        // Nh?n d? li?u t? UART
        USART_receiveString(USART1, (unsigned char *) rx_buffer, sizeof(rx_buffer));
        int speed = 0;
        if (sscanf((char*)rx_buffer, "<Speed:%d>", &speed) == 1) {
            xSemaphoreTake(speedMutex, portMAX_DELAY);
            target_speed = speed;
            xSemaphoreGive(speedMutex);
            xQueueSend(target_speed_queue, (const void *)&target_speed, portMAX_DELAY);
        }
        
        vTaskDelay(pdMS_TO_TICKS(10)); // Tránh chi?m CPU
    }
}

void UART_Transmit_Task(void *pvParameters) {
    char tx_buffer[32];
    
    while (1) {
        xSemaphoreTake(speedMutex, portMAX_DELAY);
        int32_t current_speed = actual_speed;
        xSemaphoreGive(speedMutex);
        
        sprintf(tx_buffer, "<Speed:%d>", current_speed);
        USART_str(USART1, (unsigned char *)tx_buffer);
        
        vTaskDelay(pdMS_TO_TICKS(200)); // G?i m?i 200ms
    }
}

