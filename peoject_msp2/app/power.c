/***************************************************************************
 * File          : power.c
 * Author        : Luckys.
 * Date          : 2023-06-09
 * description   : ��Դ  
****************************************************************************/


#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vBoost_5V_Control(PWR_t);     // ��ѹ��Դ����
static void vPeripheral_3V3_Control(PWR_t);       // �����Դ����
static void vCharging_Indicate(void);     // ���ָʾ
/*====================================static function declaration area   END====================================*/


Power_t Power = 
{
  Charge_Status_Stop,
  PWR_Status_ON,
  PWR_Status_ON,
  vBoost_5V_Control,
  vPeripheral_3V3_Control,
  vCharging_Indicate,
};

/*
* @function     : vBoost_5V_Control
* @param        : PWR_Status -> ��Դ״̬
* @retval       : None
* @brief        : ��ѹ��Դ����(P50�ܽ�)
*/
static void vBoost_5V_Control(PWR_t PWR_Status)
{
  if (PWR_ON == PWR_Status)
  {
    P5OUT |= BIT0;      // ʹ����ѹоƬ--ME2159
    Power.Boost_5V_Status = PWR_Status_ON;
  }
  else
  {
    P5OUT &= (~BIT0);      // ʧ����ѹоƬ--ME2159
    Power.Boost_5V_Status = PWR_Status_OFF;    
  }
}

/*
* @function     : vPeripheral_3V3_Control
* @param        : None
* @retval       : None
* @brief        : �����Դ����
*/
static void vPeripheral_3V3_Control(PWR_t PWR_Status)
{
  if (PWR_ON == PWR_Status)
  {
    P6OUT &= (~BIT3);   // ��PMOS��
    Power.Peripheral_3V3_Status = PWR_Status_ON;
    P5DIR |= (BIT5 + BIT6);     // �ָ�SDA SCL �ܽŷ���Ϊ���
    OLED.vOLED_Init();  // OLED��ʼ��
  }
  else
  {
    P6OUT |= BIT3;   // �ر�PMOS��
    Power.Peripheral_3V3_Status = PWR_Status_OFF;
    P5DIR &= (~(BIT5 + BIT6));  // ��SDA SCL �ܽ���Ϊ����
  }
}

/*
* @function     : vCharging_Indicate
* @param        : None
* @retval       : None
* @brief        : ��ѹ��Դ����
*/
static void vCharging_Indicate(void)
{
  //���ڳ�� - CHAGΪ�͵�ƽ
  if((P1IN & BIT0) != BIT0)
  {
    OLED.vOLED_Show_CHN(6,64,"��");
    OLED.vOLED_Show_CHN(6,80,"��");
  }
  //��ɳ�� - DONEΪ�͵�ƽ
  else if((P1IN & BIT5) != BIT5)
  {
    OLED.vOLED_Show_CHN(6,64,"��");
    OLED.vOLED_Show_CHN(6,80,"��");
  }
  //ֹͣ��� - CHAG��DONE��Ϊ�ߵ�ƽ��̫���������ѹ���ͣ�CN3791û�й���
  else
  {
    OLED.vOLED_Show_CHN(6,64,"ͣ");
    OLED.vOLED_Show_CHN(6,80,"ֹ");
  }
}
