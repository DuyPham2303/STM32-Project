#include"control_device.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern uint16_t led_duty;
extern uint16_t servo_duty;
uint8_t duty_cycle_change = 5;
void Increase_Led_brightness(){
	if(led_duty <= counter_period_led ){
		led_duty = led_duty + duty_cycle_change;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,led_duty);
	}
}
void Decrease_Led_brightness(){
	if(led_duty > 0){
		led_duty = led_duty - duty_cycle_change;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,led_duty);
	}
}
void Increase_Servo_Angle(){
	if(servo_duty < PULSE_POS90_ANGLE){
		servo_duty = servo_duty + duty_cycle_change;
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,servo_duty);
	}
}
void Decrease_Servo_Angle(){
	if(servo_duty > PULSE_NEG90_ANGLE){
		servo_duty = servo_duty - duty_cycle_change;
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,servo_duty);
	}
}
void Limit_Angle(){
	if(servo_duty > PULSE_POS90_ANGLE){
		servo_duty = 250; //max angle -> 180
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,servo_duty);
	}
	else if(servo_duty < PULSE_NEG90_ANGLE){
		servo_duty = 50; // min angle -> 0
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,servo_duty);
	}
}

