/***************************************************************************
 * File          : timerB.c
 * Author        : Luckys.
 * Date          : 2023-06-09
 * description   : ��ʱ��B
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
* @brief        : ��ʱ����ʼ��
*/
static void vTimerB_Init(void)
{
  // ���ÿ��ƼĴ��� - ACLK + 8��Ƶ + ���ϼ���ģʽ
  TBCTL = TASSEL_1 + ID_3 + MC_1;
  // ���ò���/ �ȽϼĴ��� - ��������10s
  // ��ʱ���㣺
  // ��ʱ��Bʱ�� = ACLK/8 = 32768Hz / 8 = 4096Hz
  // Fre = 1 / 10s = 0.1Hz
  // 4096Hz / 0.1Hz = 40960 - 1  
  TBCCR0 = 40960 - 1;
  // ���ò���/�ȽϿ��ƼĴ��� - Ĭ�ϱȽ�ģʽ�� ����CCIE�ж�
  TBCCTL0 = CCIE;
}

// ��ʱ���жϺ���---10s
#pragma vector = TIMERB0_VECTOR
__interrupt void TimerB_isr(void)
{
  if (System_StandBy == System.ucSystem_Status)
  {
    System.ucSystem_Status = System_Run;
    LPM3_EXIT;  // �˳��͹���
  }
}