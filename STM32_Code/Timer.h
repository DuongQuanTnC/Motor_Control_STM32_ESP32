#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h" // Device header
#include "GPIO.h"

void TIM_PWMICMInit(TIM_TypeDef* TIMx);
void TIM_OC1_PWMM1_Init(TIM_TypeDef* TIMx, uint32_t Frequency, uint32_t Duty_Cycle);
void TIM3_PWM_IC_EM_Init(void);
void TIM3_PinConfig();

void TIM1_PinConfig();
void TIM1_PWMOC(uint32_t Frequency, uint32_t Duty_Cycle);
void Timer4_Init(void);
#endif /* __TIM_H__ */