#ifndef __POWER_H
#define __POWER_H
#include <main.h>

typedef enum
{
  PWR_ON,
  PWR_OFF
}PWR_t;

typedef enum
{
  PWR_Status_ON,        // ��
  PWR_Status_OFF,       // �ر�
}PWR_Status_t;

typedef enum
{
  Charge_Status_Just = (uint8_t)1,
  Charge_Status_Full = (uint8_t)2,
  Charge_Status_Stop = (uint8_t)3,
}Charge_Status_t;

typedef struct
{
  Charge_Status_t Charge_Status;       //���״̬
  PWR_Status_t Boost_5V_Status; // ��ѹ��Դ״̬
  PWR_Status_t Peripheral_3V3_Status;   // ����3.3V��Դ״̬
  
  void (*vBoost_5V_Control)(PWR_t);     // ��ѹ��Դ����
  void (*vPeripheral_3V3_Control)(PWR_t);       // �����Դ����
  void (*vCharging_Indicate)(void);     // ���ָʾ
}Power_t;


extern Power_t Power;

#endif
