#ifndef __USART1_H
#define __USART1_H
#include <main.h>

// 接收和发送最大字节
#define USART1_Send_LEN 20
#define USART1_Rec_LEN  20

typedef struct
{
  uint8_t volatile ucRec_Flag;  // 接收标志位
  uint8_t volatile ucRec_Cnt;   // 接收计数
  uint8_t* pucSend_Buffer;      // 发送缓存指针
  uint8_t* pucRec_Buffer;       // 接收缓存指针
  
  void (*vUSART1_Init)(void);   // 串口1初始化
  void (*vUSART1_SendArray)(uint8_t*, uint16_t);        // 发送数组
  void (*vUSART1_SendString)(uint8_t*); // 发送字符串
  void (*vUSART1_Protocol)(void);       // 接口协议
  
}USART1_t;


extern USART1_t USART1;

#endif
