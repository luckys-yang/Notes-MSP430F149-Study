#ifndef __ADC_H
#define __ADC_H
#include <main.h>

// 打开/关闭(置1/0)BAT采集电路的MOS
#define BAT_MOS_OPEN    (P6OUT |= BIT4)
#define BAT_MOS_CLOSE   (P6OUT &= (~BIT4))

// 引脚定义 P65 P66
#define ADC_BAT_PIN     BIT5
#define ADC_VIN_PIN     BIT6


typedef enum
{
  INCH_BAT = INCH_5,    // 输入通道5
  INCH_VIN = INCH_6,    // 输入通道6
}ADC_CHANNEL_t;

typedef struct
{
  float fBAT_Voltage;   // 电池电压 初始化默认3.7V
  float fVIN_VOltage;   // 输入电压
  
  void (*vADC_Init)(void);      // ADC初始化
  void (*vADC_Get_BAT_Voltage)(void);       // 获取电池电压
  void (*vADC_Get_VIN_Voltage)(void);       // 获取输入电压
}ADC_t;

extern ADC_t ADC;

#endif
