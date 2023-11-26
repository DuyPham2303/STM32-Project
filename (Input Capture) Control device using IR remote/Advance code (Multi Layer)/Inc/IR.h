#ifndef INC_IR_H_
#define INC_IR_H_
#define START_MIN_TIME	29
#define START_MAX_TIME	32
#define COMMAND			15
#define IR_PRESCALER	799
#include"stm32f1xx_hal.h"
#include"main.h"
typedef struct{
	TIM_HandleTypeDef* TIMER;
	uint32_t CHANNEL;
}IR_Name;
void IR_Init(IR_Name* IR_type, TIM_HandleTypeDef* TIMER,uint32_t CHANNEL);
uint16_t IR_Read(IR_Name* IR_type, TIM_HandleTypeDef* TIMER,uint32_t CHANNEL);
#endif /* INC_IR_H_ */
