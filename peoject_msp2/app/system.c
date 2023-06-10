/***************************************************************************
 * File          : system.c
 * Author        : Luckys.
 * Date          : 2023-06-05
 * description   : ϵͳ���к���
****************************************************************************/

#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vRun(void); // ϵͳ����
static void vStandBy(void); // ϵͳ����
static void vError_Handle(void);  // ������

static void StandBy_Enter_Config(void); // �����������
static void StandBy_Exit_Config(void);  // �˳���������
/*====================================static function declaration area   END====================================*/

System_t System = 
{
  System_Run,
  ErrorCode_NoError,
  
  vRun,
  vStandBy,
  vError_Handle,
};

/*
* @function     : vRun
* @param        : None
* @retval       : None
* @brief        : ϵͳ����
*/
static void vRun(void)
{
  uint16_t Temp_uint  = 0;
  uint16_t i;
  
  //�ɼ������̫���ܰ��ѹ
  ADC.vADC_Get_BAT_Voltage();
  ADC.vADC_Get_VIN_Voltage();
   
  //OLED��ʾ̫���ܰ��ѹ
  Temp_uint = (uint16_t)(ADC.fVIN_VOltage * 10);
  OLED.vOLED_Show_Char(2,64,Temp_uint / 100 + '0',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(2,72,Temp_uint % 100 / 10 + '0',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(2,80,'.',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(2,88,Temp_uint % 10 + '0',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(2,96,'V',ASCII_SIZE_16);
  
  //OLED��ʾ��ص�ѹ
  Temp_uint = (uint16_t)(ADC.fBAT_Voltage * 10);
  OLED.vOLED_Show_Char(4,72,Temp_uint / 10 + '0',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(4,80,'.',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(4,88,Temp_uint % 10 + '0',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(4,96,'V',ASCII_SIZE_16);
  
  // ���ָʾ
  Power.vCharging_Indicate();
  //���500ms�ɼ�һ��
  for (i = 0; i < 500; i++)
  {
    Public.vDelay_ms(1);
    Key.Key_Detect();     // ����ɨ��
    if (Key.Key_Flag == TRUE)
    {
      break;
    }
  }

  if (Key.Key_Flag == TRUE)
  {
    if (Key.Key_Click == TRUE)  // ����
    {
      printf("ϵͳ����͹���ģʽ\r\n");
      System.ucSystem_Status = System_StandBy;
    }
    // ���KEY���б�־λ
    Key.Key_Flag = FALSE;
    Key.Key_Click = FALSE;
  }
}

/*
* @function     : vStandBy
* @param        : None
* @retval       : None
* @brief        : ϵͳ����
*/
static void vStandBy(void)
{
  StandBy_Enter_Config();       // ����
  
  // ����͹���LPM3
  WDTCTL = WDTPW + WDTHOLD;     // �رտ��Ź�,�������ʱϵͳ����
  TBR = 0;      // ��ǰ����ֵ��0��֤�̶�ʱ�份��
  LPM3;
  StandBy_Exit_Config();        // �˳�
}

/*
* @function     : vError_Handle
* @param        : None
* @retval       : None
* @brief        : ������
*/
static void vError_Handle(void)
{
  
}

/*
* @function     : StandBy_Enter_Config
* @param        : None
* @retval       : None
* @brief        : �����������
*/
static void StandBy_Enter_Config(void)
{
  // �ر�LED
  Led.vLed_OFF(LED1);
  Led.vLed_OFF(LED2);
  Led.vLed_OFF(LED3);
  Led.vLed_OFF(LED4);
  
  Power.vBoost_5V_Control(PWR_OFF);     // �ر���ѹ��Դ
  Power.vPeripheral_3V3_Control(PWR_OFF);       // �ر�3.3v�����Դ
  Pwm.vLED_Power_Control(LED_Power_OFF);        // �رմ���LED��Դ
  BAT_MOS_CLOSE;        // �ر�VAT��ѹ������·
  // �ܽ�����
  P3SEL &= (~BIT6);     // P36�Ǵ���1TX����ΪGPIO������͵�ƽ
  P3OUT &= (~BIT6);
  // ADC����
  ADC12CTL0 &= (~ENC);
  ADC12CTL0 &= (~REFON);        // �رջ�׼��ѹ
}

/*
* @function     : StandBy_Exit_Config
* @param        : None
* @retval       : None
* @brief        : �˳���������
*/
static void StandBy_Exit_Config(void)
{
  Power.vBoost_5V_Control(PWR_ON);     // �ָ���ѹ��Դ
  Power.vPeripheral_3V3_Control(PWR_ON);       // �ָ�3.3v�����Դ  
  BAT_MOS_OPEN; // �ָ�
  Pwm.vLED_Power_Control(LED_Power_ON);        // �򿪴���LED��Դ
  ADC.vADC_Init();
  // �ܽ�����
  P3SEL |= BIT6;        // �ָ�����1TX
  Public.vDelay_ms(10); // ��ʱ10ms�ȶ���Դ
}

