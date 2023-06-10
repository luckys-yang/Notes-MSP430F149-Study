#ifndef __TIMERB_H
#define __TIMERB_H
#include <main.h>

typedef struct
{
  void (*vTimerB_Init)(void);
}TimerB_t;

extern TimerB_t TimerB;

#endif
