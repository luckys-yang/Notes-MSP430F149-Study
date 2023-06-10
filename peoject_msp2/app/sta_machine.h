#ifndef __STA_MACHINE_H
#define __STA_MACHINE_H
#include <main.h>

// ����ö������
typedef enum
{
  STA1 = (uint8_t)0x01,
  STA2 = (uint8_t)0x02,
  STA3 = (uint8_t)0x03,
  STA4 = (uint8_t)0x04,
  STA5 = (uint8_t)0x05,
}STA_Machine_Status_t;

typedef struct
{
  STA_Machine_Status_t ucSTA_Machine_Status;    // ״̬��״̬
  uint8_t ucSTA_Machine_Switch_Flag;    // ״̬���л�״̬��־λ
  
  void (*vFun_STA1)(void);
  void (*vFun_STA2)(void);
  void (*vFun_STA3)(void);
  void (*vFun_STA4)(void);
  void (*vFun_STA5)(void);
}STA_Machine_t;


extern STA_Machine_t STA_Machine;

#endif
