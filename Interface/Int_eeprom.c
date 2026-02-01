#include "Int_eeprom.h"
#include "i2c.h"

void EEPROM_PageWrite(uint8_t inner_addr, uint8_t *bytes, uint8_t size)
{
    HAL_I2C_Mem_Write(&hi2c2,W_ADDR,inner_addr,I2C_MEMADD_SIZE_8BIT,bytes,size,1000);
    HAL_Delay(5);
}

void EEPROM_ReadBytes(uint8_t inner_addr, uint8_t *bytes, uint8_t size)
{
    HAL_I2C_Mem_Read(&hi2c2,R_ADDR,inner_addr,I2C_MEMADD_SIZE_8BIT,bytes,size,1000);
}
