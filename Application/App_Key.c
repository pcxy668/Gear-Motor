#include "App_Key.h"

uint8_t key_value;
extern uint8_t machine_id;
extern uint8_t display_page;
extern GearMotor_PID_Type motor_pid;

/**
 * 根据用户按下不同的按键 会进行不同的响应
 * 必须要在循环中调用
 */
void App_Key_Function(void)
{

    if (key_value == 4)
    {
        // 切换不同页面
        key_value = 0;
        display_page = (display_page + 1) % 2;
        Int_oled_clear();
    }
    else if (display_page == 0)
    {
        // 当前为主页面
        // 按下key1 => 速度加15
        // 按下key2 => 速度减15
        // 按下key3 => 速度归0
        switch (key_value)
        {
        case 1:
            key_value = 0;
            motor_pid.target_speed += 15;
            if (motor_pid.target_speed >= 120)
            {
                motor_pid.target_speed = 120;
            }
            break;
        case 2:
            key_value = 0;
            motor_pid.target_speed -= 15;
            if (motor_pid.target_speed <= -120)
            {
                motor_pid.target_speed = -120;
            }
            break;
        case 3:
            key_value = 0;
            motor_pid.target_speed = 0;
            break;
        default:
            break;
        }

        if ((int32_t)motor_pid.target_speed == 0)
        {
            Int_Gear_Motor_Stop();
        }
    }
    else if (display_page == 1)
    {
        // 当前为ID页面
        // 按下key1 => ID+1
        // 按下key2 => ID-1
        // 按下key3 => 存储id
        switch (key_value)
        {
        case 1:
            key_value = 0;
            machine_id++;
            break;
        case 2:
            key_value = 0;
            machine_id--;
            break;
        case 3:
            key_value = 0;
            EEPROM_PageWrite(0x01, &machine_id, 1);
            break;
        default:
            break;
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY1_Pin || GPIO_Pin == KEY2_Pin || GPIO_Pin == KEY3_Pin || GPIO_Pin == KEY4_Pin)
    {
        HAL_Delay(15);
        if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
        {
            key_value = 1;
        }
        else if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
        {
            key_value = 2;
        }
        else if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET)
        {
            key_value = 3;
        }
        else if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET)
        {
            key_value = 4;
        }
    }
}
