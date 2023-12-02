#include "main.h"
#include"string.h"
#include"stdio.h"
#include"stdlib.h"

char buff_rx[35];
char* led_key = "led 1 status:";
uint8_t led1_status,led2_status;
char led1_buff[1],led2_buff[1];
uint8_t  data_rx;
uint8_t rx_index = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1){
		buff_rx[rx_index] = data_rx;
		rx_index++;
		if(data_rx == '*'){
			rx_index = 0;
			char* key_pos = strstr(buff_rx,led_key);
			key_pos = key_pos + strlen(led_key);
			do{
				led1_buff[rx_index] = key_pos[rx_index];
				rx_index++;
			}while(key_pos[rx_index] != '^');
			rx_index = 0;
			key_pos = key_pos + 15;
			do{
				led2_buff[rx_index] = key_pos[rx_index];
				rx_index++;
			}while(key_pos[rx_index] != '*');
			led1_status = atoi(led1_buff);
			led2_status = atoi(led2_buff);
			rx_index = 0;
		}
	}
	HAL_UART_Receive_IT(&huart1, &data_rx, sizeof(data_rx));
}
int main(void)
{

  HAL_UART_Receive_IT(&huart1,&data_rx,sizeof(data_rx));

  while (1)
  {
	 if(led1_status == 1) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,1);
	 else 				  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,0);

	 if(led2_status == 1) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,1);
	 else 				  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,0);

  }

}

