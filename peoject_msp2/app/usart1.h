#ifndef __USART1_H
#define __USART1_H
#include <main.h>

// ���պͷ�������ֽ�
#define USART1_Send_LEN 20
#define USART1_Rec_LEN  20

typedef struct
{
  uint8_t volatile ucRec_Flag;  // ���ձ�־λ
  uint8_t volatile ucRec_Cnt;   // ���ռ���
  uint8_t* pucSend_Buffer;      // ���ͻ���ָ��
  uint8_t* pucRec_Buffer;       // ���ջ���ָ��
  
  void (*vUSART1_Init)(void);   // ����1��ʼ��
  void (*vUSART1_SendArray)(uint8_t*, uint16_t);        // ��������
  void (*vUSART1_SendString)(uint8_t*); // �����ַ���
  void (*vUSART1_Protocol)(void);       // �ӿ�Э��
  
}USART1_t;


extern USART1_t USART1;

#endif
