/***************************************************************************
 * File          : watch_dog.c
 * Author        : Luckys.
 * Date          : 2023-06-06
 * description   : ���Ź� 
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
* @brief        : ���Ź���ʼ��
*/
static void vWatchDog_Init(void)
{
  WDTCTL = WDTPW + WDTHOLD;     // �л�ʱ��Դǰ�ȹرտ��Ź�
  // ʱ��Դѡ��ACLK,���Ź�ʱ��Ϊ250ms
  WDTCTL = WDT_ARST_250;
}

/*
* @function     : vWatchDog_Feed
* @param        : None
* @retval       : None
* @brief        : ι��
*/
static void vWatchDog_Feed(void)
{
  WDTCTL = WDT_ARST_250;
}

