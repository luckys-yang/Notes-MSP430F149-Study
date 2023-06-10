/***************************************************************************
 * File          : key.c
 * Author        : Luckys.
 * Date          : 2023-06-06
 * description   : 按键  
****************************************************************************/

#include <main.h>
   
/*====================================variable definition declaration area BEGIN=================================*/
static uint8_t Key_Click_Status = FALSE;        // 单击状态缓存
/*====================================variable definition declaration area   END=================================*/

/*====================================static function declaration area BEGIN====================================*/
static void Key_Detect(void);   // 按键检测
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
  if (Key_STA_Machine.usKey_STA_Machine_Scan_Timer >= TimerA_10ms)      // 间隔10ms扫描
  {
    switch(Key_STA_Machine.ucKey_STA_Machine_Status)    // 运行状态机检测-> 单击 双击 长按
    {
    case STA1_KEY_UP:   // 按键弹起
      {
        // 单击检测
        if ((P1IN & KEY1) == KEY1)      // 高电平
        {
          if (Key_Click_Status == TRUE) // 判断缓存，只有前面有单击，而且没检测到双击与长按 单击才有效
          {
            if (Key_STA_Machine.usKey_Double_Click_Timer >= Set_Double_TIME)    // 双击检测超时，单击有效
            {
              Key.Key_Flag = TRUE;
              Key.Key_Click = TRUE;
              Key_Click_Status = FALSE; // 清除单击缓存
            }
          }
        }
        else    // 检测到低电平 状态切换到按下抖动
        {
          Key_STA_Machine.ucKey_STA_Machine_Status = STA2_KEY_D_SHAKE;
          printf("事件通知：弹起状态 -> 按下抖动状态\r\n");
        }
        break;
      }
    case STA2_KEY_D_SHAKE:      // 按下抖动
      {
        if ((P1IN & KEY1) == KEY1)      // 检测到高电平，抖动引起，状态切换到弹起状态
        {
          Key_STA_Machine.ucKey_STA_Machine_Status = STA1_KEY_UP;
        }
        else    // 检测到低电平 确认按键按下，状态回到按下状态
        {
          Key_STA_Machine.ucKey_STA_Machine_Status = STA3_KEY_DOWN;
          Key_STA_Machine.usKey_Long_Click_Timer = 0;   // 长按检测定时器清0 开始计时
          printf("事件通知：按下抖动状态 -> 按下状态\r\n");
        }
        break;
      }
    case STA3_KEY_DOWN: // 按键按下
      {
        if ((P1IN & KEY1) == KEY1)      // 检测到高电平，抖动引起，状态切换到弹起抖动状态
        {
          Key_STA_Machine.ucKey_STA_Machine_Status = STA4_KEY_U_SHAKE;
          
          if (Key.Key_Long_Click == FALSE)      // 双击检测
          {
            if (Key_Click_Status == TRUE)
            {
              Key.Key_Flag = TRUE;
              Key.Key_Double_Click = TRUE;
              Key_Click_Status = FALSE; // 清除单击缓存
            }
            else
            {
              Key_Click_Status = TRUE; // 单击缓存
              Key_STA_Machine.usKey_Double_Click_Timer = 0;     // 双击检测定时器清0 开始定时
            }
          }
          printf("事件通知：按下状态 -> 弹起抖动状态\r\n");
        }
        else    // 检测到低电平 按键保持按下 进行长按检测
        {
          if (Key.Key_Long_Click == FALSE)      // 长按检测
          {
            if (Key_STA_Machine.usKey_Long_Click_Timer >= Set_Long_TIME)
            {
              Key_STA_Machine.ucKey_STA_Machine_Status = STA4_KEY_U_SHAKE;      // 按键按下超过Set_Press_TIME，状态切换至STA4弹起抖动，避免反复检测长按
              Key.Key_Flag = TRUE;
              Key.Key_Long_Click = TRUE;
              Key_Click_Status = FALSE; // 清除单击缓存
              printf("事件通知：按下状态 -> 弹起抖动状态\r\n");
            }
          }
        }
        break;
      }
    case STA4_KEY_U_SHAKE:      // 弹起抖动
      {
        if ((P1IN & KEY1) == KEY1)      // 检测到高电平 回到弹起状态
        {
          Key_STA_Machine.ucKey_STA_Machine_Status = STA1_KEY_UP;
          printf("事件通知：弹起抖动状态 -> 弹起状态\r\n");
        }
        else    // 检测到低电平 抖动引起 按键保持弹起抖动状态
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
    Key_STA_Machine.usKey_STA_Machine_Scan_Timer = 0;   // 扫描定时清0
  }
}


// 按键中断函数
#pragma vector = PORT1_VECTOR
__interrupt void Key1_isr(void)
{
  uint8_t Delay_Cnt;
  
  // KEY1中断
  if (KEY1 == (P1IFG & KEY1))
  {
    // 延时消抖
    Public.vDelay_ms(5);
    if ((P1IN & KEY1) != KEY1)  // 检测输入是否为低电平
    {
      for (Delay_Cnt = 0; Delay_Cnt < 200; Delay_Cnt++)
      {
        Public.vDelay_ms(10);
        
        if (KEY1 == (P1IN & KEY1))      // 提前弹起则退出
        {
          break;
        }
      }
      if (Delay_Cnt >= 200)     // 长按
      {
        
      }
      else      // 单击
      {
        if (System_StandBy == System.ucSystem_Status)   // 退出低功耗
        {
          System.ucSystem_Status = System_Run;
          LPM4_EXIT;
        }
      }
    }
    // 清除标志位
    P1IFG &= (~KEY1); 
  }
}