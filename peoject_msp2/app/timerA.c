/***************************************************************************
 * File          : timerA.c
 * Author        : Luckys.
 * Date          : 2023-06-05
 * description   : 定时器A 
****************************************************************************/

#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vTimerA_Init(void);
/*====================================static function declaration area   END====================================*/

TimerA_t TimerA = 
{
  0,
  0,
  vTimerA_Init,
};

/*
* @function     : vTimerA_Init
* @param        : None
* @retval       : None
* @brief        : 定时器A初始化
*/
static void vTimerA_Init(void)
{
  // 设置控制寄存器 -- SMCLK + 8分频 + 工作模式为向上计数
  TACTL = TASSEL_2 + ID_3 + MC_1;
  // 设置捕获/比较寄存器 -- 设置周期为5ms
  // 定时计算：
  // 定时器A时钟 = SMCLK/8 = 8000000Hz / 8 = 1000 000Hz
  // Fre = 1 / 0.005s = 200Hz
  // 1000 000Hz / 200Hz = 5000 - 1
  CCR0 = 5000 - 1;
  // 设置捕获/比较控制寄存器 - 默认比较模式， 开启CCIE中断
  CCTL0 = CCIE;
}

#pragma vector = TIMERA0_VECTOR // 定义一下中断向量
__interrupt void TimerA_isr(void)       // __interrupt 是中断关键字
{
  WatchDog.vWatchDog_Feed();    //喂狗
  
  if (++TimerA.usMCU_Run_Timer >= TimerA_100ms)
  {
    TimerA.usMCU_Run_Timer = 0;
    // 置位流水灯状态机标志位
    STA_Machine.ucSTA_Machine_Switch_Flag = TRUE;
  }
  
  Key_STA_Machine.usKey_STA_Machine_Scan_Timer++;       // 按键状态扫描定时器
  Key_STA_Machine.usKey_Double_Click_Timer++;   // KEY1长按检测定时器
  Key_STA_Machine.usKey_Long_Click_Timer++;     // KEY1双击检测定时器
}
