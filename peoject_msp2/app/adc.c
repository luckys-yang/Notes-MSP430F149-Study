/***************************************************************************
 * File          : adc.c
 * Author        : Luckys.
 * Date          : 2023-06-07
 * description   : ADC  
****************************************************************************/

#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vADC_Init(void);    // ADC��ʼ��
static void vADC_Get_BAT_Voltage(void); // ��ȡ��ص�ѹ
static void vADC_Get_VIN_Voltage(void); // ��ȡ�����ѹ
/*====================================static function declaration area   END====================================*/

ADC_t ADC = 
{
  3.7,
    0,
  vADC_Init,
  vADC_Get_BAT_Voltage,
  vADC_Get_VIN_Voltage,
};

/*
* @function     : vADC_Init
* @param        : None
* @retval       : None
* @brief        : ADC��ʼ��
*/
static void vADC_Init(void)
{
  P6SEL |= ADC_BAT_PIN + ADC_VIN_PIN;   // ��������
  // ����ADC12 ����0�Ĵ���--- ��������256 + �ο��ڲ���ѹ2.5V + ʹ��2.5V + ʹ��ADC12
  ADC12CTL0 = SHT0_8 + REF2_5V + REFON + ADC12ON;
}

/*
* @function     : vADC_Get_BAT_Voltage
* @param        : None
* @retval       : None
* @brief        : ��ȡ��ص�ѹ
*/
static void vADC_Get_BAT_Voltage(void)
{
  // ��MOS��
  BAT_MOS_OPEN;
  Public.vDelay_ms(1);
  printf("�ɼ���ص�ѹ��\r\n");
  // ����ADC�Ĵ���ǰ��Ҫ��ENC��0
  ADC12CTL0 &= (~ENC);
  // �������ģʽ SHP = 1
  // ת��ģʽѡ��ͨ��������ת�� CONSEQ = 00  
  // ����ת���ڴ�Ĵ���ѡ��5 
  // ADC12ʱ��Ĭ��ѡ��ADC12OSC�����5MHz, ����Ƶ
  // ��ѹ�����뱣��ʱ��Դѡ��������� - ADC12SC
  ADC12CTL1 = CSTARTADD_5 + SHP + CONSEQ_0;
  // �ο���ѹVR+ = VREF+�� VR- = VREF-
  // ѡ������ͨ��5---��Ӳ����أ���ΪBAT������A5�� 
  ADC12MCTL5 = SREF_5 + INCH_BAT;
  // ʹ��ת��
  ADC12CTL0 |= ENC;
  
  // ��ʼת��
  ADC12CTL0 |= ADC12SC;
  // �ȴ�ת������(��־λ��1)--ͨ���ж��жϱ�־λ��������û�д��жϵ��ǻ�����ͨ��������ѯ�Ƿ�ת����
  while ((ADC12IFG & ADC_BAT_PIN) != ADC_BAT_PIN);
  // �����ѹ
  ADC.fBAT_Voltage = (ADC12MEM5 * 2.5) / 4095;
  ADC.fBAT_Voltage *= 2;
  // ��ӡ
  printf("VBAT: %.2fV\r\n",ADC.fBAT_Voltage);
  // �ر�MOS��
  BAT_MOS_CLOSE;  
}

/*
* @function     : vADC_Get_VIN_Voltage
* @param        : None
* @retval       : None
* @brief        : ��ȡ�����ѹ
*/
static void vADC_Get_VIN_Voltage(void)
{
  printf("�ɼ�̫���ܵ�ѹ��\r\n");
  // ����ADC�Ĵ���ǰ��Ҫ��ENC��0
  ADC12CTL0 &= (~ENC);
  // �������ģʽ SHP = 1
  // ת��ģʽѡ��ͨ��������ת�� CONSEQ = 00  
  // ����ת���ڴ�Ĵ���ѡ��6 (��������������5,4,3,2,1,����ǿ����6)
  // ADC12ʱ��Ĭ��ѡ��ADC12OSC�����5MHz, ����Ƶ
  // ��ѹ�����뱣��ʱ��Դѡ��������� - ADC12SC
  ADC12CTL1 = CSTARTADD_6 + SHP + CONSEQ_0;
  // �ο���ѹVR+ = VREF+�� VR- = VREF-
  // ѡ������ͨ��6---��Ӳ����أ���ΪBAT������A5�� 
  ADC12MCTL6 = SREF_5 + INCH_VIN;
  // ʹ��ת��
  ADC12CTL0 |= ENC;
  
  // ��ʼת��
  ADC12CTL0 |= ADC12SC;
  // �ȴ�ת������(��־λ��1)--ͨ���ж��жϱ�־λ��������û�д��жϵ��ǻ�����ͨ��������ѯ�Ƿ�ת����
  while ((ADC12IFG & ADC_VIN_PIN) != ADC_VIN_PIN);
  // �����ѹ
  ADC.fVIN_VOltage = (ADC12MEM6 * 2.5) / 4095;
  ADC.fVIN_VOltage *= 9.2;      // ������� 9.2*2.5V=23V
  // ��ӡ
  printf("VIN: %.2fV\r\n",ADC.fVIN_VOltage);
}
