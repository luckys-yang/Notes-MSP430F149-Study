/***************************************************************************
 * File          : pwm.c
 * Author        : Luckys.
 * Date          : 2023-06-07
 * description   : PWM���
------------------
LED�������ų�ʼ��Ĭ��Ϊ�ߵ�ƽ���رպ���״̬
------------------
****************************************************************************/

#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vPWM_Init(void);      // PWM��ʼ��
static void vLED_Power_Control(LED_Power_t);     // LED��Դ����
static void vLED_Bright_Adjust(LED_Bright_t);      // ����LED������
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
* @brief        : PWM��ʼ��
*/
static void vPWM_Init(void)
{
  vLED_Power_Control(LED_Power_ON);     // ��LED��Դ
  Public.vDelay_ms(5);  // ��ʱ���ȴ�LED����IC�ȶ�����
  CCTL2 |= OUTMOD_6;    // ѡ��ģʽ6
  CCR2 = Pwm.LED_Duty;  // ��ʼռ�ձ�
  P1SEL |= LED_PWM_PIN; // �����������Ź���
}

/*
* @function     : vLED_Power_Control
* @param        : None
* @retval       : None
* @brief        : LED��Դ����
*/
static void vLED_Power_Control(LED_Power_t Value)
{
  if (Value == LED_Power_ON)
  {
    P1OUT |= LED_POWER_EN_PIN;  // �ߵ�ƽ�ǹر�
  }
  else
  {
    P1OUT &= (~LED_POWER_EN_PIN);  // �͵�ƽ�Ǵ�
  }
}

/*
* @function     : LED_Bright_Adjust
* @param        : None
* @retval       : None
* @brief        : ����LED������
*/
static void vLED_Bright_Adjust(LED_Bright_t Value)
{
  // ռ�ձ�Խ������ԽС
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
  // ����PWMռ�ձ�
  CCR2 = Pwm.LED_Duty;
}



