#ifndef __ADC_H
#define __ADC_H
#include <main.h>

// ��/�ر�(��1/0)BAT�ɼ���·��MOS
#define BAT_MOS_OPEN    (P6OUT |= BIT4)
#define BAT_MOS_CLOSE   (P6OUT &= (~BIT4))

// ���Ŷ��� P65 P66
#define ADC_BAT_PIN     BIT5
#define ADC_VIN_PIN     BIT6


typedef enum
{
  INCH_BAT = INCH_5,    // ����ͨ��5
  INCH_VIN = INCH_6,    // ����ͨ��6
}ADC_CHANNEL_t;

typedef struct
{
  float fBAT_Voltage;   // ��ص�ѹ ��ʼ��Ĭ��3.7V
  float fVIN_VOltage;   // �����ѹ
  
  void (*vADC_Init)(void);      // ADC��ʼ��
  void (*vADC_Get_BAT_Voltage)(void);       // ��ȡ��ص�ѹ
  void (*vADC_Get_VIN_Voltage)(void);       // ��ȡ�����ѹ
}ADC_t;

extern ADC_t ADC;

#endif
