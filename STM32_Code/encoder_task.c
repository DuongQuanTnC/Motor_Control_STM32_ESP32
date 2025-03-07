#include "encoder_task.h"
#include "FreeRTOS.h"
#include "Timer.h"
#include "queue.h"
#include "semphr.h"


extern QueueHandle_t encoder_speed_queue;
extern SemaphoreHandle_t speedMutex;

volatile int32_t actual_speed;

#define PPR 100  // V� d? encoder c� 1000 xung/v�ng

volatile int32_t encoder_count = 0;
volatile int32_t last_encoder_count = 0;

void Encoder_Init(void) {
	TIM3_PinConfig();
	TIM3_PWM_IC_EM_Init();
}

int32_t Encoder_GetCount(void) {
    return TIM3->CNT;
}

float get_actual_speed() {
    return actual_speed;
}
void Encoder_Task(void *pvParameters) {
    
    while (1) {
        int32_t encoder_count = Encoder_GetCount();  // H�m d?c encoder
        xSemaphoreTake(speedMutex, portMAX_DELAY);
        actual_speed = get_Encoder_Speed(encoder_count);
        xSemaphoreGive(speedMutex);
        xQueueSend(encoder_speed_queue, (void*)&actual_speed, portMAX_DELAY);
        
        vTaskDelay(pdMS_TO_TICKS(100));  // C?p nh?t m?i 100ms
    }
}

float get_Encoder_Speed(int32_t encoder_count) {
        
        int32_t delta_count = encoder_count - last_encoder_count;

        //xu ly overflow
		if (delta_count > 32767) {  // Quay ngu?c v� b? underflow
            delta_count += -0xFFFF;
        } else if (delta_count < -32767) {  // Quay xu�i v� b? overflow
            delta_count += 0xFFFF;
        }

        // T�nh t?c d? (s? xung do du?c trong 100ms => nh�n v?i 10 d? ra s? xung/s)
        float speed = (delta_count * 10 * 60) / PPR;
        last_encoder_count = encoder_count;
        
        return speed;
 }

// Ng?t Timer4 d? do t?c d? m?i 100ms
/*void TIM4_IRQHandler(void) {
	
    if (TIM4->SR & (1 << 0)) {
        TIM4->SR &= ~(1 << 0);
        
        int32_t current_count = Encoder_GetCount();
        int32_t delta_count = current_count - last_encoder_count;

        //xu ly overflow
				if (delta_count > 32767) {  // Quay ngu?c v� b? underflow
            delta_count += -0xFFFF;
        } else if (delta_count < -32767) {  // Quay xu�i v� b? overflow
            delta_count += 0xFFFF;
        }

        // T�nh t?c d? (s? xung do du?c trong 100ms => nh�n v?i 10 d? ra s? xung/s)
        encoder_speed = (delta_count * 10 * 60) / PPR;
        last_encoder_count = current_count;
    }
}*/

