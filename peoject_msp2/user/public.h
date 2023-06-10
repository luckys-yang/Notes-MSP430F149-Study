#ifndef __PUBLIC_H
#define __PUBLIC_H
#include <main.h>

// ���������ض���
typedef signed char        sint8_t;
typedef signed short int   sint16_t;
typedef signed long        sint32_t;

typedef unsigned char      uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned long      uint32_t;

// ����ö������
typedef enum
{
  TRUE = (uint8_t)1,
  FALSE = (uint8_t)0,
}BOOL_t;

typedef struct
{
  void (*vDelay_ms)(uint16_t); // ms��ʱ����
}Public_t;


extern Public_t Public;

#endif

