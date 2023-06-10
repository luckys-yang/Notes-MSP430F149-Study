/***************************************************************************
 * File          : system.c
 * Author        : Luckys.
 * Date          : 2023-06-05
 * description   : 系统运行函数
****************************************************************************/

#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vRun(void); // 系统运行
static void vStandBy(void); // 系统待机
static void vError_Handle(void);  // 错误处理

static void StandBy_Enter_Config(void); // 进入待机配置
static void StandBy_Exit_Config(void);  // 退出待机配置
/*====================================static function declaration area   END====================================*/

System_t System = 
{
  System_Run,
  ErrorCode_NoError,
  
  vRun,
  vStandBy,
  vError_Handle,
};

/*
* @function     : vRun
* @param        : None
* @retval       : None
* @brief        : 系统运行
*/
static void vRun(void)
{
  uint16_t Temp_uint  = 0;
  uint16_t i;
  
  //采集电池与太阳能板电压
  ADC.vADC_Get_BAT_Voltage();
  ADC.vADC_Get_VIN_Voltage();
   
  //OLED显示太阳能板电压
  Temp_uint = (uint16_t)(ADC.fVIN_VOltage * 10);
  OLED.vOLED_Show_Char(2,64,Temp_uint / 100 + '0',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(2,72,Temp_uint % 100 / 10 + '0',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(2,80,'.',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(2,88,Temp_uint % 10 + '0',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(2,96,'V',ASCII_SIZE_16);
  
  //OLED显示电池电压
  Temp_uint = (uint16_t)(ADC.fBAT_Voltage * 10);
  OLED.vOLED_Show_Char(4,72,Temp_uint / 10 + '0',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(4,80,'.',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(4,88,Temp_uint % 10 + '0',ASCII_SIZE_16);
  OLED.vOLED_Show_Char(4,96,'V',ASCII_SIZE_16);
  
  // 充电指示
  Power.vCharging_Indicate();
  //间隔500ms采集一次
  for (i = 0; i < 500; i++)
  {
    Public.vDelay_ms(1);
    Key.Key_Detect();     // 按键扫描
    if (Key.Key_Flag == TRUE)
    {
      break;
    }
  }

  if (Key.Key_Flag == TRUE)
  {
    if (Key.Key_Click == TRUE)  // 单击
    {
      printf("系统进入低功耗模式\r\n");
      System.ucSystem_Status = System_StandBy;
    }
    // 清除KEY所有标志位
    Key.Key_Flag = FALSE;
    Key.Key_Click = FALSE;
  }
}

/*
* @function     : vStandBy
* @param        : None
* @retval       : None
* @brief        : 系统待机
*/
static void vStandBy(void)
{
  StandBy_Enter_Config();       // 进入
  
  // 进入低功耗LPM3
  WDTCTL = WDTPW + WDTHOLD;     // 关闭看门狗,避免待机时系统重启
  TBR = 0;      // 当前计数值清0保证固定时间唤醒
  LPM3;
  StandBy_Exit_Config();        // 退出
}

/*
* @function     : vError_Handle
* @param        : None
* @retval       : None
* @brief        : 错误处理
*/
static void vError_Handle(void)
{
  
}

/*
* @function     : StandBy_Enter_Config
* @param        : None
* @retval       : None
* @brief        : 进入待机配置
*/
static void StandBy_Enter_Config(void)
{
  // 关闭LED
  Led.vLed_OFF(LED1);
  Led.vLed_OFF(LED2);
  Led.vLed_OFF(LED3);
  Led.vLed_OFF(LED4);
  
  Power.vBoost_5V_Control(PWR_OFF);     // 关闭升压电源
  Power.vPeripheral_3V3_Control(PWR_OFF);       // 关闭3.3v外设电源
  Pwm.vLED_Power_Control(LED_Power_OFF);        // 关闭大功率LED电源
  BAT_MOS_CLOSE;        // 关闭VAT电压采样电路
  // 管脚配置
  P3SEL &= (~BIT6);     // P36是串口1TX设置为GPIO口输出低电平
  P3OUT &= (~BIT6);
  // ADC设置
  ADC12CTL0 &= (~ENC);
  ADC12CTL0 &= (~REFON);        // 关闭基准电压
}

/*
* @function     : StandBy_Exit_Config
* @param        : None
* @retval       : None
* @brief        : 退出待机配置
*/
static void StandBy_Exit_Config(void)
{
  Power.vBoost_5V_Control(PWR_ON);     // 恢复升压电源
  Power.vPeripheral_3V3_Control(PWR_ON);       // 恢复3.3v外设电源  
  BAT_MOS_OPEN; // 恢复
  Pwm.vLED_Power_Control(LED_Power_ON);        // 打开大功率LED电源
  ADC.vADC_Init();
  // 管脚配置
  P3SEL |= BIT6;        // 恢复串口1TX
  Public.vDelay_ms(10); // 延时10ms稳定电源
}

