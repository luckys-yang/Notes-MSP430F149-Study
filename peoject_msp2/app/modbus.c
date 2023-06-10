/***************************************************************************
 * File          : modbus.c
 * Author        : Luckys.
 * Date          : 2023-06-09
 * description   : modbusЭ��
****************************************************************************/


#include <main.h>


/*====================================static function declaration area BEGIN====================================*/
static uint16_t CRC_16_Check(uint8_t*, uint8_t);        // CRCУ��
static void vModbus_Protocol_Analysis(USART1_t*);       // ModbusЭ�����
static void vModbus_Read_Register(USART1_t*);   // ���Ĵ���
static void vModbus_Write_Register(USART1_t*);   // д�Ĵ���
/*====================================static function declaration area   END====================================*/


CRC_16_t CRC_16 = 
{
  0,
  0,
  0,
  CRC_16_Check,
};

Modbus_t Modbus = 
{
  1,
  vModbus_Protocol_Analysis,
};


/*
* @function     : CRC_16_Check
* @param        : p_Arr -> ����ָ�� LEN -> ���鳤��
* @retval       : None
* @brief        : CRCУ��
*/
static uint16_t CRC_16_Check(uint8_t* p_Arr, uint8_t LEN)
{
  uint16_t CRC_Value = 0;
  uint8_t i = 0,j = 0;
  
  CRC_Value = 0xFFFF;
  
  for (i = 0; i < LEN; i++)
  {
    CRC_Value ^= *(p_Arr + i);
    for (j = 0; j < 8; j++)
    {
      if (CRC_Value & 0x00001)
      {
        CRC_Value = (CRC_Value >> 1) ^ 0xA001;
      }
      else
      {
        CRC_Value = (CRC_Value >>1);
      }
    }
  }
  CRC_Value = (CRC_Value >>8) + (CRC_Value <<8);        // �����ߵ��ֽ�
  
  return CRC_Value;
}

/*
* @function     : vModbus_Protocol_Analysis
* @param        : UARTx -> ����ָ��
* @retval       : None
* @brief        : ModbusЭ�����
*/
static void vModbus_Protocol_Analysis(USART1_t* UART)
{
  USART1_t* const COM = UART;
  
  // ����CRC
  CRC_16.CRC = CRC_16.CRC_16_Check(COM->pucRec_Buffer,6);
  CRC_16.CRC_H = (uint8_t)(CRC_16.CRC >> 8);
  CRC_16.CRC_L = (uint8_t)CRC_16.CRC;
  
  // ����
  if(((*(COM->pucRec_Buffer + 6) == CRC_16.CRC_L) && (*(COM->pucRec_Buffer + 7) == CRC_16.CRC_H)) || 
     ((*(COM->pucRec_Buffer + 6) == CRC_16.CRC_H) && (*(COM->pucRec_Buffer + 7) == CRC_16.CRC_L)))
  {
    // У���ַ
    if ((*(COM->pucRec_Buffer + 0)) == Modbus.Modbus_Addr)
    {
      // ��������
      if ((*(COM->pucRec_Buffer + 1)) == Modbus_Function_NUM_Read)        // ���Ĵ���
      {
        vModbus_Read_Register(COM);
      }
      else if ((*(COM->pucRec_Buffer + 1)) == Modbus_Function_NUM_Write)  // д�Ĵ���
      {
        vModbus_Write_Register(COM);
      }
    }
  }
}

/*
* @function     : vModbus_Read_Register
* @param        : UARTx -> ����ָ��
* @retval       : None
* @brief        : ���Ĵ���
*/
static void vModbus_Read_Register(USART1_t* UART)
{
  USART1_t* const COM = UART;
  
  // ��Ӧ����
  *(COM->pucSend_Buffer + 0) = Modbus.Modbus_Addr;      // ��ַ��
  *(COM->pucSend_Buffer + 1) = Modbus_Function_NUM_Read;      // ������
  *(COM->pucSend_Buffer + 2) = 8;      // ���ݳ���
  // VIN��ѹֵ
  *(COM->pucSend_Buffer + 3) = (uint16_t)(ADC.fVIN_VOltage * 10) / 256;      
  *(COM->pucSend_Buffer + 4) = (uint16_t)(ADC.fVIN_VOltage * 10) % 256;
  // BAT��ѹֵ
  *(COM->pucSend_Buffer + 5) = 0;      
  *(COM->pucSend_Buffer + 6) = (uint8_t)ADC.fBAT_Voltage * 10;  
  // ·������ֵ
  *(COM->pucSend_Buffer + 7) = Pwm.LED_Duty / 256;      
  *(COM->pucSend_Buffer + 8) = Pwm.LED_Duty % 256;  
  // ���״̬
  *(COM->pucSend_Buffer + 9) = 0;      
  *(COM->pucSend_Buffer + 10) = (uint8_t)Power.Charge_Status;  
  // ����CRC
  CRC_16.CRC = CRC_16.CRC_16_Check(COM->pucSend_Buffer,11);     // ����CRC
  CRC_16.CRC_H = (uint8_t)(CRC_16.CRC >> 8);
  CRC_16.CRC_L = (uint8_t)CRC_16.CRC;  
  // CRC
  *(COM->pucSend_Buffer + 11) = CRC_16.CRC_L;      
  *(COM->pucSend_Buffer + 12) = CRC_16.CRC_H;    
  // ��������
  USART1.vUSART1_SendArray(COM->pucSend_Buffer,13);
}

/*
* @function     : vModbus_Write_Register
* @param        : UARTx -> ����ָ��
* @retval       : None
* @brief        : д�Ĵ���
*/
static void vModbus_Write_Register(USART1_t* UART)
{
  USART1_t* const COM = UART;
  uint8_t i;
  
  for (i = 0; i < 8; i++)
  {
    *(COM->pucSend_Buffer + i) = *(COM->pucRec_Buffer + i);
  }
  // ��������
  USART1.vUSART1_SendArray(COM->pucSend_Buffer,8);
  
  // ��ȡ���� 0x4003 = 0x9C43
  if ((*(COM->pucRec_Buffer + 2) == 0x9C) && (*(COM->pucRec_Buffer + 3) == 0x43))
  {
    // ����ռ�ձ�
    Pwm.LED_Duty = (PWM_Duty_t)((*(COM->pucRec_Buffer + 4)) * 256 + (*(COM->pucRec_Buffer + 5)));
    CCR2 = Pwm.LED_Duty;        
  }
  
}