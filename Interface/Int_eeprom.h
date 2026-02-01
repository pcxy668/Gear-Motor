#ifndef __INT_EEPROM_H__
#define __INT_EEPROM_H__

#include <stdint.h>
#define W_ADDR 0xA0
#define R_ADDR 0xA1

void EEPROM_PageWrite(uint8_t inner_addr,uint8_t *bytes,uint8_t size);
void EEPROM_ReadBytes(uint8_t inner_addr,uint8_t *bytes,uint8_t size);

#endif
