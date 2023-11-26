/* USER CODE BEGIN Header */
/**
  ******************************************************************************
#include "stdio.h"
#define PCF8574_ADDRESS 0x27 << 1
#define DS3231_ADDRESS 0x68 << 1
#define LCD_LEN	16
LCD_I2C_Name MY_LCD;
char mystring[] = "DIGITAL CLOCK";
char buffer[16];
uint8_t Tx_time[] = {0,0,0};
uint8_t Rx_time[3];
int hour,second,minute;
volatile uint8_t flag_mode = 0;
volatile uint8_t flag_up = 0;
volatile uint8_t flag_down = 0;
volatile uint8_t flag_enter = 0;
volatile uint8_t count = 0;
uint8_t flag_clr = 0;

uint8_t BCDtoDEC(uint8_t data)
{
	return (data>>4)*10 + (data&0x0f);
}
uint8_t DECtoBCD(uint8_t data)
{
	return (data/10)<<4|(data%10);
}
void ReadTime(){
	HAL_I2C_Mem_Read(&hi2c1,DS3231_ADDRESS,0,I2C_MEMADD_SIZE_8BIT,Rx_time,3,10);
	second = BCDtoDEC(Rx_time[0]);
	minute = BCDtoDEC(Rx_time[1]);
	hour = BCDtoDEC(Rx_time[2]);
}
void SettingTime(){
	//set hour
	if(flag_enter == 0){
		LCD_I2C_SetCursor(&MY_LCD,5,1);
		if(flag_up == 1){
			hour++;
			if(hour > 23){
				hour = 0;
			}
			flag_up = 0; // reset interrupt flag
		}
		if(flag_down == 1){
			hour--;
			if(hour < 0){
				hour = 23;
			}
			flag_down = 0; // reset interrupt flag
		}
		Tx_time[2] = DECtoBCD(hour);
	}
	//set minute
	else if(flag_enter == 1){
		LCD_I2C_SetCursor(&MY_LCD,8,1);
		if(flag_up == 1){
			minute++;
			if(minute > 59){
				minute = 0;
			}
			flag_up = 0; // reset interrupt flag
		}
		if(flag_down == 1){
			minute--;
			if(minute < 0){
				minute = 59;
			}
			flag_down = 0; // reset interrupt flag
		}
		Tx_time[1] = DECtoBCD(minute);
	}
	//set second
	else if(flag_enter == 2){
		LCD_I2C_SetCursor(&MY_LCD,11,1);
		if(flag_up == 1){
			second++;
			if(second > 59){
				second = 0;
			}
			flag_up = 0; // reset interrupt flag
		}
		if(flag_down == 1){
			second--;
			if(second < 0){
				second = 59;
			}
			flag_down = 0; // reset interrupt flag
		}
		Tx_time[0] = DECtoBCD(second);
	}
	LCD_I2C_CursorOn(&MY_LCD);
	LCD_I2C_BlinkOn(&MY_LCD);
	HAL_Delay(200);
}
void Mode_0(){
	ReadTime();
	LCD_I2C_SetCursor(&MY_LCD,1,0);
	LCD_I2C_WriteString(&MY_LCD,mystring);
	LCD_I2C_SetCursor(&MY_LCD,4,1);
	sprintf(buffer,"%.2d:%.2d:%.2d",hour,minute,second);
	LCD_I2C_WriteString(&MY_LCD,buffer);
}
void Mode_1(){
	if(flag_clr == 1){
		LCD_I2C_Clear(&MY_LCD); 
		flag_clr = 0;	// reset interrupt flag
	}
	LCD_I2C_SetCursor(&MY_LCD,2,0);
	LCD_I2C_WriteString(&MY_LCD,"SETTING MODE");
	LCD_I2C_SetCursor(&MY_LCD,4,1);
	sprintf(buffer,"%.2d:%.2d:%.2d",hour,minute,second);
	LCD_I2C_WriteString(&MY_LCD,buffer);
}

int main(void)
{
  HAL_Init(); 
  //SET INITIAL TIME
  HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS,0,I2C_MEMADD_SIZE_8BIT,Tx_time,3,10);
  // SET UP LCD
  LCD_I2C_Init(&MY_LCD, &hi2c1,PCF8574_ADDRESS,16,2);
  while (1){
	if(flag_mode == 0){
		Mode_0(); // hiển thị màn hình chạy thời gian
	}
	else{
		Mode_1(); // hiển thị màn hình cài đặt
		SettingTime(); // hàm xử lý 
	}
  }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_0){
		count++;
		if(count == 1){
			flag_clr = 1;
			flag_mode = 1;
		}
		else{
			HAL_I2C_Mem_Write(&hi2c1,DS3231_ADDRESS,0,I2C_MEMADD_SIZE_8BIT,Tx_time,3,10);
			flag_mode = 0;
			count = 0;
		}
	}
	if(GPIO_Pin == GPIO_PIN_1){
		flag_up = 1;
	}
	if(GPIO_Pin == GPIO_PIN_2){
		flag_down = 1;
	}
	if(GPIO_Pin == GPIO_PIN_3){
		flag_enter++;
		if(flag_enter == 3){
			flag_enter = 0;
		}
	}
}


