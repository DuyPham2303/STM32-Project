#ifndef INC_LED_H_
#define INC_LED_H_
#include"stm32f1xx.h"
#define prescaler_led			799 // => timer_clk_led	= 0.1 ms
#define resolution				100 // T_period / timer_clk = 10ms / 0.1ms
#define counter_period_led      resolution - 1
typedef struct{
	TIM_HandleTypeDef* LED_TIMER;
	uint32_t LED_CHANNEL;
}Led_Name;
void Led_Init(Led_Name* Led1,TIM_HandleTypeDef* timer,uint32_t channel);
#endif /* INC_LED_H_ */
