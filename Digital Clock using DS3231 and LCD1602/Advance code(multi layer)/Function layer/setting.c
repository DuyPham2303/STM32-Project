#include"setting.h"
extern DS3231_Name rtc;
extern LCD_I2C_Name MY_LCD;
extern int8_t hour,second,minute;
extern volatile uint8_t flag_mode;
extern volatile uint8_t flag_up;
extern volatile uint8_t flag_down;
extern volatile uint8_t flag_cursor;
extern volatile uint8_t pointer_mode;
static void Init_Cursor(){
	LCD_I2C_CursorOn(&MY_LCD);
	LCD_I2C_BlinkOn(&MY_LCD);
	HAL_Delay(200);
}
void Handle_Button_Interrupt(){
	//set hour
		if(flag_cursor == 0){
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
		}
		//set minute
		else if(flag_cursor == 1){
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
		}
		//set second
		else if(flag_cursor == 2){
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
		}
		Init_Cursor();
}
void Reset_Time(){
	hour = 0,second = 0; minute = 0;
}
void Initalize_Time(){
	DS3231_SetTime(&rtc,hour, minute, second);
}

