#ifndef __SYSTEM_H
#define __SYSTEM_H
#include <main.h>

// ϵͳ״̬�궨��
#define System_StandBy  (uint8_t)0
#define System_Run      (uint8_t)1
// ����궨��
#define ErrorCode_NoError (uint8_t)0
#define ErrorCode_CLK     (uint8_t)1

typedef struct
{
  uint8_t ucSystem_Status;  // ϵͳ״̬
  uint8_t ucSystem_ErrorCode; // ϵͳ������
  
  void (*vRun)(void); // ϵͳ����
  void (*vStandBy)(void); // ϵͳ����
  void (*vError_Handle)(void);  // ������
}System_t;


extern System_t System;

#endif

