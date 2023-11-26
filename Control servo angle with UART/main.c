#include "main.h"
#include "stdio.h"
#include "string.h"
#define	SERVO_PRES		79 		//timer clk = 0.01 ms
#define SERVO_PERIOD 		2000		//T_servo / timer_clk = 20 ms / 0.01 ms
#define PULSE_0_ANGLE		50		//pulse counter = 0.5ms / 0.01ms
#define PULSE_90_ANGLE		150		//pulse counter = 1.5ms / 0.01ms
#define PULSE_180_ANGLE		250		//pulse counter = 2.5ms / 0.01ms

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;
uint8_t flag = 0;
uint8_t repeat_flag = 0;
uint8_t buffer_rx[15];
uint8_t rx_index = 0;
uint8_t data_rx = 0;
uint16_t counter_period_servo = SERVO_PERIOD - 1;
uint16_t current_position = 150; // duty_cycle
char ledon_buffer[] = "LED1=1\n";
char ledoff_buffer[] = "LED1=0\n";
char servo0_buffer[] = "SERVO=0\n";
char servo90_buffer[] = "SERVO=90\n";
char servo180_buffer[] = "SERVO=180\n";
char mode1_buffer[] = "REPEAT=ON\n";
char mode0_buffer[] = "REPEAT=OFF\n";
void get_message(){
	if(strcmp((char*)buffer_rx,ledon_buffer) == 0){
		flag = 1;
	}
	else if(strcmp((char*)buffer_rx,ledoff_buffer) == 0){
		flag = 2;
	}
	else if(strcmp((char*)buffer_rx,servo0_buffer) == 0){
		flag = 3;
	}
	else if(strcmp((char*)buffer_rx,servo90_buffer) == 0){
		flag = 4;
	}
	else if(strcmp((char*)buffer_rx,servo180_buffer) == 0){
		flag = 5;
	}
	else if(strcmp((char*)buffer_rx,mode0_buffer) == 0){
		repeat_flag = 0;
	}
	else if(strcmp((char*)buffer_rx,mode1_buffer) == 0){
		repeat_flag = 1;
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == huart1.Instance){
		buffer_rx[rx_index] = data_rx;
		rx_index++;
		if(data_rx == '\n'){
			get_message();
			if(repeat_flag == 1){
				HAL_UART_Transmit(&huart1,buffer_rx,sizeof(buffer_rx),100);
			}
			rx_index = 0;
			memset(buffer_rx,0,sizeof(buffer_rx));
		}
		HAL_UART_Receive_IT(huart,&data_rx,1); // recal this function to keep receive data
	}
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  HAL_UART_Receive_IT(&huart1,&data_rx,1); // receives 1 byte each time and call interrupt function to process data
  while (1)
  {
	  if(flag == 1){
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,1);
		  flag = 0;
	  }
	  else if(flag == 2){
		  HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_15,0);
		  flag = 0;
	  }
	  else if(flag == 3){
		  for(;current_position > PULSE_0_ANGLE;current_position--){
		  	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,current_position);
		  	HAL_Delay(10);
		  }
		  flag = 0;
	  }
	  else if(flag == 4){
		  if(current_position == PULSE_180_ANGLE){
		  	for(;current_position > PULSE_90_ANGLE;current_position--){
		  	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,current_position);
		  	HAL_Delay(10);
		  	}
		  }
		  else if(current_position == PULSE_0_ANGLE){
		  	for(;current_position < PULSE_90_ANGLE;current_position++){
		  	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,current_position);
		  	HAL_Delay(10);
		  	}
		  }
		  flag = 0;
	  }
	  else if(flag == 5){
		  for(;current_position < PULSE_180_ANGLE;current_position++){
		  	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,current_position);
		  	HAL_Delay(10);
		  }
		  flag = 0;
	  }
    }
}


  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = SERVO_PRES;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = SERVO_PERIOD;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = current_position;
