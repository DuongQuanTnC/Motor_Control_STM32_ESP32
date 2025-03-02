#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

#define UART_BUFFER_SIZE 32  // Kích thu?c buffer nh?n d? li?u UART

volatile char uart_buffer[UART_BUFFER_SIZE];  // Buffer luu chu?i nh?n
volatile int target_speed = 0;
volatile int duty_cycle = 0; 

void USART1_IRQHandler(void) {
    USART_receiveString(USART1, (unsigned char *)uart_buffer, UART_BUFFER_SIZE);
    
    int speed, duty;
    if (sscanf((char *)uart_buffer, "<Speed:%d>,<DutyCycle:%d>", &speed, &duty) == 2) {
        target_speed = speed;
        duty_cycle = duty;
    }
    TIM1_PWMOC(5000, duty_cycle);
}




int main() {
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIO_SetMode(GPIOA, 0, GPIO_OUTPUT_MODE_50M_PP);
	GPIO_SetMode(GPIOA, 1, GPIO_OUTPUT_MODE_50M_PP);
	
	GPIOA->BSRR = (1 << 0);
	GPIOA->BRR = (1 << 1);
    
	Encoder_Init();
    Timer4_Init(); 
    USART_Init(USART1, 115200);
    TIM1_PWMOC(5000, 80);
    

    
		
		//xTaskCreate( taskFunction, taskName, stackSize, parameters, priority, taskHandle)

		// T?o các Task
    //xTaskCreate(Motor_Task, "Motor", 128, NULL, 2, NULL);
    //xTaskCreate(Encoder_Task, "Encoder", 128, NULL, 2, NULL);
    //xTaskCreate(UART_Task, "UART", 128, NULL, 1, NULL);
    //xTaskCreate(PID_Task, "PID", 128, NULL, 3, NULL);

    // B?t d?u Scheduler
    //vTaskStartScheduler();
    
	
    while (1) {		
        float speed = getSpeed();
			//motor_set_speed(100, 10000);
		}
    return 0;
}





//Configure USART1
void USART1_Config(uint32_t BAUD_RATE)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	// Initialize UART1
    USART_Init(USART1, BAUD_RATE);
	// Configure GPIO for UART
    GPIO_SetMode(GPIOA, 9, GPIO_ALT_MODE_50M_PP);
    GPIO_SetMode(GPIOA, 10, GPIO_INPUT_MODE_FLOAT);
	// UART1 Enable
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
	USART1->CR1 |= USART_CR1_RXNEIE; // B?t ng?t nh?n d? li?u
	NVIC_SetPriority(USART1_IRQn, 0); // Ð?t uu tiên ng?t
	NVIC_EnableIRQ(USART1_IRQn);      // Kích ho?t ng?t USART1
}
void clearCharArray(char* str) {
    for(int i = 0; str[i] != '\0'; i++) {
        str[i] = '\0';
    }
}

