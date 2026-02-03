#include "Int_gear_motor.h"

void Int_Gear_Motor_Init(void)
{
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);
}

void Int_Gear_Motor_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port,MOTOR_IN1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port,MOTOR_IN2_Pin,GPIO_PIN_RESET);
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

void Int_Gear_Motor_GetSpeed(void)
{
    if (set_speed == 0)
    {
        current_speed = 0;
    }
    else
    {
        //此值为霍尔编码器一个脉冲周期的时间,单位为us，即1/11圈
        int32_t ccr1 = __HAL_TIM_GetCompare(&htim4,TIM_CHANNEL_1);

        //此值为减速后电机一圈时间，减速比为21.3
        float time_Per_turn = ccr1 * 11 * 21.3 / 1000000;

        current_speed = (int16_t)(60.0 / time_Per_turn);

        if (set_speed < 0)
        {
            current_speed = -current_speed;
        }
    }
}

//TIM4-CH1通道输入捕获中断，重置CNT值以便捕获寄存器始终为正确周期值
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        __HAL_TIM_SetCounter(&htim4,0);
    }
}
