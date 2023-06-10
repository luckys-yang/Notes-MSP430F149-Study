/***************************************************************************
 * File          : key.c
 * Author        : Luckys.
 * Date          : 2023-06-06
 * description   : ����  
****************************************************************************/

#include <main.h>
   
/*====================================variable definition declaration area BEGIN=================================*/
static uint8_t Key_Click_Status = FALSE;        // ����״̬����
/*====================================variable definition declaration area   END=================================*/

/*====================================static function declaration area BEGIN====================================*/
static void Key_Detect(void);   // �������
/*====================================static function declaration area   END====================================*/

Key_t Key = 
{
  FALSE,
  FALSE,
  FALSE,
  FALSE,
  Key_Detect,
};

Key_STA_Machine_t Key_STA_Machine = 
{
  STA1_KEY_UP,
  0,
  0,
  0,
};


static void Key_Detect(void)
{
  if (Key_STA_Machine.usKey_STA_Machine_Scan_Timer >= TimerA_10ms)      // ���10msɨ��
  {
    switch(Key_STA_Machine.ucKey_STA_Machine_Status)    // ����״̬�����-> ���� ˫�� ����
    {
    case STA1_KEY_UP:   // ��������
      {
        // �������
        if ((P1IN & KEY1) == KEY1)      // �ߵ�ƽ
        {
          if (Key_Click_Status == TRUE) // �жϻ��棬ֻ��ǰ���е���������û��⵽˫���볤�� ��������Ч
          {
            if (Key_STA_Machine.usKey_Double_Click_Timer >= Set_Double_TIME)    // ˫����ⳬʱ��������Ч
            {
              Key.Key_Flag = TRUE;
              Key.Key_Click = TRUE;
              Key_Click_Status = FALSE; // �����������
            }
          }
        }
        else    // ��⵽�͵�ƽ ״̬�л������¶���
        {
          Key_STA_Machine.ucKey_STA_Machine_Status = STA2_KEY_D_SHAKE;
          printf("�¼�֪ͨ������״̬ -> ���¶���״̬\r\n");
        }
        break;
      }
    case STA2_KEY_D_SHAKE:      // ���¶���
      {
        if ((P1IN & KEY1) == KEY1)      // ��⵽�ߵ�ƽ����������״̬�л�������״̬
        {
          Key_STA_Machine.ucKey_STA_Machine_Status = STA1_KEY_UP;
        }
        else    // ��⵽�͵�ƽ ȷ�ϰ������£�״̬�ص�����״̬
        {
          Key_STA_Machine.ucKey_STA_Machine_Status = STA3_KEY_DOWN;
          Key_STA_Machine.usKey_Long_Click_Timer = 0;   // ������ⶨʱ����0 ��ʼ��ʱ
          printf("�¼�֪ͨ�����¶���״̬ -> ����״̬\r\n");
        }
        break;
      }
    case STA3_KEY_DOWN: // ��������
      {
        if ((P1IN & KEY1) == KEY1)      // ��⵽�ߵ�ƽ����������״̬�л������𶶶�״̬
        {
          Key_STA_Machine.ucKey_STA_Machine_Status = STA4_KEY_U_SHAKE;
          
          if (Key.Key_Long_Click == FALSE)      // ˫�����
          {
            if (Key_Click_Status == TRUE)
            {
              Key.Key_Flag = TRUE;
              Key.Key_Double_Click = TRUE;
              Key_Click_Status = FALSE; // �����������
            }
            else
            {
              Key_Click_Status = TRUE; // ��������
              Key_STA_Machine.usKey_Double_Click_Timer = 0;     // ˫����ⶨʱ����0 ��ʼ��ʱ
            }
          }
          printf("�¼�֪ͨ������״̬ -> ���𶶶�״̬\r\n");
        }
        else    // ��⵽�͵�ƽ �������ְ��� ���г������
        {
          if (Key.Key_Long_Click == FALSE)      // �������
          {
            if (Key_STA_Machine.usKey_Long_Click_Timer >= Set_Long_TIME)
            {
              Key_STA_Machine.ucKey_STA_Machine_Status = STA4_KEY_U_SHAKE;      // �������³���Set_Press_TIME��״̬�л���STA4���𶶶������ⷴ����ⳤ��
              Key.Key_Flag = TRUE;
              Key.Key_Long_Click = TRUE;
              Key_Click_Status = FALSE; // �����������
              printf("�¼�֪ͨ������״̬ -> ���𶶶�״̬\r\n");
            }
          }
        }
        break;
      }
    case STA4_KEY_U_SHAKE:      // ���𶶶�
      {
        if ((P1IN & KEY1) == KEY1)      // ��⵽�ߵ�ƽ �ص�����״̬
        {
          Key_STA_Machine.ucKey_STA_Machine_Status = STA1_KEY_UP;
          printf("�¼�֪ͨ�����𶶶�״̬ -> ����״̬\r\n");
        }
        else    // ��⵽�͵�ƽ �������� �������ֵ��𶶶�״̬
        {
          
        }
        break;
      }
    default:
      {
        Key_STA_Machine.ucKey_STA_Machine_Status = STA1_KEY_UP;
        break;
      }      
    }
    Key_STA_Machine.usKey_STA_Machine_Scan_Timer = 0;   // ɨ�趨ʱ��0
  }
}


// �����жϺ���
#pragma vector = PORT1_VECTOR
__interrupt void Key1_isr(void)
{
  uint8_t Delay_Cnt;
  
  // KEY1�ж�
  if (KEY1 == (P1IFG & KEY1))
  {
    // ��ʱ����
    Public.vDelay_ms(5);
    if ((P1IN & KEY1) != KEY1)  // ��������Ƿ�Ϊ�͵�ƽ
    {
      for (Delay_Cnt = 0; Delay_Cnt < 200; Delay_Cnt++)
      {
        Public.vDelay_ms(10);
        
        if (KEY1 == (P1IN & KEY1))      // ��ǰ�������˳�
        {
          break;
        }
      }
      if (Delay_Cnt >= 200)     // ����
      {
        
      }
      else      // ����
      {
        if (System_StandBy == System.ucSystem_Status)   // �˳��͹���
        {
          System.ucSystem_Status = System_Run;
          LPM4_EXIT;
        }
      }
    }
    // �����־λ
    P1IFG &= (~KEY1); 
  }
}