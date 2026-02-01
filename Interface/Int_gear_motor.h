#ifndef __INT_GEAR_MOTOR_H__
#define __INT_GEAR_MOTOR_H__

#include "tim.h"
#include "gpio.h"
#include "stdlib.h"

void Int_Gear_Motor_Init(void);
void Int_Gear_Motor_Stop(void);
void Int_Gear_Motor_SetSpeed(int16_t set_speed);

#endif /* __INT_GEAR_MOTOR_H__ */
