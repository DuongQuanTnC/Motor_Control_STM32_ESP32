#include "Timer.h"
#include "motor_task.h"


float Kp = 1.0, Ki = 0.1, Kd = 0.05;
float prev_error = 0, integral = 0;

float Motor_PID_Controller(float target_speed) {
	
	float encoder_speed = Encoder_GetSpeed();
	
    float error = target_speed - encoder_speed;

    // PID Control
    integral += error;
    float derivative = error - prev_error;
    float output = Kp * error + Ki * integral + Kd * derivative;
    prev_error = error;
	
	return output;
}

void motor_set_speed(float target_speed, int fre) {
	
    float pid_output = Motor_PID_Controller(target_speed);

    // Gi?i h?n tín hi?u PWM trong kho?ng 0 - 100%
    if (pid_output > 1000) pid_output = 1000;
    if (pid_output < 0) pid_output = 0;

	TIM1_PWMOC(fre, pid_output);
}

