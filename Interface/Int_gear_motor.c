#include "Int_gear_motor.h"

extern GearMotor_PID_Type motor_pid;
extern uint8_t REG_COILS_BUF[REG_COILS_SIZE];
extern uint8_t REG_DISC_BUF[REG_DISC_SIZE];
extern uint16_t REG_INPUT_BUF[REG_INPUT_SIZE];

void Int_Gear_Motor_Init(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
}

void Int_Gear_Motor_Stop(void)
{
    motor_pid.current_speed = 0;
    motor_pid.error_sum = 0;
    motor_pid.error = 0;
    motor_pid.error_last = 0;
    motor_pid.output = 0;

    //线圈更新电机运行状态
    REG_COILS_BUF[2] = 0;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_RESET);
}

void Int_Gear_Motor_SetSpeed()
{
    if ((int32_t)motor_pid.target_speed > 0)
    {
        // 正转
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_SET);

        //线圈更新电机运行状态
        REG_COILS_BUF[2] = 1;   
        REG_DISC_BUF[2] = 1;
    }
    else if ((int32_t)motor_pid.target_speed < 0)
    {
        // 反转
        HAL_GPIO_WritePin(MOTOR_IN1_GPIO_Port, MOTOR_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_IN2_GPIO_Port, MOTOR_IN2_Pin, GPIO_PIN_RESET);

        //线圈更新电机运行状态
        REG_COILS_BUF[2] = 1;  
        REG_DISC_BUF[2] = 0; 
    }
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, motor_pid.output);
}

void Int_Gear_Motor_GetSpeed(void)
{
    if ((int32_t)motor_pid.target_speed == 0)
    {
        motor_pid.current_speed = 0;

        //线圈更新电机运行状态
        REG_COILS_BUF[2] = 0;        
    }
    else
    {
        // 此值为霍尔编码器一个脉冲周期的时间,单位为us，即1/11圈
        int32_t ccr1 = __HAL_TIM_GetCompare(&htim4, TIM_CHANNEL_1);

        // 此值为减速后电机一圈时间，减速比为21.3
        float time_Per_turn = ccr1 * 11 * 21.3 / 1000000;

        motor_pid.current_speed = 60.0 / time_Per_turn;

        if (motor_pid.target_speed < 0)
        {
            motor_pid.current_speed = -motor_pid.current_speed;
        }
    }

    //更新输入寄存器值
    REG_INPUT_BUF[3] = motor_pid.current_speed;
}

void Int_Gear_Motor_CalculatePID()
{
    float pid_target = abs(motor_pid.target_speed);
    float pid_measure = abs(motor_pid.current_speed);
    float pid_error_diff;
    motor_pid.error = pid_target - pid_measure;
    pid_error_diff = motor_pid.error - motor_pid.error_last;
    motor_pid.error_sum += motor_pid.error;
    motor_pid.output = motor_pid.kp * motor_pid.error + motor_pid.ki * motor_pid.error_sum+ motor_pid.kd * pid_error_diff; 
    if (motor_pid.output >= 100)
    {
        motor_pid.output = 100;
    }
    else if (motor_pid.output <= 0)
    {
        motor_pid.output = 0;
    }
    
    motor_pid.error_last = motor_pid.error;
}

// TIM4-CH1通道输入捕获中断，重置CNT值以便捕获寄存器始终为正确周期值
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        __HAL_TIM_SetCounter(&htim4, 0);
    }
}

// TIM2 溢出中断 每隔100ms溢出 计算PID输出值
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        if ((int32_t)motor_pid.target_speed != 0)
        {
            Int_Gear_Motor_GetSpeed();
            Int_Gear_Motor_CalculatePID();
            Int_Gear_Motor_SetSpeed();
        }
        //printf("target:%d,current:%d,output:%d\n",(int32_t)motor_pid.target_speed,(int32_t)motor_pid.current_speed,(int32_t)motor_pid.output);
    }
}
