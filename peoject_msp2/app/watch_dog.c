/***************************************************************************
 * File          : watch_dog.c
 * Author        : Luckys.
 * Date          : 2023-06-06
 * description   : 看门狗 
****************************************************************************/

#include <main.h>


/*====================================static function declaration area BEGIN====================================*/
static void vWatchDog_Init(void);
static void vWatchDog_Feed(void);
/*====================================static function declaration area   END====================================*/

WatchDog_t WatchDog = 
{
  vWatchDog_Init,
  vWatchDog_Feed,
};

/*
* @function     : vWatchDog_Init
* @param        : None
* @retval       : None
* @brief        : 看门狗初始化
*/
static void vWatchDog_Init(void)
{
  WDTCTL = WDTPW + WDTHOLD;     // 切换时钟源前先关闭看门狗
  // 时钟源选择ACLK,看门狗时间为250ms
  WDTCTL = WDT_ARST_250;
}

/*
* @function     : vWatchDog_Feed
* @param        : None
* @retval       : None
* @brief        : 喂狗
*/
static void vWatchDog_Feed(void)
{
  WDTCTL = WDT_ARST_250;
}

