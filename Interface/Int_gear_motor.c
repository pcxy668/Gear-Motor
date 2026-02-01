#include "Int_gear_motor.h"

void Int_Gear_Motor_Init(void)
{
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
}

void Int_Gear_Motor_SetSpeed(int16_t set_speed)
{
    uint16_t speed = abs(set_speed);
    if (set_speed > 0)
    {
        //正转
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port,MOTOR_IN1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port,MOTOR_IN2_Pin,GPIO_PIN_SET);
    }
    else if (set_speed < 0)
    {
        //反转
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port,MOTOR_IN1_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port,MOTOR_IN2_Pin,GPIO_PIN_RESET);
    }    
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,speed);
}

void Int_Gear_Motor_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port,MOTOR_IN1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port,MOTOR_IN2_Pin,GPIO_PIN_RESET);
}
