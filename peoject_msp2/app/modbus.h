#ifndef __MODBUS_H
#define __MODBUS_H
#include <main.h>

// ������
#define Modbus_Function_NUM_Read        (uint8_t)0x03
#define Modbus_Function_NUM_Write       (uint8_t)0x06

typedef struct
{
  uint16_t CRC; // CRCУ��ֵ
  uint16_t CRC_H;       // ��λ
  uint16_t CRC_L;       // ��λ
  uint16_t (*CRC_16_Check)(uint8_t*, uint8_t);  // CRCУ��
}CRC_16_t;

typedef struct
{
  uint16_t Modbus_Addr;        // ��ַ
  
  void (*vModbus_Protocol_Analysis)(USART1_t*); // ModbusЭ�����
}Modbus_t;


extern CRC_16_t CRC_16;
extern Modbus_t Modbus;

#endif
