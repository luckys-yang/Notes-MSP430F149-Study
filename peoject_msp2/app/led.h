#ifndef __LED_H
#define __LED_H
#include <main.h>


// 定义枚举类型
typedef enum
{
  LED1 = (uint8_t)0x01,
  LED2 = (uint8_t)0x02,
  LED3 = (uint8_t)0x03,
  LED4 = (uint8_t)0x04,
}Led_Num_t;


typedef struct
{
  void (*vLed_ON)(Led_Num_t);     // 打开
  void (*vLed_OFF)(Led_Num_t);    // 关闭
  void (*vLed_Flip)(Led_Num_t);   // 翻转
}Led_t;

extern Led_t Led;

#endif
