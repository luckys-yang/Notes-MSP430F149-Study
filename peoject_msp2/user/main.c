/***************************************************************************
* File          : main.c
* Author        : Luckys.
* Date          : 2023-06-04
* description   : ������ 
****************************************************************************/
#include <main.h>

int main( void )
{
  Hardware_Init.vSys_Init(); // ϵͳ��ʼ��
  // ��ѭ��
  while(1)
  {
    // ��Ҫ����
    if (System_Run == System.ucSystem_Status)
    {
      System.vRun();  // ϵͳ����
    }
    else
    {
      System.vStandBy();  // ϵͳ����
    }
  }
}


















