#ifndef INC_SERVO_H_
#define INC_SERVO_H_
#include"stm32f1xx.h"
#define prescaler_servo			79   // timer_clk_servo = 0.01 ms
#define SERVO_PERIOD 			2000 // T_servo / timer_clk_servo = 20ms / 0.01 ms
#define PULSE_NEG90_ANGLE		50    // pulse at -90 is 0.5 ms = 50 * 0.01
#define PULSE_0_ANGLE			150  // pulse at 0 is 1.5 ms = 150 * 0.01
#define PULSE_POS90_ANGLE		250	 //	pulse at 90 is 2.5 ms = 250 * 0.01
#define counter_period_servo 	SERVO_PERIOD - 1
typedef struct{
	TIM_HandleTypeDef* SERVO_TIMER;
	uint32_t SERVO_CHANNEL;
}Servo_Name;
void Servo_Init(Servo_Name* servo1,TIM_HandleTypeDef* timer,uint32_t channel);
#endif /* INC_SERVO_H_ */
