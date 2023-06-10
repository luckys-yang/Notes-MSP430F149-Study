/***************************************************************************
 * File          : power.c
 * Author        : Luckys.
 * Date          : 2023-06-09
 * description   : 电源  
****************************************************************************/


#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vBoost_5V_Control(PWR_t);     // 升压电源控制
static void vPeripheral_3V3_Control(PWR_t);       // 外设电源控制
static void vCharging_Indicate(void);     // 充电指示
/*====================================static function declaration area   END====================================*/


Power_t Power = 
{
  Charge_Status_Stop,
  PWR_Status_ON,
  PWR_Status_ON,
  vBoost_5V_Control,
  vPeripheral_3V3_Control,
  vCharging_Indicate,
};

/*
* @function     : vBoost_5V_Control
* @param        : PWR_Status -> 电源状态
* @retval       : None
* @brief        : 升压电源控制(P50管脚)
*/
static void vBoost_5V_Control(PWR_t PWR_Status)
{
  if (PWR_ON == PWR_Status)
  {
    P5OUT |= BIT0;      // 使能升压芯片--ME2159
    Power.Boost_5V_Status = PWR_Status_ON;
  }
  else
  {
    P5OUT &= (~BIT0);      // 失能升压芯片--ME2159
    Power.Boost_5V_Status = PWR_Status_OFF;    
  }
}

/*
* @function     : vPeripheral_3V3_Control
* @param        : None
* @retval       : None
* @brief        : 外设电源控制
*/
static void vPeripheral_3V3_Control(PWR_t PWR_Status)
{
  if (PWR_ON == PWR_Status)
  {
    P6OUT &= (~BIT3);   // 打开PMOS管
    Power.Peripheral_3V3_Status = PWR_Status_ON;
    P5DIR |= (BIT5 + BIT6);     // 恢复SDA SCL 管脚方向为输出
    OLED.vOLED_Init();  // OLED初始化
  }
  else
  {
    P6OUT |= BIT3;   // 关闭PMOS管
    Power.Peripheral_3V3_Status = PWR_Status_OFF;
    P5DIR &= (~(BIT5 + BIT6));  // 把SDA SCL 管脚设为输入
  }
}

/*
* @function     : vCharging_Indicate
* @param        : None
* @retval       : None
* @brief        : 升压电源控制
*/
static void vCharging_Indicate(void)
{
  //正在充电 - CHAG为低电平
  if((P1IN & BIT0) != BIT0)
  {
    OLED.vOLED_Show_CHN(6,64,"正");
    OLED.vOLED_Show_CHN(6,80,"在");
  }
  //完成充电 - DONE为低电平
  else if((P1IN & BIT5) != BIT5)
  {
    OLED.vOLED_Show_CHN(6,64,"完");
    OLED.vOLED_Show_CHN(6,80,"成");
  }
  //停止充电 - CHAG，DONE都为高电平，太阳能输入电压过低，CN3791没有工作
  else
  {
    OLED.vOLED_Show_CHN(6,64,"停");
    OLED.vOLED_Show_CHN(6,80,"止");
  }
}
