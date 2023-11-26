prescaler = 7999
counter period = 99
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM1){
		if(count < 15){
			if(count <= 5){
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,1);
			}
			else{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,0);
			}
			count++;
		}
		else count = 1;
	}
}