/***************************************************************************
 * File          : pwm.c
 * Author        : Luckys.
 * Date          : 2023-06-07
 * description   : PWM输出
------------------
LED两个引脚初始化默认为高电平即关闭和灭状态
------------------
****************************************************************************/

#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vPWM_Init(void);      // PWM初始化
static void vLED_Power_Control(LED_Power_t);     // LED电源控制
static void vLED_Bright_Adjust(LED_Bright_t);      // 调整LED灯亮度
/*====================================static function declaration area   END====================================*/

Pwm_t Pwm = 
{
  PWM_Duty_100,
  vPWM_Init,
  vLED_Power_Control,
  vLED_Bright_Adjust,
};

/*
* @function     : vPWM_Init
* @param        : None
* @retval       : None
* @brief        : PWM初始化
*/
static void vPWM_Init(void)
{
  vLED_Power_Control(LED_Power_ON);     // 打开LED电源
  Public.vDelay_ms(5);  // 延时，等待LED驱动IC稳定工作
  CCTL2 |= OUTMOD_6;    // 选择模式6
  CCR2 = Pwm.LED_Duty;  // 初始占空比
  P1SEL |= LED_PWM_PIN; // 开启复用引脚功能
}

/*
* @function     : vLED_Power_Control
* @param        : None
* @retval       : None
* @brief        : LED电源控制
*/
static void vLED_Power_Control(LED_Power_t Value)
{
  if (Value == LED_Power_ON)
  {
    P1OUT |= LED_POWER_EN_PIN;  // 高电平是关闭
  }
  else
  {
    P1OUT &= (~LED_POWER_EN_PIN);  // 低电平是打开
  }
}

/*
* @function     : LED_Bright_Adjust
* @param        : None
* @retval       : None
* @brief        : 调整LED灯亮度
*/
static void vLED_Bright_Adjust(LED_Bright_t Value)
{
  // 占空比越大亮度越小
  switch(Value)
  {
  case LED_Bright_0:
    {
      Pwm.LED_Duty = PWM_Duty_100;
      break;
    }
  case LED_Bright_1:
    {
      Pwm.LED_Duty = PWM_Duty_80;
      break;
    }
  case LED_Bright_2:
    {
      Pwm.LED_Duty = PWM_Duty_60;
      break;
    }
  case LED_Bright_3:
    {
      Pwm.LED_Duty = PWM_Duty_40;
      break;
    }
  case LED_Bright_4:
    {
      Pwm.LED_Duty = PWM_Duty_20;
      break;
    }
  case LED_Bright_5:
    {
      Pwm.LED_Duty = PWM_Duty_0;
      break;
    }    
  }
  // 更新PWM占空比
  CCR2 = Pwm.LED_Duty;
}



