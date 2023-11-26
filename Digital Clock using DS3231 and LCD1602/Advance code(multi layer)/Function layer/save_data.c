#include"save_data.h"
extern int8_t hour,second,minute;
int8_t read_hour = 0,read_second = 0,read_minute = 0;
static void write_second(){
	Flash_erase(ADDRESS_SEC);
	Flash_Write_Int(ADDRESS_SEC,second);
}
static void write_minute(){
	Flash_erase(ADDRESS_MIN);
	Flash_Write_Int(ADDRESS_SEC,minute);
}
static void write_hour(){
	Flash_erase(ADDRESS_HOUR);
	Flash_Write_Int(ADDRESS_SEC,hour);
}
static void Read_Second(){
	read_second = Flash_Read_Int(ADDRESS_SEC);
	second = read_second;
}
static void Read_Minute(){
	read_minute = Flash_Read_Int(ADDRESS_MIN);
	minute = read_minute;
}
static void Read_Hour(){
	read_hour = Flash_Read_Int(ADDRESS_HOUR);
	hour = read_hour;
}
void Write_Flash(){
	write_second();
	write_minute();
	write_hour();
}
void Read_Flash(){
	Read_Second();
	Read_Minute();
	Read_Hour();
}
