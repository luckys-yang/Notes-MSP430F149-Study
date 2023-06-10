/***************************************************************************
* File          : sys_init.c
* Author        : Luckys.
* Date          : 2023-06-04
* description   : 系统初始化  
****************************************************************************/

#include <main.h>
/*====================================static function declaration area BEGIN====================================*/
static void vCLK_Init(void);  // 时钟初始化
static void vGPIO_Init(void); // 通用输入输出端口初始化
static void vIE_Init(void); // 中断初始化
static void vPower_On_Indication(void); // 上电指示
static void vSys_Init(void); // 系统初始化
/*====================================static function declaration area   END====================================*/

Hardware_Init_t Hardware_Init = 
{
  vCLK_Init,
  vGPIO_Init,
  vIE_Init,
  vPower_On_Indication,
  vSys_Init,
};

/*
* @function     : vCLK_Init
* @param        : None
* @retval       : None
* @brief        : 时钟初始化
*/
static void vCLK_Init(void)
{
  uint8_t i;
  uint8_t Error_Cnt = 0;
  
  // MCLK  -> 主系统时钟，CPU使用
  // SMCLK -> 子系统时钟，主要给高速外设使用
  // ACLK  -> 辅助系统，主要给低速外设使用
  
  // 系统复位后，时钟默认情况如下：
  // MCLK与SMCLK默认使用DCO，频率为800kHz；
  // ACLK使用LFXT1，频率为32.768KH;
  
  // 运行时钟
  // MCLK = SMCLK = XT2 = 8MHz
  // ACLK = LFXT1 = 32.768KHZ
  BCSCTL1 &= ~XT2OFF;  // 打开XT2高频晶体振荡器
  do
  {
    IFG1 &= ~OFIFG;     // 清除振荡器故障标志位
    for (i = 0; i < 100; i++);  // 至少等待50个周期--50us

    if(Error_Cnt++ >= 100)        // 超时退出
    {
      break;
    }
  }
  while (IFG1&OFIFG);   // 检测荡器故障标志位，为0时退出循环
  
  if (Error_Cnt < 100)  // 说明晶体正常，上面语句是正常退出
  {
    BCSCTL2 |= SELM_2 + SELS;   // MCLK和SMCLK选择XT2(可以使用|)
  }
  else
  {
    BCSCTL1 = XT2OFF;   // 关闭XT2高频晶体振荡器
    // DCO设置为最大值(全部置1)，8MHz
    DCOCTL = DCO0 + DCO1 + DCO2;
    BCSCTL1 = RSEL0 + RSEL1 + RSEL2;
    // 系统错误处理
    System.ucSystem_ErrorCode = ErrorCode_CLK;
    System.vError_Handle();
  }
}

/*
* @function     : vGPIO_Init
* @param        : None
* @retval       : None
* @brief        : 通用输入输出端口初始化
*/
static void vGPIO_Init(void)
{
  // GPIO配置
  // 引脚复用PxSEL ->  0 - GPIO / 1 - 复用(外部中断禁用)
  // 方向PxDIR -> 0 - 输入 / 1 - 输出
  // 输入PxIN / 输出PxOUT
  
  // P1与P2中断
  // 标志位   P1IFG / P2IFG - 中断标志位(必须软件清除)
  // 触发选择 P1IES / P2IES - 0 - 上升沿   1 - 下降沿(写入时会影响PxIFG)
  // 中断启用 P1IE / P2IE 
  
  // 根据外设进行配置
  P1DIR = 0xC6;P1OUT = 0x80;
  P2DIR = 0xFF;P2OUT = 0x00;
  P3DIR = 0x7F;P3OUT = 0xC0;
  P4DIR = 0xFF;P4OUT = 0x00;
  P5DIR = 0xFF;P5OUT = 0x61;
  P6DIR = 0x9F;P6OUT = 0x70;  
}

/*
* @function     : vIE_Init
* @param        : None
* @retval       : None
* @brief        : 中断初始化
*/
static void vIE_Init(void)
{
  // KEY中断
  P1IES |= KEY1;        // 下降沿触发
  P1IFG &= (~KEY1);     // 清除标志位，避免误触发
  P1IE |= KEY1; // 使能KEY1中断
  
  __enable_interrupt(); // 使能全局中断
}

/*
* @function     : vPower_On_Indication
* @param        : None
* @retval       : None
* @brief        : 上电指示
*/
static void vPower_On_Indication(void)
{
  
}

/*
* @function     : Sys_Init
* @param        : None
* @retval       : None
* @brief        : 系统初始化
*/
static void vSys_Init(void)
{
  WatchDog.vWatchDog_Init();    // 看门狗初始化
  Hardware_Init.vCLK_Init();    // 时钟初始化
  Hardware_Init.vGPIO_Init();   // GPIO初始化
  TimerA.vTimerA_Init();        // 定时器A初始化
  Hardware_Init.vIE_Init();     // 中断初始化
  USART1.vUSART1_Init();        // 串口1初始化
  Pwm.vPWM_Init();      // PWM初始化
  ADC.vADC_Init();      // ADC初始化
  OLED.vOLED_Init();                //OLED初始化
  TimerB.vTimerB_Init();        // 定时器B初始化
  USART1.vUSART1_SendString("系统初始化完成\r\n");
  printf("PI = %.1f\r\n",3.14);
}

