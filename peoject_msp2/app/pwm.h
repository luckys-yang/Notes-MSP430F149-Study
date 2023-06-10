#ifndef __PWM_H
#define __PWM_H
#include <main.h>

// 电源控制引脚P16
#define LED_POWER_EN_PIN        BIT6  
// PWM引脚P17
#define LED_PWM_PIN             BIT7

// 占空比
typedef enum
{
  PWM_Duty_0 = (uint16_t)0,     // 占空比为0%
  PWM_Duty_20 = (uint16_t)1000,     // 占空比为20%
  PWM_Duty_40 = (uint16_t)2000,     // 占空比为40%
  PWM_Duty_60 = (uint16_t)3000,     // 占空比为60%
  PWM_Duty_80 = (uint16_t)4000,     // 占空比为80%
  PWM_Duty_100 = (uint16_t)5001,     // 占空比为100%
}PWM_Duty_t;

// 大功率LED亮度
typedef enum
{
  LED_Bright_0,
  LED_Bright_1,
  LED_Bright_2,
  LED_Bright_3,
  LED_Bright_4,
  LED_Bright_5,
}LED_Bright_t;

// 大功率LED电源控制
typedef enum
{
  LED_Power_ON = (uint8_t)0,
  LED_Power_OFF = (uint8_t)1,
}LED_Power_t;

typedef struct
{
  PWM_Duty_t LED_Duty;  // PWM占空比
  
  void (*vPWM_Init)(void);      // PWM初始化
  void (*vLED_Power_Control)(LED_Power_t);     // LED电源控制
  void (*vLED_Bright_Adjust)(LED_Bright_t);      // 调整LED灯亮度
}Pwm_t;

extern Pwm_t Pwm;

#endif
