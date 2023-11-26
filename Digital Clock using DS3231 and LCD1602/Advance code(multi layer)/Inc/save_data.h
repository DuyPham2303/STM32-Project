#ifndef INC_SAVE_DATA_H_
#define INC_SAVE_DATA_H_
#include"FLASH.h"
#define ADDRESS_SEC   ((uint32_t)0x0801F400)
#define ADDRESS_MIN   ((uint32_t)0x0801F800)
#define ADDRESS_HOUR  ((uint32_t)0x0801FC00)
void Read_Flash();
void Write_Flash();
#endif /* INC_SAVE_DATA_H_ */
