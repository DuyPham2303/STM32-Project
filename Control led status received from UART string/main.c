/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define prescaler 	799
#define resolution	100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
#if defined(__GNUC__)
int _write(int fd, char * ptr, int len) {
  HAL_UART_Transmit( & huart1, (uint8_t * ) ptr, len, HAL_MAX_DELAY);
  return len;
}
#elif defined(__ICCARM__)#include "LowLevelIOInterface.h"

size_t __write(int handle,
  const unsigned char * buffer, size_t size) {
  HAL_UART_Transmit( & huart1, (uint8_t * ) buffer, size, HAL_MAX_DELAY);
  return size;
}
#elif defined(__CC_ARM)
int fputc(int ch, FILE * f) {
  HAL_UART_Transmit( & huart1, (uint8_t * ) & ch, 1, HAL_MAX_DELAY);
  return ch;
}
#endif
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t data_rx;
uint8_t rx_index = 0;
//
uint8_t data_tx1[] = "PWM MODE:";
uint8_t data_tx2[] = "PROGRAM MODE:";
uint8_t data_tx3[] = "LED STATE:";
char buffer[16];
//
char var_A[3];
char var_B[3];
char var_C[3];
//
char* pvar_A = 0;
char* pvar_B = 0;
char* pvar_C = 0;
//
int ivar_A = 0;
int ivar_B = 0;
int ivar_C = 0;
//
uint8_t temp_varA = 0;
uint8_t temp_varB = 0;
uint8_t temp_varC = 0;
//
uint8_t  ON_led1 = 0;
uint8_t  ON_led2 = 0;
uint8_t  ON_led3 = 0;
uint8_t  ON_led1_2_3 = 0;
//
uint8_t  OFF_led1 = 0;
uint8_t  OFF_led2 = 0;
uint8_t  OFF_led3 = 0;
//
volatile uint8_t flag = 0;
volatile uint8_t check = 0;
volatile uint8_t i = 0;
//
volatile uint8_t key = 0;
//
uint32_t counter_period = resolution - 1;
volatile uint16_t duty_cycle = 0;
//
uint8_t key_pwm = 0;
uint8_t key_ledstate = 0;
uint8_t key_programmode = 0;
int pwm = 0;
uint8_t block = 0;
uint8_t start_receive_flag = 0;
uint8_t count = 0;
uint8_t* address_select = 0;
/////////////////////////////
void pwm_mode();
void program_mode();
void output_ledstate();
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
uint8_t* address_var(){
	if(count == 0){
		return &data_rx;
	}
	else if(count == 1){
		return &temp_varA;
	}
	else if(count == 2){
		return &temp_varC;
	}
	else if(count == 3){
		return &temp_varB;
	}
	else return 0;
}
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  //activate pwm timer
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
  //activate receive data mode
  address_select = address_var();
  HAL_UART_Receive_IT(&huart1,address_select,1);
	  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	  while (1)
	  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		  if(key == 1){
			 // print "PWM MODE"
			 HAL_UART_Transmit(&huart1,data_tx1,sizeof(data_tx1),1000);
			 check = 3;
			 key = 2;
		  }
		  if(key == 3){
			//execute PWM MODE
			  pwm_mode();
			  if(key_pwm == 2){
				  count = 2;
				  address_select = address_var();
				  key = 4;
			  }
		  }
		  if(key == 4){
			// print "PROGRAM MODE"
			HAL_UART_Transmit(&huart1, data_tx2,sizeof(data_tx2),10);
			check = 4;
			key = 5;
		  }
		  if(key == 6){
			//Execute PROGRAM MODE
			program_mode();
			key = 7;
		  }
		  if(key == 7){
			// print 'LED STATE"
			HAL_UART_Transmit(&huart1, data_tx3,sizeof(data_tx3),10);
			count = 3;
			address_select = address_var();
			check = 5;
			key = 8;
		  }
		  if(key == 9){
			// execute LED STATE
			output_ledstate();
			count = 1;
			address_select = address_var();
			check = 2;
			key_pwm = 0;
			temp_varA = 0;
			temp_varB = 0;
			temp_varC = 0;
			key = 1;
			}
		}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = prescaler;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = counter_period;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void pwm_mode(){
	duty_cycle = 0;
	if(temp_varA == 49){
		printf("\npwm mode is activated\n");
		while(key_pwm != 2){
			if(key_pwm == 0){
				duty_cycle++;
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,duty_cycle);
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,duty_cycle);
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,duty_cycle);
				HAL_Delay(50);
				if(duty_cycle == counter_period){
					HAL_Delay(500);
					key_pwm = 1;
				}
			}
			else{
				duty_cycle--;
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,duty_cycle);
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,duty_cycle);
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,duty_cycle);
				HAL_Delay(50);
				if(duty_cycle == 0){
					HAL_Delay(500);
					key_pwm = 2;
				}
			}

		}
	}
	if(temp_varA == 48){
		printf("\npwm mode is disactivated\n");
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		key_pwm = 2;
	}
}
void program_mode(){
	printf("\n");
	switch (temp_varC) {
	case 48: // 0 -> 48
		printf("***STOP MODE IS ACTIVATED***\n");
		printf("ivar_C => 0b");
		ivar_C = 0;
		for (uint8_t bit = 0; bit < 3; bit++) {
			uint8_t bit_value = (ivar_C >> bit) & 1;
			printf("%d", bit_value);
		}
		printf("\n");
		ON_led1 = 0;
		ON_led2 = 0;
		ON_led3 = 0;
		ON_led1_2_3 = 0;
		///////////
		OFF_led1 = 0;
		OFF_led2 = 0;
		OFF_led3 = 0;
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,0); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,0); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		break;
	case 49: // 1 -> 49
		printf("***STANDARED MODE IS ACTIVATED***\n");
		printf("ivar_C => dec val:%d   binary val:0b", ivar_C);
		ivar_C = ivar_C >> 1;
		for (uint8_t bit = 0; bit < 3; bit++) {
			uint8_t bit_value = (ivar_C >> bit) & 1;
			printf("%d", bit_value);
		}
		printf("\n");
		//MODE ON
		ON_led1 = ~(ivar_B);	  			// lấy giá trị 0b xxxx x001
		ON_led2 = ivar_B & 0b010;       	// lấy giá trị 0b xxxx x010
		ON_led3 = ivar_B & 0b101; 			// lấy giá trị 0b xxxx x100
		ON_led1_2_3 = ivar_B | 1;
		//MODE Off
		OFF_led1 = ivar_B;					// 0b110
		OFF_led2 = (ivar_B | 1) & 0b101;	// 0b101
		OFF_led3 = ivar_B >> 3;				// 0b011
		break;
	default:
		break;
	}
}

void output_ledstate(){
	printf("\n");
	switch(temp_varB){
	case 49: // 001 -> 49 -> '1'
		ivar_B = 30;
		ivar_B = ON_led1;
		printf("ivar_B => dec val:%d   binary val:0b", ivar_B);
		for (uint8_t bit = 0; bit < 3; bit++) {
				uint8_t bit_value = (ivar_B >> bit) & 1;
				printf("%d", bit_value);
		}
		printf("\nLED_1 ON\nLED_2 OFF\nLED_3 OFF");
		if(ivar_B != 0){
			duty_cycle = 99;
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,1); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,0); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,duty_cycle);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		}
		printf("\n");
		break;
	case 50: // 010 -> 50 -> '2'
		ivar_B = ON_led2;
		printf("ivar_B => dec val:%d   binary val:0b", ivar_B);
		for (uint8_t bit = 0; bit < 3; bit++) {
			uint8_t bit_value = (ivar_B >> bit) & 1;
			printf("%d", bit_value);
		}
		printf("\nLED_1 OFF\nLED_2 ON\nLED_3 OFF\n");
		if(ivar_B != 0){
			duty_cycle = 99;
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,0); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,1); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,duty_cycle);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		}
		printf("\n");
		break;
	case 51: //100 -> 51 -> '3'
		ivar_B = ON_led3;
		printf("ivar_B => dec val:%d   binary val:0b", ivar_B);
		for (uint8_t bit = 0; bit < 3; bit++) {
			uint8_t bit_value = (ivar_B >> bit) & 1;
			printf("%d", bit_value);
		}
		printf("\nLED_1 OFF\nLED_2 OFF\nLED_3 ON");
		if(ivar_B != 0){
			duty_cycle = 99;
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,0); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,0); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,1);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,duty_cycle);
		}
		printf("\n");
		break;
	case 52: //110 -> 52 -> '4'
		ivar_B = OFF_led1;
		printf("ivar_B => dec val:%d   binary val:0b", ivar_B);
		for (uint8_t bit = 0; bit < 3; bit++) {
			uint8_t bit_value = (ivar_B >> bit) & 1;
			printf("%d", bit_value);
		}
		printf("\nLED_1 OFF\nLED_2 ON\nLED_3 ON");
		if(ivar_B != 0){
			duty_cycle = 99;
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,0); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,1); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,1);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,duty_cycle);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,duty_cycle);
		}
		printf("\n");
		break;
	case 53: //101 -> 53 - '5'
		ivar_B = OFF_led2;
		printf("ivar_B => dec val:%d   binary val:0b", ivar_B);
		for (uint8_t bit = 0; bit < 3; bit++) {
			uint8_t bit_value = (ivar_B >> bit) & 1;
			printf("%d", bit_value);
		}
		printf("\nLED_1 ON\nLED_2 OFF\nLED_3 ON");
		if(ivar_B != 0){
			duty_cycle = 99;
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,1); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,0); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,1);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,duty_cycle);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,duty_cycle);
		}
		printf("\n");
		break;
	case 54: //011 -> 54 -> '6'
		ivar_B = OFF_led3;
		printf("ivar_B => dec val:%d   binary val:0b", ivar_B);
		for (uint8_t bit = 0; bit < 3; bit++) {
			uint8_t bit_value = (ivar_B >> bit) & 1;
			printf("%d", bit_value);
		}
		printf("\nLED_1 ON\nLED_2 ON\nLED_3 OFF");
		if(ivar_B != 0){
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,1); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,1); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,0);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,duty_cycle);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,duty_cycle);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		}
		printf("\n");
		break;
	case 55: //111 -> 55 -> '7'
		ivar_B = 30;
		ivar_B = ON_led1_2_3;
		printf("ivar_B => dec val:%d   binary val:0b", ivar_B);
		for (uint8_t bit = 0; bit < 3; bit++) {
			uint8_t bit_value = (ivar_B >> bit) & 1;
			printf("%d", bit_value);
		}
		printf("\nLED_1 ON\n\tLED_2 ON\n\tLED_3 ON");
		if(ivar_B != 0){
			duty_cycle = 99;
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,1); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,1); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,1);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,duty_cycle);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,duty_cycle);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,duty_cycle);
		}
		printf("\n");
		break;
	case 48: // 000 -> 48 -> '0'
		ivar_B = 0;
		printf("ivar_B => 0b");
		for (uint8_t bit = 0; bit < 3; bit++) {
			uint8_t bit_value = (ivar_B>> bit) & 1;
			printf("%d", bit_value);
		}
		printf("\nLED_1 OFF\n\tLED_2 OFF\n\tLED_3 OFF");
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,0); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,0); HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
		printf("\n");
		break;
	default:
		break;
	}

}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == huart1.Instance){
		HAL_UART_Receive_IT(&huart1,address_select,1);
		if(check == 0){
			if(*address_select == '#'){
			  flag = 1;
			}
			if(flag == 1){
					 if(rx_index < sizeof(buffer)){
						 buffer[rx_index] = *address_select;
						 if(*address_select == '#' && rx_index == 15){
							 rx_index = 0;
							 flag = 0;
							 check++;
							 address_select = address_var();
						 }
					 rx_index++;
					 }
			}
		}
		if(check == 1){
			printf("\ndata has been successfully received\n");
			//get first address of character in message and attach to 3 variable
			pvar_A = strtok(buffer,":");
			pvar_B = strtok(buffer+6,":");
			pvar_C = strtok(buffer+11,":");
			//copy amount of data necessary and save in 3 variables
			strncpy(var_A,pvar_A+2,3);
			strncpy(var_B,pvar_B+2,3);
			strncpy(var_C,pvar_C+2,3);
			//convert char to integer/''
			ivar_A = atoi(var_A);
			ivar_B = atoi(var_B);
			ivar_C = atoi(var_C);
			count = 1;
			address_select = address_var();
			check = 2;
		}
		if(check == 2){
			key = 1;
		}
		if(check == 3){
			//HAL_UART_Receive_IT(&huart1,&temp_varA,1);
			if(*address_select != 0){ // temp_varA
				key = 3; // enable flag for control PWM led
			}
		}
		if(check == 4){
			//HAL_UART_Receive_IT(&huart1,&temp_varC,2);
			if(*address_select != 0){ //temp_varC
				key = 6; // enable flag for control STOP/STANDARD MODE
			}
		}
		if(check == 5){
			//HAL_UART_Receive_IT(&huart1,&temp_varB,2);
			if(*address_select != 0){ // temp_varB
				key = 9; // enable flag for control led ON/OFF
			}
		}
	}
 }


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
