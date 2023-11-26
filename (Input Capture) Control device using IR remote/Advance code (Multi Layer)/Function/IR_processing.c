#include"IR_Processing.h"
#include"stdint.h"
extern uint8_t IR_flag;
uint16_t time_pre = 0;
uint16_t time_current = 0;
uint16_t period = 0;
uint8_t count = 0;
uint16_t bit = 0;
extern uint16_t hex_ir;
extern TIM_HandleTypeDef htim1;
extern IR_Name Sony;	
void IR_processing(){
	if(IR_flag == 0){
		time_current = IR_Read(&Sony, &htim1, TIM_CHANNEL_1);
		period = time_current - time_pre;
		time_pre = time_current;
		if(period > START_MIN_TIME && period < START_MAX_TIME){
			hex_ir = 0;
			count = 0 ;
			bit = 0;
		}
		else if(period > COMMAND) //bit 1
		{
			bit = bit | 0x01; // set bit 1
			bit = bit << 1;  // shift next bit
			count++;
		}
		else if(period < COMMAND) //bit 0
		{
			bit = bit << 1; // shift bit 0
			count++;
		}
		if(count == 11) //ghi 12 lan
		{
			hex_ir = bit;
			IR_flag = 1;
		}
	}
}
