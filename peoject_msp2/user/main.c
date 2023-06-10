/***************************************************************************
* File          : main.c
* Author        : Luckys.
* Date          : 2023-06-04
* description   : 主函数 
****************************************************************************/
#include <main.h>

int main( void )
{
  Hardware_Init.vSys_Init(); // 系统初始化
  // 主循环
  while(1)
  {
    // 主要任务
    if (System_Run == System.ucSystem_Status)
    {
      System.vRun();  // 系统运行
    }
    else
    {
      System.vStandBy();  // 系统待机
    }
  }
}


















