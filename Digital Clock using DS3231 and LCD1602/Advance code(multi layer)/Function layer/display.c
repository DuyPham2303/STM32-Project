#include"display.h"
#include"stdio.h"
LCD_I2C_Name MY_LCD;
DS3231_Name rtc;
extern volatile uint8_t flag_clr;
char mystring[] = "DIGITAL CLOCK";
char buffer[16];
char string[] = "**HELLO WORLD**";
char string1[] = "^_^";
int8_t hour = 0,minute = 0,second = 0;
extern I2C_HandleTypeDef hi2c1;
void Init_Display(){
	  LCD_I2C_Init(&MY_LCD, &hi2c1,PCF8574_ADDRESS,16,2);
	  DS3231_Init(&rtc,&hi2c1);
}
void Start_Display(){
	LCD_I2C_SetCursor(&MY_LCD,0,0);
	LCD_I2C_WriteString(&MY_LCD,string);
	LCD_I2C_SetCursor(&MY_LCD,6,1);
	LCD_I2C_WriteString(&MY_LCD,string1);
	HAL_Delay(3000);
	LCD_I2C_Clear(&MY_LCD);
	DS3231_SetTime(&rtc, hour, minute, second);
}
void Mode_Run_Display(){
	DS3231_GetTime(&rtc);
	LCD_I2C_SetCursor(&MY_LCD,1,0);
	LCD_I2C_WriteString(&MY_LCD,mystring);
	LCD_I2C_SetCursor(&MY_LCD,4,1);
	sprintf(buffer,"%.2d:%.2d:%.2d",rtc.Hours,rtc.Min,rtc.Sec);
	LCD_I2C_WriteString(&MY_LCD,buffer);
}
void Mode_Setting_Display(){
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
