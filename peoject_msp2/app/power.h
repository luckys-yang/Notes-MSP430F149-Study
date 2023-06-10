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
  PWR_Status_ON,        // 打开
  PWR_Status_OFF,       // 关闭
}PWR_Status_t;

typedef enum
{
  Charge_Status_Just = (uint8_t)1,
  Charge_Status_Full = (uint8_t)2,
  Charge_Status_Stop = (uint8_t)3,
}Charge_Status_t;

typedef struct
{
  Charge_Status_t Charge_Status;       //充电状态
  PWR_Status_t Boost_5V_Status; // 升压电源状态
  PWR_Status_t Peripheral_3V3_Status;   // 外设3.3V电源状态
  
  void (*vBoost_5V_Control)(PWR_t);     // 升压电源控制
  void (*vPeripheral_3V3_Control)(PWR_t);       // 外设电源控制
  void (*vCharging_Indicate)(void);     // 充电指示
}Power_t;


extern Power_t Power;

#endif
