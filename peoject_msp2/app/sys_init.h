#ifndef __SYS_INIT_H
#define __SYS_INIT_H
#include <main.h>

typedef struct
{
  void (*vCLK_Init)(void);  // ʱ�ӳ�ʼ��
  void (*vGPIO_Init)(void); // ͨ����������˿ڳ�ʼ��
  void (*vIE_Init)(void); // �жϳ�ʼ��
  void (*vPower_On_Indication)(void); // �ϵ�ָʾ
  void (*vSys_Init)(void); // ϵͳ��ʼ��
}Hardware_Init_t;

extern Hardware_Init_t Hardware_Init;

#endif

