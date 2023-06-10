/***************************************************************************
 * File          : timerB.c
 * Author        : Luckys.
 * Date          : 2023-06-09
 * description   : 定时器B
****************************************************************************/

#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vTimerB_Init(void);
/*====================================static function declaration area   END====================================*/


TimerB_t TimerB = 
{
  vTimerB_Init,
};

/*
* @function     : vTimerB_Init
* @param        : None
* @retval       : None
* @brief        : 定时器初始化
*/
static void vTimerB_Init(void)
{
  // 设置控制寄存器 - ACLK + 8分频 + 向上计数模式
  TBCTL = TASSEL_1 + ID_3 + MC_1;
  // 设置捕获/ 比较寄存器 - 设置周期10s
  // 定时计算：
  // 定时器B时钟 = ACLK/8 = 32768Hz / 8 = 4096Hz
  // Fre = 1 / 10s = 0.1Hz
  // 4096Hz / 0.1Hz = 40960 - 1  
  TBCCR0 = 40960 - 1;
  // 设置捕获/比较控制寄存器 - 默认比较模式， 开启CCIE中断
  TBCCTL0 = CCIE;
}

// 定时器中断函数---10s
#pragma vector = TIMERB0_VECTOR
__interrupt void TimerB_isr(void)
{
  if (System_StandBy == System.ucSystem_Status)
  {
    System.ucSystem_Status = System_Run;
    LPM3_EXIT;  // 退出低功耗
  }
}