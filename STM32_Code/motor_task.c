#include "motor_task.h"
#include "FreeRTOS.h"
#include "Timer.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"


#define FREQ 10000

extern QueueHandle_t target_speed_queue;
extern QueueHandle_t encoder_speed_queue;

float Kp = 1.0, Ki = 0.1, Kd = 0.05;
float prev_error = 0, integral = 0;
volatile uint8_t duty_cycle;

void PID_Task(void *pvParameters) {
    int32_t target_speed = 0;
    int32_t encoder_speed = 0;
    
    while (1) {
        // Nh?n t?c d? mong mu?n t? UART Task
        xQueueReceive(target_speed_queue, (void *)&target_speed, portMAX_DELAY);

        // Nh?n t?c d? th?c t? t? Encoder Task
        xQueueReceive(encoder_speed_queue, (void *)&encoder_speed, portMAX_DELAY);

        // T�nh to�n PID
        float error = target_speed - encoder_speed;
        integral += error;
        float derivative = error - prev_error;
        prev_error = error;

        // �i?u ch?nh duty cycle d?a tr�n PID
        duty_cycle = (uint8_t)(Kp * error + Ki * integral + Kd * derivative);
        if (duty_cycle > 100) duty_cycle = 100;  // Gi?i h?n PWM max
        if (duty_cycle < 0) duty_cycle = 0;      // Gi?i h?n PWM min
        
        TIM1_PWMOC(FREQ, duty_cycle);

        // Delay 100ms d? PID ch?y li�n t?c
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

