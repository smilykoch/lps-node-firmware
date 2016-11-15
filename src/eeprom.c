/*
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * LPS node firmware.
 *
 * Copyright 2016, Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stm32f0xx_hal.h>
#include <stdio.h>

#include <stdbool.h>
#include <string.h>

#include "eeprom.h"
#include "eeprom_emul.h"


#define X_ (0)

uint16_t VirtAddVarTab[NB_OF_VAR];
uint16_t VarValue = 0;

void eepromInit(I2C_HandleTypeDef * i2c)
{
  HAL_FLASH_Unlock();

  int i = 0;
  for(i = 0; i < NB_OF_VAR; i++){
    VirtAddVarTab[i] = 0x5555 + i;
  }


  HAL_StatusTypeDef status = HAL_OK;
  status = EE_Init();
  if(status != HAL_OK){
    printf("Failed initializing EEPROM!\r\n");
  }else{
    printf("Success initializing EEPROM!\r\n");
  }

  /*for(i = 0; i < NB_OF_VAR; i++){
    EE_WriteVariable(VirtAddVarTab[i], 0);   
  }*/

  HAL_FLASH_Lock();
}

bool eepromTest()
{
  uint8_t test;
  return eepromRead(0, &test, 1);
}

bool eepromRead(int address, uint8_t* data, size_t length)
{
  HAL_StatusTypeDef status = HAL_OK;
  int i = 0;
  uint16_t data_16;
  HAL_FLASH_Unlock();



  for (i = 0; i < length; i++){       
    status = EE_ReadVariable(VirtAddVarTab[i] + address, &data_16);
    if (status != HAL_OK){
      HAL_FLASH_Lock();
      return false;
    }
    data[i] = (uint8_t)(data_16 & 0x00FF); 
  }

  HAL_FLASH_Lock();


  return true;

}

bool eepromWrite(int address, uint8_t* data, size_t length)
{
  HAL_StatusTypeDef status = HAL_OK;
  int i = 0;
  uint16_t data_16;

  HAL_FLASH_Unlock();


  for (i = 0; i < length; i++){       
    data_16 = (data[i] & 0x00FF);  
    status = EE_WriteVariable(VirtAddVarTab[i] + address, data_16);   
    if (status != HAL_OK){
      HAL_FLASH_Lock();

      return false;  
    }
  } 

  HAL_FLASH_Lock();


  return true;
}
