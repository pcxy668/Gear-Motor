#ifndef __INT_GEAR_MOTOR_H__
#define __INT_GEAR_MOTOR_H__

#include "tim.h"
#include "gpio.h"
#include "Com_Debug.h"
#include "stdlib.h"

typedef struct 
{
    float kp;
    float ki;
    float kd;
    float target_speed; // 设置转速，单位：转/分，范围-120~120，绝对值越大，速度越大
    float current_speed; // 当前转速，此值受PID控制
    float error;
    float error_last;
    float error_sum;
    float output;
} GearMotor_PID_Type;

void Int_Gear_Motor_Init(void);
void Int_Gear_Motor_Stop(void);
void Int_Gear_Motor_SetSpeed(void);
void Int_Gear_Motor_GetSpeed(void);
void Int_Gear_Motor_CalculatePID(void);

#endif /* __INT_GEAR_MOTOR_H__ */
