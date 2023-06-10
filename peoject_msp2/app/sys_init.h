#ifndef __SYS_INIT_H
#define __SYS_INIT_H
#include <main.h>

typedef struct
{
  void (*vCLK_Init)(void);  // 时钟初始化
  void (*vGPIO_Init)(void); // 通用输入输出端口初始化
  void (*vIE_Init)(void); // 中断初始化
  void (*vPower_On_Indication)(void); // 上电指示
  void (*vSys_Init)(void); // 系统初始化
}Hardware_Init_t;

extern Hardware_Init_t Hardware_Init;

#endif

