#define time_crease 5
#define prescaler 7999
uint8_t flag = 0;
uint8_t duty_cycle = 1; 
uint8_t T = 100;
uint8_t pwm = 1; //  htim2.Init.Period = pwm
int main(void){
  HAL_TIM_Base_Start_IT(&htim2);
  while (1)
  {
	 if(flag == 1){
		 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
		 HAL_Delay(time_increase);
	         duty_cycle++;
	         flag = 0;
	  }
  }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		if(duty_cycle == T){
			duty_cycle = 1;
		}
		if(pwm < duty_cycle){
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1);
		}
		else{
			pwm = 1;
			flag = 1;
		}
		pwm++;
	}
}