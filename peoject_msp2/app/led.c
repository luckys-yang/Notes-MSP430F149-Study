/***************************************************************************
 * File          : led.c
 * Author        : Luckys.
 * Date          : 2023-06-05
 * description   : LED
****************************************************************************/

#include <main.h>


/*====================================static function declaration area BEGIN====================================*/
static void vLed_ON(Led_Num_t);     // ¥Úø™
static void vLed_OFF(Led_Num_t);    // πÿ±’
static void vLed_Flip(Led_Num_t);   // ∑≠◊™

/*====================================static function declaration area   END====================================*/

Led_t Led = 
{
  vLed_ON,
  vLed_OFF,
  vLed_Flip,
};

/*
* @function     : vLed_ON
* @param        : Led_Num ->LED±‡∫≈
* @retval       : None
* @brief        : LED¥Úø™
*/
static void vLed_ON(Led_Num_t Led_Num)
{
  switch(Led_Num)
  {
  case LED1:
    {
      P5OUT |= BIT1;    // P51÷√1
      break;
    }
  case LED2:
    {
      P5OUT |= BIT2;    // P52÷√1
      break;
    }
  case LED3:
    {
      P5OUT |= BIT3;    // P53÷√1
      break;
    }
  case LED4:
    {
      P5OUT |= BIT4;    // P54÷√1
      break;
    }
  default:
    {
      P5OUT &= (~(BIT1 + BIT2 + BIT3 + BIT4));
      break;    
    }
  }
}

/*
* @function     : vLed_OFF
* @param        : Led_Num ->LED±‡∫≈
* @retval       : None
* @brief        : LEDπÿ±’
*/
static void vLed_OFF(Led_Num_t Led_Num)
{
  switch(Led_Num)
  {
  case LED1:
    {
      P5OUT &= (~BIT1);    // P51÷√0
      break;
    }
  case LED2:
    {
      P5OUT &= (~BIT2);    // P52÷√0
      break;
    }
  case LED3:
    {
      P5OUT &= (~BIT3);    // P53÷√0
      break;
    }
  case LED4:
    {
      P5OUT &= (~BIT4);    // P54÷√0
      break;
    }
  default:
    {
      P5OUT &= (~(BIT1 + BIT2 + BIT3 + BIT4));
      break;    
    }
  }
}

/*
* @function     : vLed_Flip
* @param        : Led_Num ->LED±‡∫≈
* @retval       : None
* @brief        : LED∑≠◊™
*/
static void vLed_Flip(Led_Num_t Led_Num)
{
  switch(Led_Num)
  {
  case LED1:
    {
      P5OUT ^= BIT1;    // P51÷√∑¥
      break;
    }
  case LED2:
    {
      P5OUT ^= BIT2;    // P52÷√∑¥
      break;
    }
  case LED3:
    {
      P5OUT ^= BIT3;    // P53÷√∑¥
      break;
    }
  case LED4:
    {
      P5OUT ^= BIT4;    // P54÷√∑¥
      break;
    }
  default:
    {
      P5OUT &= (~(BIT1 + BIT2 + BIT3 + BIT4));
      break;    
    }
  }
}
