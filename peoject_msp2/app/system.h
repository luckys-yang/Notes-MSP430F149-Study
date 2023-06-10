#ifndef __SYSTEM_H
#define __SYSTEM_H
#include <main.h>

// 系统状态宏定义
#define System_StandBy  (uint8_t)0
#define System_Run      (uint8_t)1
// 错误宏定义
#define ErrorCode_NoError (uint8_t)0
#define ErrorCode_CLK     (uint8_t)1

typedef struct
{
  uint8_t ucSystem_Status;  // 系统状态
  uint8_t ucSystem_ErrorCode; // 系统错误码
  
  void (*vRun)(void); // 系统运行
  void (*vStandBy)(void); // 系统待机
  void (*vError_Handle)(void);  // 错误处理
}System_t;


extern System_t System;

#endif

