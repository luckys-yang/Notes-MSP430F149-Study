#ifndef __PWM_H
#define __PWM_H
#include <main.h>

// ��Դ��������P16
#define LED_POWER_EN_PIN        BIT6  
// PWM����P17
#define LED_PWM_PIN             BIT7

// ռ�ձ�
typedef enum
{
  PWM_Duty_0 = (uint16_t)0,     // ռ�ձ�Ϊ0%
  PWM_Duty_20 = (uint16_t)1000,     // ռ�ձ�Ϊ20%
  PWM_Duty_40 = (uint16_t)2000,     // ռ�ձ�Ϊ40%
  PWM_Duty_60 = (uint16_t)3000,     // ռ�ձ�Ϊ60%
  PWM_Duty_80 = (uint16_t)4000,     // ռ�ձ�Ϊ80%
  PWM_Duty_100 = (uint16_t)5001,     // ռ�ձ�Ϊ100%
}PWM_Duty_t;

// ����LED����
typedef enum
{
  LED_Bright_0,
  LED_Bright_1,
  LED_Bright_2,
  LED_Bright_3,
  LED_Bright_4,
  LED_Bright_5,
}LED_Bright_t;

// ����LED��Դ����
typedef enum
{
  LED_Power_ON = (uint8_t)0,
  LED_Power_OFF = (uint8_t)1,
}LED_Power_t;

typedef struct
{
  PWM_Duty_t LED_Duty;  // PWMռ�ձ�
  
  void (*vPWM_Init)(void);      // PWM��ʼ��
  void (*vLED_Power_Control)(LED_Power_t);     // LED��Դ����
  void (*vLED_Bright_Adjust)(LED_Bright_t);      // ����LED������
}Pwm_t;

extern Pwm_t Pwm;

#endif
