#ifndef __OLED_H
#define __OLED_H
#include <main.h>

// OLED��IIC��ַ(SA0 = 0) --- 0111 1000
#define OLED_ADDR       0x78
// OLED����(��ȣ��߶ȣ�ҳ����)
#define OLED_WIDTH      128
#define OLED_HEIGHT     64
#define OLED_PAGE_MAX   8
// IIC��� --- SCL
#define SET_SCL P5OUT |= BIT6   // SCL����ߵ�ƽ
#define CLR_SCL P5OUT &= (~BIT6)        // SCL����͵�ƽ
// IIC��� --- SDA
#define SDA_IN          P5DIR &= (~BIT5)        // SDA��Ϊ����
#define READ_SDA        ((P5IN >> 5) & BIT0)    // ��ȡSDA����
#define SDA_OUT         P5DIR |= BIT5   // SDA��Ϊ���
#define SET_SDA         P5OUT |= BIT5   // SDA����ߵ�ƽ
#define CLR_SDA         P5OUT &= (~BIT5)        // SDA����͵�ƽ
  

typedef enum
{
  ASCII_SIZE_8 = 8,
  ASCII_SIZE_16 = 16,
  ASCII_SIZE_24 = 24,
}ASCII_Size_t;

typedef struct
{
  void (*vOLED_Init)(void);     // OLED��ʼ��
  void (*vOLED_Clear)(void);    // ����
  void (*vOLED_Show_Char)(uint8_t, uint8_t, uint8_t, ASCII_Size_t);        // OLED��ʾ�ַ�
  void (*vOLED_Show_String)(uint8_t, uint8_t, const char*, ASCII_Size_t);       // OLED��ʾ�ַ���
  void (*vOLED_Show_CHN)(uint8_t, uint8_t, const char*);        // OLED��ʾ����
}OLED_t;

extern OLED_t OLED;

#endif
