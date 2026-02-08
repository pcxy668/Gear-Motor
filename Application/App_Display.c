#include "App_Display.h"

uint8_t machine_id = 1;   // 设备id
uint8_t display_page = 0; // 页面序号
GearMotor_PID_Type motor_pid = {
    .kp = 0.04,
    .ki = 0.1,
    .kd = 0
}; 

/**
 * 使用AT24C02来永久化存储从设备ID
 * 选择一个字节地址来存储id =>   0x01
 * 还要选择一个地址来存储验证码   0x00 => 68
 *
 * 先读取验证码 => 判断是否为68 => 是 => 读取id
 * 不是68 => 第一次启动 => 写入验证码 => 写入初始值id
 */
void App_ID_Init(void)
{
    // 读取EEPROM中存储的验证码，地址0x00，验证码为68
    uint8_t code;
    EEPROM_ReadBytes(0x00, &code, 1);
    if (code != 68)
    {
        // 为新设备重新写入验证码和id
        code = 68;
        EEPROM_PageWrite(0x00, &code, 1);
        EEPROM_PageWrite(0x01, &machine_id, 1);
    }
    else
    {
        EEPROM_ReadBytes(0x01, &machine_id, 1);
    }
}

void App_OLED_Init(void)
{
    Int_oled_init();
}

/**
 * 展示ID修改和设置的圈数
 *
 * 存在两个页面
 *  1: 主页面 => 展示需要运行的圈数set: 当前的状态state:
 *  2. ID页面 => 展示从设备ID
 *
 *  开关标识 => 0: 主页面  1: ID页面 => 每次切换需要清空屏幕
 */
void App_OLED_Display(void)
{
    char str[16] = {0};
    if (display_page == 0)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            // 展示“直流减速电机项目”
            Int_oled_displayWord(i * 2, 0, 13 + i);
        }
        sprintf(str, "Set:%-4d", (int32_t)motor_pid.target_speed);
        Int_oled_displayStr(4, 1, str);
        sprintf(str, "Speed:%-4d",(int32_t)motor_pid.current_speed);
        Int_oled_displayStr(4, 2, str);
    }
    else if (display_page == 1)
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            // 展示“直流减速电机项目”
            Int_oled_displayWord(i * 2, 0, 13 + i);
        }
        sprintf(str, "ID:%03d", machine_id);
        Int_oled_displayStr(5, 1, str);
    }
}
