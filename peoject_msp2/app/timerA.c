/***************************************************************************
 * File          : timerA.c
 * Author        : Luckys.
 * Date          : 2023-06-05
 * description   : ��ʱ��A 
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
* @brief        : ��ʱ��A��ʼ��
*/
static void vTimerA_Init(void)
{
  // ���ÿ��ƼĴ��� -- SMCLK + 8��Ƶ + ����ģʽΪ���ϼ���
  TACTL = TASSEL_2 + ID_3 + MC_1;
  // ���ò���/�ȽϼĴ��� -- ��������Ϊ5ms
  // ��ʱ���㣺
  // ��ʱ��Aʱ�� = SMCLK/8 = 8000000Hz / 8 = 1000 000Hz
  // Fre = 1 / 0.005s = 200Hz
  // 1000 000Hz / 200Hz = 5000 - 1
  CCR0 = 5000 - 1;
  // ���ò���/�ȽϿ��ƼĴ��� - Ĭ�ϱȽ�ģʽ�� ����CCIE�ж�
  CCTL0 = CCIE;
}

#pragma vector = TIMERA0_VECTOR // ����һ���ж�����
__interrupt void TimerA_isr(void)       // __interrupt ���жϹؼ���
{
  WatchDog.vWatchDog_Feed();    //ι��
  
  if (++TimerA.usMCU_Run_Timer >= TimerA_100ms)
  {
    TimerA.usMCU_Run_Timer = 0;
    // ��λ��ˮ��״̬����־λ
    STA_Machine.ucSTA_Machine_Switch_Flag = TRUE;
  }
  
  Key_STA_Machine.usKey_STA_Machine_Scan_Timer++;       // ����״̬ɨ�趨ʱ��
  Key_STA_Machine.usKey_Double_Click_Timer++;   // KEY1������ⶨʱ��
  Key_STA_Machine.usKey_Long_Click_Timer++;     // KEY1˫����ⶨʱ��
}
