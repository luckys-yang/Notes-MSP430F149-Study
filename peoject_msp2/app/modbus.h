#ifndef __MODBUS_H
#define __MODBUS_H
#include <main.h>

// 功能码
#define Modbus_Function_NUM_Read        (uint8_t)0x03
#define Modbus_Function_NUM_Write       (uint8_t)0x06

typedef struct
{
  uint16_t CRC; // CRC校验值
  uint16_t CRC_H;       // 高位
  uint16_t CRC_L;       // 低位
  uint16_t (*CRC_16_Check)(uint8_t*, uint8_t);  // CRC校验
}CRC_16_t;

typedef struct
{
  uint16_t Modbus_Addr;        // 地址
  
  void (*vModbus_Protocol_Analysis)(USART1_t*); // Modbus协议解析
}Modbus_t;


extern CRC_16_t CRC_16;
extern Modbus_t Modbus;

#endif
