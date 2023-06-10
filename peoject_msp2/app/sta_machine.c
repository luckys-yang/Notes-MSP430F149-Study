/***************************************************************************
 * File          : sta_machine.c
 * Author        : Luckys.
 * Date          : 2023-06-05
 * description   : ×´Ì¬»ú   
****************************************************************************/

#include <main.h>

/*====================================static function declaration area BEGIN====================================*/
static void vFun_STA1(void);
static void vFun_STA2(void);
static void vFun_STA3(void);
static void vFun_STA4(void);
static void vFun_STA5(void);
/*====================================static function declaration area   END====================================*/

STA_Machine_t STA_Machine = 
{
  STA1,
  FALSE,
  
  vFun_STA1,
  vFun_STA2,
  vFun_STA3,
  vFun_STA4,
  vFun_STA5,
};


/*
* @function     : vFun_STA1~5
* @param        : None
* @retval       : None
* @brief        : ×´Ì¬º¯Êý
*/
static void vFun_STA1(void)
{
  Led.vLed_ON(LED1);
  Led.vLed_OFF(LED2);
  Led.vLed_OFF(LED3);
  Led.vLed_OFF(LED4);
}

static void vFun_STA2(void)
{
  Led.vLed_OFF(LED1);
  Led.vLed_ON(LED2);
  Led.vLed_OFF(LED3);
  Led.vLed_OFF(LED4);
}

static void vFun_STA3(void)
{
  Led.vLed_OFF(LED1);
  Led.vLed_OFF(LED2);
  Led.vLed_ON(LED3);
  Led.vLed_OFF(LED4);
}

static void vFun_STA4(void)
{
  Led.vLed_OFF(LED1);
  Led.vLed_OFF(LED2);
  Led.vLed_OFF(LED3);
  Led.vLed_ON(LED4);
}

static void vFun_STA5(void)
{
  Led.vLed_OFF(LED1);
  Led.vLed_OFF(LED2);
  Led.vLed_OFF(LED3);
  Led.vLed_OFF(LED4);
}

