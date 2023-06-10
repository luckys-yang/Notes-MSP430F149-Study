#ifndef __TIMERA_H
#define __TIMERA_H
#include <main.h>


// ����ö������
typedef enum
{
  TimerA_10ms = (uint16_t)2,
  TimerA_50ms = (uint16_t)10,
  TimerA_100ms = (uint16_t)20,
  TimerA_200ms = (uint16_t)40,
  TimerA_500ms = (uint16_t)100,
  TimerA_1s = (uint16_t)200,
  TimerA_2s = (uint16_t)400,
}TimerA_Value_t;

typedef struct
{
  uint16_t volatile usMCU_Run_Timer;    // ϵͳ����ʱ��
  uint16_t volatile usUSART1_Delay_Timer;       // ������ʱ��ʱ��
  void (*vTimerA_Init)(void);   // ��ʱ����ʼ��
}TimerA_t;

extern TimerA_t TimerA;

#endif
