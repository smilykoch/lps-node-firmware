/*
https://www.linkedin.com/groups/Flash-erase-write-on-stm32f103-4240067.S.108244546
Johnson C.
Founder/Director of Engineering at Dreampro Technology Ltd.
 
Actually, STM32F do not have internal EEPROM(beside STM32L series), ST website had use Internal Flash emulate the EEPROM function. It need consider the Flash start address and end address.
1. The start address do not cover your program code section. Otherwise, it will overwrite your program and your STM32 will hanging. You need the start address outside our total complied code size. ( e.g. your total flash size=64K, the start address >64K address)
2. Flash memory need erase the whole flash page (1Kbyte) data when you want to rewrite the same address. So, It will also erase other data in same page. It need read and backup the data in RAM first before erase the flash page.
3. The end address do you excess the physic memory of STM32 (e.g STM32F103RBT6 is max 128Kflash memory)
 
It is my example code for write EEPROM
It is my example code for Read/erase EEPROM
*/
 
/*****Flash eeprom.c*****/
/// user define function
 
#include "eeprom_emul.h"
 
HAL_StatusTypeDef FLASHStatus;
uint32_t FLASH_EEPROM_ProgramStatus;
uint32_t Flash_EEPROM_Addr;
uint32_t Flash_EEPROM_Nb_Of_Page;
uint32_t EraseCounter;
 
 
/**** Function : Write the internal Flash data****/
HAL_StatusTypeDef Flash_EEPROM_Write_Data( uint32_t Start_Address, uint16_t *data, uint16_t No_of_Data, uint8_t Data_format)
{
  uint32_t End_Of_Address = 0;
  FLASHStatus = HAL_OK;
   
  HAL_FLASH_Unlock();
   
  /*Start EEPROM*/
  Flash_EEPROM_Addr = Start_Address;
  End_Of_Address = Start_Address + (No_of_Data * Data_format);
  /* Write page data*/
  while((Flash_EEPROM_Addr<End_Of_Address) ){
    switch(Data_format){      
      case Data_8Bit :
        break;      
      case Data_16Bit :
        FLASHStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Flash_EEPROM_Addr, *data);
        if (FLASHStatus != HAL_OK){
          HAL_FLASH_Lock();
          return FLASHStatus;
        }
        Flash_EEPROM_Addr +=2; // point to next 2byte address
        data++;
        break;
      case Data_32Bit :
        FLASHStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Flash_EEPROM_Addr, *data);
        if (FLASHStatus != HAL_OK){
          HAL_FLASH_Lock();
          return FLASHStatus;
        }
        Flash_EEPROM_Addr +=4; // point to next 4byte address
        data++;
        break;
    }
  }
  HAL_FLASH_Lock();
  return FLASHStatus;
}
 
 
  
/*
Function : Erase the internal Flash data
Input :- start address 
- Number of page
Output: return the FLASH status
*/
HAL_StatusTypeDef Flash_EEPROM_ErasePage( uint32_t Start_Address, uint16_t No_of_Size)
{
  HAL_FLASH_Unlock();
  FLASHStatus=HAL_OK;
  FLASH_EEPROM_ProgramStatus= PASSED;

  FLASH_EraseInitTypeDef eDef;
  eDef.TypeErase = FLASH_TYPEERASE_PAGES;
  eDef.PageAddress = Start_Address;
  eDef.NbPages = No_of_Size;
   
  /*erase the page*/
  FLASHStatus = HAL_FLASHEx_Erase(&eDef, &FLASH_EEPROM_ProgramStatus);
   
  if (FLASHStatus != HAL_OK){
    HAL_FLASH_Lock();
    return FLASHStatus;
  }
 
   
  HAL_FLASH_Lock();
   
  return FLASHStatus;
   
}
 
HAL_StatusTypeDef Flash_EEPROM_Read_Data( uint32_t Start_Address, uint16_t *dest_data, uint16_t No_of_Data, uint8_t Data_format)
{
  uint32_t End_Of_Address=0;
  /*
  uint32_t *EEPROM_32Bit_p = (uint32_t *)Start_Address; // pointer point to address after 72kbyte flash size
  uint16_t *EEPROM_16Bit_p = (uint32_t *)Start_Address;
  uint8_t *EEPROM_8Bit_p = (uint32_t *)Start_Address;
  */
  FLASHStatus=HAL_OK;
   
   
  // HAL_FLASH_Unlock();
  /*Start EEPROM*/
  Flash_EEPROM_Addr= Start_Address;
  End_Of_Address= Start_Address +(No_of_Data * Data_format) ;

  while((Flash_EEPROM_Addr<End_Of_Address))
  {
    switch(Data_format){
      case Data_8Bit : 
        *dest_data = *(uint8_t *)Flash_EEPROM_Addr ;   //
        dest_data++ ;
        Flash_EEPROM_Addr+=1;
        break;
      case Data_16Bit : 
        *dest_data = *(uint16_t *)Flash_EEPROM_Addr ;
        dest_data++;
        Flash_EEPROM_Addr+=2;
        break;
      case Data_32Bit : 
        *dest_data = *(uint32_t *)Flash_EEPROM_Addr ;
        dest_data++;
        Flash_EEPROM_Addr+=4;
        break;
      default: 
        break;  
    }
  }
  HAL_FLASH_Lock();
  return FLASHStatus;
   
}
 
/*
Function : format the internal Flash data
Input :- start address 
- Number of page
Output: return FLASHStatus;
*/
HAL_StatusTypeDef Flash_EEPROM_Format( uint32_t Start_Address, uint32_t End_Address)
{
  HAL_FLASH_Unlock();
  FLASHStatus=HAL_OK;
  FLASH_EEPROM_ProgramStatus= PASSED;


  FLASH_EraseInitTypeDef eDef;
  eDef.TypeErase = FLASH_TYPEERASE_PAGES;
  eDef.PageAddress = Start_Address;
  eDef.NbPages = (End_Address- Start_Address)/FL_EEPROM_PageSize;
  if (eDef.NbPages< 1){
    eDef.NbPages=1;
  }
   
  /*erase the page*/
  FLASHStatus = HAL_FLASHEx_Erase(&eDef, &FLASH_EEPROM_ProgramStatus);
   
  if (FLASHStatus != HAL_OK){
    HAL_FLASH_Lock();
    return FLASHStatus;
  }
   
  HAL_FLASH_Lock();
   
  return FLASHStatus;
   
}