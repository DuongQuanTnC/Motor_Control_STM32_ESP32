#include "encoder_task.h"
#include "Timer.h"

#define PPR 100  // Ví d? encoder có 1000 xung/vòng

volatile int32_t encoder_count = 0;
volatile float encoder_speed = 0;
volatile int32_t last_encoder_count = 0;

void Encoder_Init(void) {
	TIM3_PinConfig();
	TIM3_PWM_IC_EM_Init();
}

int32_t Encoder_GetCount(void) {
    return TIM3->CNT;
}

float Encoder_GetSpeed(void) {
    return encoder_speed;
}

float getSpeed() {
        int32_t current_count = Encoder_GetCount();
        int32_t delta_count = current_count - last_encoder_count;

        //xu ly overflow
				if (delta_count > 32767) {  // Quay ngu?c và b? underflow
            delta_count += -0xFFFF;
        } else if (delta_count < -32767) {  // Quay xuôi và b? overflow
            delta_count += 0xFFFF;
        }

        // Tính t?c d? (s? xung do du?c trong 100ms => nhân v?i 10 d? ra s? xung/s)
        float speed = (delta_count * 10 * 60) / PPR;
        last_encoder_count = current_count;
        
        return speed;
        
 }

// Ng?t Timer4 d? do t?c d? m?i 100ms
void TIM4_IRQHandler(void) {
	
    if (TIM4->SR & (1 << 0)) {
        TIM4->SR &= ~(1 << 0);
        
        int32_t current_count = Encoder_GetCount();
        int32_t delta_count = current_count - last_encoder_count;

        //xu ly overflow
				if (delta_count > 32767) {  // Quay ngu?c và b? underflow
            delta_count += -0xFFFF;
        } else if (delta_count < -32767) {  // Quay xuôi và b? overflow
            delta_count += 0xFFFF;
        }

        // Tính t?c d? (s? xung do du?c trong 100ms => nhân v?i 10 d? ra s? xung/s)
        encoder_speed = (delta_count * 10 * 60) / PPR;
        last_encoder_count = current_count;
    }
}

