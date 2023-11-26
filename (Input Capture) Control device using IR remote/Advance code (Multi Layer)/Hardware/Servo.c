#include"Servo.h"
void Servo_Init(Servo_Name* servo1,TIM_HandleTypeDef* timer,uint32_t channel){
	servo1->SERVO_CHANNEL = channel;
	servo1->SERVO_TIMER = timer;
	HAL_TIM_PWM_Start(servo1->SERVO_TIMER,servo1->SERVO_CHANNEL);
}

