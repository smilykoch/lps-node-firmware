/******EEPROM .h*****/
#ifndef FLASH_EEPROM_LNK_IN_H
#define FLASH_EEPROM_LNK_IN_H
 
#define FL_EERPOM_StartAddr ((uint32_t) 0x08010000) // start from address after 94kbyte flash size //0x080107000
#define FL_EEPROM_EndAddr ((uint32_t)   0x08010800) // 2K data size  //0x08017800
#define FL_EEPROM_PageSize ((uint16_t) 0x800) // 2K
 
/* Page status definitions */
#define ERASED ((uint16_t)0xFFFF) /* PAGE is empty */
#define RECEIVE_DATA ((uint16_t)0xEEEE) /* PAGE is marked to receive data */
#define VALID_PAGE ((uint16_t)0x0000) /* PAGE containing valid data */
 
#include "stdint.h"
#include <stm32f0xx_hal.h>

#define Data_8Bit (1)
#define Data_16Bit (2)
#define Data_32Bit (4)
 
 
#define PASSED (0xFFFFFFFF)
 
 
HAL_StatusTypeDef Flash_EEPROM_Write_Data( uint32_t Start_Address, uint16_t *data, uint16_t No_of_Data, uint8_t Data_format);
HAL_StatusTypeDef Flash_EEPROM_ErasePage( uint32_t Start_Address, uint16_t No_of_Size);
HAL_StatusTypeDef Flash_EEPROM_Read_Data( uint32_t Start_Address, uint16_t *dest_data, uint16_t No_of_Data, uint8_t Data_format);
HAL_StatusTypeDef Flash_EEPROM_Format( uint32_t Start_Address, uint32_t End_Address);
 
 
#endif /*FLASH_EEPROM_LNK_IN_H */