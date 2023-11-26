#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f1xx_hal.h"

#define FLASH_ADDR_PAGE_126 ((uint32_t)0x0801F800)	//Page 126
#define FLASH_ADDR_PAGE_127 ((uint32_t)0x0801FC00)	//Page 127

#define FLASH_USER_START_ADDR   FLASH_ADDR_PAGE_126
#define FLASH_USER_END_ADDR     FLASH_ADDR_PAGE_127 + FLASH_PAGE_SIZE

void FLASH_WritePage(uint32_t startPage, uint32_t endPage, uint32_t data);
uint32_t FLASH_ReadData(uint32_t addr);
void FLASH_WritePageArray(uint32_t address , uint8_t* arr_write,uint16_t len);
void FLASH_ReadPageArray(uint32_t address,uint8_t* arr_read,uint16_t len);
void Flash_Write_Array(uint32_t address,uint8_t* arr,uint16_t len);
void Flash_Read_Array(uint32_t address,uint8_t* arr,uint16_t len);
void Flash_erase(uint32_t address);
int Flash_Read_Int(uint32_t address);
void Flash_Write_Int(uint32_t address,int value);
#endif
