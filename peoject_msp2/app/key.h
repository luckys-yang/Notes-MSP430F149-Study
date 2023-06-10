#ifndef __KEY_H
#define __KEY_H
#include <main.h>


// ����������P13
#define KEY1    BIT3
// �趨����ʱ��
#define Set_Long_TIME   TimerA_2s
// �趨˫��ʱ��
#define Set_Double_TIME TimerA_200ms

typedef struct
{
  uint8_t Key_Flag;     // ���� ��־λ
  uint8_t Key_Click;    // ����
  uint8_t Key_Double_Click;     // ˫��
  uint8_t Key_Long_Click;       // ����
  
  void (*Key_Detect)(void);     // �������
}Key_t;

typedef enum
{
  STA1_KEY_UP = (uint8_t)0x01,  // ��������
  STA2_KEY_D_SHAKE = (uint8_t)0x02,       // ���¶���
  STA3_KEY_DOWN = (uint8_t)0x03,        // ��������
  STA4_KEY_U_SHAKE = (uint8_t)0x04,     // ���𶶶�
}Key_STA_Machine_Status_t;

typedef struct
{
  Key_STA_Machine_Status_t ucKey_STA_Machine_Status;    // ����״̬��״̬
  uint16_t volatile usKey_STA_Machine_Scan_Timer;       // ״̬��ɨ�趨ʱ��
  uint16_t volatile usKey_Double_Click_Timer;   // ˫����ʱ��
  uint16_t volatile usKey_Long_Click_Timer;     // ������ʱ��
}Key_STA_Machine_t;

extern Key_t Key;
extern Key_STA_Machine_t Key_STA_Machine;

#endif
