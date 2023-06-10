#ifndef __TIMERA_H
#define __TIMERA_H
#include <main.h>


// 定义枚举类型
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
  uint16_t volatile usMCU_Run_Timer;    // 系统运行时间
  uint16_t volatile usUSART1_Delay_Timer;       // 串口延时定时器
  void (*vTimerA_Init)(void);   // 定时器初始化
}TimerA_t;

extern TimerA_t TimerA;

#endif
