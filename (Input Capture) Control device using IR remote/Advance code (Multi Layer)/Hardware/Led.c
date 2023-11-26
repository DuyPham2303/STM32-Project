#include <Led.h>
void Led_Init(Led_Name* Led1,TIM_HandleTypeDef* timer,uint32_t channel){
	Led1->LED_CHANNEL = channel;
	Led1->LED_TIMER = timer;
	HAL_TIM_PWM_Start(Led1->LED_TIMER,Led1->LED_CHANNEL);
}

