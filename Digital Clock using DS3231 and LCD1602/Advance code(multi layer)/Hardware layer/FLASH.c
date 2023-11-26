#include "FLASH.h"
void FLASH_WritePage(uint32_t startPage, uint32_t endPage, uint32_t data)
{
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInit;
	EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInit.PageAddress = startPage;
	EraseInit.NbPages = (endPage - startPage)/FLASH_PAGE_SIZE;
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&EraseInit, &PageError);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startPage, data);
	HAL_FLASH_Lock();
}
void Flash_Write_Int(uint32_t address,int value){
	HAL_FLASH_Unlock();
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,address,value);
	HAL_FLASH_Lock();
}
void Flash_erase(uint32_t address){
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef Flash_EraseInit;
	Flash_EraseInit.Banks = 1; // to chuc bo nho trong flash, -> stm32 chi co 1 bo nho 128kB nen banks = 1
	Flash_EraseInit.TypeErase = FLASH_TYPEERASE_PAGES; // kieu xoa
	Flash_EraseInit.NbPages = 1;// so luong page muon xoa
	Flash_EraseInit.PageAddress = address;// dia chi cua page muon xoa
	uint32_t page_error;
	HAL_FLASHEx_Erase(&Flash_EraseInit, &page_error);
	HAL_FLASH_Lock();
}
uint32_t FLASH_ReadData(uint32_t addr)
{
	uint32_t data = *(__IO uint32_t *)(addr);
	return data;
}
void FLASH_WritePageArray(uint32_t address , uint8_t* arr_write,uint16_t len){
		HAL_FLASH_Unlock();
		FLASH_EraseInitTypeDef Flash_EraseInit;
		Flash_EraseInit.Banks = 1; // to chuc bo nho trong flash, -> stm32 chi co 1 bo nho 128kB nen banks = 1
		Flash_EraseInit.TypeErase = FLASH_TYPEERASE_PAGES; // kieu xoa
		Flash_EraseInit.NbPages = 1 ;// so luong page muon xoa
		Flash_EraseInit.PageAddress = address;// dia chi cua page muon xoa
		uint32_t page_error;
		HAL_FLASHEx_Erase(&Flash_EraseInit, &page_error);
		uint16_t* pt = (uint16_t*)arr_write; // get 2 byte everytime access pt
		for(uint16_t i = 0 ; i < (len + 1)/2 ; i++){
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address + 2*i, *pt);
			pt++;
		}
		HAL_FLASH_Lock();
}
void Flash_Write_Array(uint32_t address,uint8_t* arr,uint16_t len){
	HAL_FLASH_Unlock();
	uint16_t* pt = (uint16_t*)arr; // get 2 byte everytime access pt
	for(uint16_t i = 0 ; i < (len + 1)/2 ; i++){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address + 2*i, *pt);
		pt++;
	}
	HAL_FLASH_Lock();
}
void Flash_Read_Array(uint32_t address,uint8_t* arr,uint16_t len){
	uint16_t* pt = (uint16_t*)arr;
	for(uint16_t i = 0 ; i < (len+1)/2 ; i++){
		*pt = *(__IO uint16_t*)(address+2*i);
		pt++;
	}
}
void FLASH_ReadPageArray(uint32_t address,uint8_t* arr_read,uint16_t len){
	uint16_t* pt = (uint16_t*)arr_read;
	for(uint16_t i = 0 ; i < (len+1)/2 ; i++){
		*pt = *(__IO uint16_t*)(address+2*i);
		pt++;
	}
}
int Flash_Read_Int(uint32_t address){
	return *(__IO uint16_t*)(address);
}
