#ifndef __INT_OLED_H__
#define __INT_OLED_H__

#include "i2c.h"
#include "string.h"

void Int_oled_init(void);
void Int_oled_clear(void);
void Int_oled_sendDataBytes(uint8_t isData,const uint8_t *bytes,uint16_t len);
void Int_oled_setPointer(uint8_t col,uint8_t row);
void Int_oled_displayStr(uint8_t x,uint8_t y,char *str);
void Int_oled_displayBigFont(uint8_t x,uint8_t y,uint8_t num);
void Int_oled_displayIcon(uint8_t x,uint8_t y,uint8_t num);
void Int_oled_displayWord(uint8_t x,uint8_t y,uint8_t num);
void Int_oled_On(void);
void Int_oled_Off(void);

#endif /* __INT_OLED_H__ */
