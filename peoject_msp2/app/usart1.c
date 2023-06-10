/***************************************************************************
 * File          : usart1.c
 * Author        : Luckys.
 * Date          : 2023-06-06
 * description   : 串口1  
****************************************************************************/

#include <main.h>

/*====================================variable definition declaration area BEGIN=================================*/
static uint8_t ucSend_Buffer[USART1_Send_LEN];  //发送数组
static uint8_t ucRec_Buffer[USART1_Rec_LEN];    // 接收数组
/*====================================variable definition declaration area   END=================================*/


/*====================================static function declaration area BEGIN====================================*/
static void vUSART1_Init(void);   // 串口1初始化
static void vUSART1_SendArray(uint8_t*, uint16_t);        // 发送数组
static void vUSART1_SendString(uint8_t*); // 发送字符串
static void vUSART1_SendData(uint8_t);  // 发送字符
static void vUSART1_Protocol(void);     // 接口协议
/*====================================static function declaration area   END====================================*/

USART1_t USART1 = 
{
  FALSE,
  0,
  ucSend_Buffer,
  ucRec_Buffer,
  
  vUSART1_Init,
  vUSART1_SendArray,
  vUSART1_SendString,
  vUSART1_Protocol,
};

/*
* @function     : vUSART1_Init
* @param        : None
* @retval       : None
* @brief        : 串口1初始化
*/
static void vUSART1_Init(void)
{
  P3SEL |= BIT6 + BIT7; // 开启复用引脚功能P36(TX) P3(RX)
  // 参数设置
  UCTL1 |= SWRST;       // 模块处于复位状态(默认已经置1，此行可要可不要)
  ME2 |= UTXE1 + URXE1; // 使能串口1发送和接收
  UCTL1 |= CHAR;        // 数据长度选择8位
  // 波特率设置 -- 手册查询可知：9600pcs 对应 00 03 4A
  UTCTL1 |= SSEL0;      // 配置ACLK
  UBR11 = 0x00; // UxBR1
  UBR01 = 0x03; // UxBR0
  UMCTL1 = 0x4A;        // UxMCTL
  UCTL1 &= ~SWRST;      // 把SWRST置0，启动模块
  // 开启接收中断
  IE2 |= URXIE1;
}

/*
* @function     : vUSART1_SendArray
* @param        : p_Arr --> 要发送的数组 Arr_len --> 数据的长度
* @retval       : None
* @brief        : 发送数组
*/
static void vUSART1_SendArray(uint8_t* p_Arr, uint16_t Arr_len)
{
  uint16_t i;
  
  for (i = 0; i < Arr_len; i++)
  {
    vUSART1_SendData(*(p_Arr + i));
  }
}

/*
* @function     : vUSART1_SendString
* @param        : p_Str --> 要发送的字符串
* @retval       : None
* @brief        : 发送数组
*/
static void vUSART1_SendString(uint8_t* p_Str)
{
  while (*p_Str)
  {
    vUSART1_SendData(*(p_Str++));
  }
}

/*
* @function     : vUSART1_SendData
* @param        : ch --> 要发送的字符数据
* @retval       : None
* @brief        : 发送字符
*/
static void vUSART1_SendData(uint8_t ch)
{
  while (!(IFG2 & UTXIFG1));    // 等待为空才能发送
  TXBUF1 = ch;
}

// putchar函数 重定向
extern int putchar(int c)
{
  vUSART1_SendData((uint8_t)c);
  
  return c;
}

// 串口接收中断
#pragma vector = UART1RX_VECTOR
__interrupt void USART1_RX_isr(void)
{
  uint8_t Rec_Data;
  
  if (USART1.ucRec_Cnt < USART1_Rec_LEN)
  {
    // 提取数据
    Rec_Data = RXBUF1; 
    ucRec_Buffer[USART1.ucRec_Cnt++] = Rec_Data;
  }
  // 置位接收标志位
  USART1.ucRec_Flag = TRUE;
}

/*
* @function     : vUSART1_Protocol
* @param        : None
* @retval       : None
* @brief        : 发送字符
*/
static void vUSART1_Protocol(void)
{
  if (USART1.ucRec_Flag == TRUE)
  {
    // 过滤干扰数据
    if (ucRec_Buffer[0] != 0)
    {
      TimerA.usUSART1_Delay_Timer = 0;
      while (USART1.ucRec_Cnt < 8)
      {
        if (TimerA.usUSART1_Delay_Timer >= TimerA_100ms)
        {
          break;
        }
      }
      // 协议分析
      Modbus.vModbus_Protocol_Analysis(&USART1);
    }
    // 重新接收
    USART1.ucRec_Cnt = 0;
    USART1.ucRec_Flag = FALSE;
  }
}

