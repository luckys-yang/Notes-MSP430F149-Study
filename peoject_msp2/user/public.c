/***************************************************************************
* File          : public.c
* Author        : Luckys.
* Date          : 2023-06-04
* description   : ͨ��
****************************************************************************/

#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vDelay_ms(uint16_t);  // ms��ʱ����
/*====================================static function declaration area   END====================================*/


Public_t Public = 
{
  vDelay_ms,
};


/*
* @function     : vDelay_ms
* @param        : ms -> ��Ҫ��ʱ��ʱ��
* @retval       : None
* @brief        : ms��ʱ
*/
static void vDelay_ms(uint16_t ms)
{
  uint16_t i,j;
  
  for (i = 0; i < ms; i++)
  {
    for (j = 0; j < 1590; j++); // ʾ��������ʱ��У׼����(ʹ��LED��ת)
  }
}



