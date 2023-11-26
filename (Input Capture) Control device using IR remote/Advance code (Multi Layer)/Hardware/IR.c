#include"IR.h"
void IR_Init(IR_Name* IR_type, TIM_HandleTypeDef* TIMER,uint32_t CHANNEL){
	IR_type->TIMER = TIMER;
	IR_type->CHANNEL = CHANNEL;
	HAL_TIM_IC_Start_IT(IR_type->TIMER,IR_type->CHANNEL);
}
uint16_t IR_Read(IR_Name* IR_type, TIM_HandleTypeDef* TIMER,uint32_t CHANNEL){
	uint16_t time_current = 0;
	time_current = HAL_TIM_ReadCapturedValue(IR_type->TIMER,IR_type->CHANNEL);
	return time_current;
}
