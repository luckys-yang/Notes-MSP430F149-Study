/***************************************************************************
* File          : public.c
* Author        : Luckys.
* Date          : 2023-06-04
* description   : 通用
****************************************************************************/

#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vDelay_ms(uint16_t);  // ms延时函数
/*====================================static function declaration area   END====================================*/


Public_t Public = 
{
  vDelay_ms,
};


/*
* @function     : vDelay_ms
* @param        : ms -> 需要延时的时间
* @retval       : None
* @brief        : ms延时
*/
static void vDelay_ms(uint16_t ms)
{
  uint16_t i,j;
  
  for (i = 0; i < ms; i++)
  {
    for (j = 0; j < 1590; j++); // 示波器测量时间校准即可(使用LED翻转)
  }
}



