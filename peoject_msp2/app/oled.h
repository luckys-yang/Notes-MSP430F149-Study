#ifndef __OLED_H
#define __OLED_H
#include <main.h>

// OLED的IIC地址(SA0 = 0) --- 0111 1000
#define OLED_ADDR       0x78
// OLED参数(宽度，高度，页数量)
#define OLED_WIDTH      128
#define OLED_HEIGHT     64
#define OLED_PAGE_MAX   8
// IIC相关 --- SCL
#define SET_SCL P5OUT |= BIT6   // SCL输出高电平
#define CLR_SCL P5OUT &= (~BIT6)        // SCL输出低电平
// IIC相关 --- SDA
#define SDA_IN          P5DIR &= (~BIT5)        // SDA设为输入
#define READ_SDA        ((P5IN >> 5) & BIT0)    // 读取SDA输入
#define SDA_OUT         P5DIR |= BIT5   // SDA设为输出
#define SET_SDA         P5OUT |= BIT5   // SDA输出高电平
#define CLR_SDA         P5OUT &= (~BIT5)        // SDA输出低电平
  

typedef enum
{
  ASCII_SIZE_8 = 8,
  ASCII_SIZE_16 = 16,
  ASCII_SIZE_24 = 24,
}ASCII_Size_t;

typedef struct
{
  void (*vOLED_Init)(void);     // OLED初始化
  void (*vOLED_Clear)(void);    // 清屏
  void (*vOLED_Show_Char)(uint8_t, uint8_t, uint8_t, ASCII_Size_t);        // OLED显示字符
  void (*vOLED_Show_String)(uint8_t, uint8_t, const char*, ASCII_Size_t);       // OLED显示字符串
  void (*vOLED_Show_CHN)(uint8_t, uint8_t, const char*);        // OLED显示汉字
}OLED_t;

extern OLED_t OLED;

#endif
