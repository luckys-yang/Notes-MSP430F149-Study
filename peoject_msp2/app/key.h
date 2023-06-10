#ifndef __KEY_H
#define __KEY_H
#include <main.h>


// 按键引脚是P13
#define KEY1    BIT3
// 设定长按时间
#define Set_Long_TIME   TimerA_2s
// 设定双击时间
#define Set_Double_TIME TimerA_200ms

typedef struct
{
  uint8_t Key_Flag;     // 按键 标志位
  uint8_t Key_Click;    // 单击
  uint8_t Key_Double_Click;     // 双击
  uint8_t Key_Long_Click;       // 长按
  
  void (*Key_Detect)(void);     // 按键检测
}Key_t;

typedef enum
{
  STA1_KEY_UP = (uint8_t)0x01,  // 按键弹起
  STA2_KEY_D_SHAKE = (uint8_t)0x02,       // 按下抖动
  STA3_KEY_DOWN = (uint8_t)0x03,        // 按键按下
  STA4_KEY_U_SHAKE = (uint8_t)0x04,     // 弹起抖动
}Key_STA_Machine_Status_t;

typedef struct
{
  Key_STA_Machine_Status_t ucKey_STA_Machine_Status;    // 按键状态机状态
  uint16_t volatile usKey_STA_Machine_Scan_Timer;       // 状态机扫描定时器
  uint16_t volatile usKey_Double_Click_Timer;   // 双击定时器
  uint16_t volatile usKey_Long_Click_Timer;     // 长按定时器
}Key_STA_Machine_t;

extern Key_t Key;
extern Key_STA_Machine_t Key_STA_Machine;

#endif
