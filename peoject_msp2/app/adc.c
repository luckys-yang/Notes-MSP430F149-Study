/***************************************************************************
 * File          : adc.c
 * Author        : Luckys.
 * Date          : 2023-06-07
 * description   : ADC  
****************************************************************************/

#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vADC_Init(void);    // ADC初始化
static void vADC_Get_BAT_Voltage(void); // 获取电池电压
static void vADC_Get_VIN_Voltage(void); // 获取输入电压
/*====================================static function declaration area   END====================================*/

ADC_t ADC = 
{
  3.7,
    0,
  vADC_Init,
  vADC_Get_BAT_Voltage,
  vADC_Get_VIN_Voltage,
};

/*
* @function     : vADC_Init
* @param        : None
* @retval       : None
* @brief        : ADC初始化
*/
static void vADC_Init(void)
{
  P6SEL |= ADC_BAT_PIN + ADC_VIN_PIN;   // 开启复用
  // 配置ADC12 控制0寄存器--- 采样周期256 + 参考内部电压2.5V + 使能2.5V + 使能ADC12
  ADC12CTL0 = SHT0_8 + REF2_5V + REFON + ADC12ON;
}

/*
* @function     : vADC_Get_BAT_Voltage
* @param        : None
* @retval       : None
* @brief        : 获取电池电压
*/
static void vADC_Get_BAT_Voltage(void)
{
  // 打开MOS管
  BAT_MOS_OPEN;
  Public.vDelay_ms(1);
  printf("采集电池电压：\r\n");
  // 更改ADC寄存器前需要把ENC清0
  ADC12CTL0 &= (~ENC);
  // 脉冲采样模式 SHP = 1
  // 转换模式选择单通道，单次转换 CONSEQ = 00  
  // 单次转换内存寄存器选择5 
  // ADC12时钟默认选择ADC12OSC，大概5MHz, 不分频
  // 电压采样与保持时钟源选择软件控制 - ADC12SC
  ADC12CTL1 = CSTARTADD_5 + SHP + CONSEQ_0;
  // 参考电压VR+ = VREF+， VR- = VREF-
  // 选择输入通道5---跟硬件相关，因为BAT是连接A5的 
  ADC12MCTL5 = SREF_5 + INCH_BAT;
  // 使能转换
  ADC12CTL0 |= ENC;
  
  // 开始转换
  ADC12CTL0 |= ADC12SC;
  // 等待转换结束(标志位置1)--通过判断中断标志位尽管我们没有打开中断但是还是能通过它来查询是否转换完
  while ((ADC12IFG & ADC_BAT_PIN) != ADC_BAT_PIN);
  // 计算电压
  ADC.fBAT_Voltage = (ADC12MEM5 * 2.5) / 4095;
  ADC.fBAT_Voltage *= 2;
  // 打印
  printf("VBAT: %.2fV\r\n",ADC.fBAT_Voltage);
  // 关闭MOS管
  BAT_MOS_CLOSE;  
}

/*
* @function     : vADC_Get_VIN_Voltage
* @param        : None
* @retval       : None
* @brief        : 获取输入电压
*/
static void vADC_Get_VIN_Voltage(void)
{
  printf("采集太阳能电压：\r\n");
  // 更改ADC寄存器前需要把ENC清0
  ADC12CTL0 &= (~ENC);
  // 脉冲采样模式 SHP = 1
  // 转换模式选择单通道，单次转换 CONSEQ = 00  
  // 单次转换内存寄存器选择6 (可以是其他比如5,4,3,2,1,不是强制是6)
  // ADC12时钟默认选择ADC12OSC，大概5MHz, 不分频
  // 电压采样与保持时钟源选择软件控制 - ADC12SC
  ADC12CTL1 = CSTARTADD_6 + SHP + CONSEQ_0;
  // 参考电压VR+ = VREF+， VR- = VREF-
  // 选择输入通道6---跟硬件相关，因为BAT是连接A5的 
  ADC12MCTL6 = SREF_5 + INCH_VIN;
  // 使能转换
  ADC12CTL0 |= ENC;
  
  // 开始转换
  ADC12CTL0 |= ADC12SC;
  // 等待转换结束(标志位置1)--通过判断中断标志位尽管我们没有打开中断但是还是能通过它来查询是否转换完
  while ((ADC12IFG & ADC_VIN_PIN) != ADC_VIN_PIN);
  // 计算电压
  ADC.fVIN_VOltage = (ADC12MEM6 * 2.5) / 4095;
  ADC.fVIN_VOltage *= 9.2;      // 最大输入 9.2*2.5V=23V
  // 打印
  printf("VIN: %.2fV\r\n",ADC.fVIN_VOltage);
}
