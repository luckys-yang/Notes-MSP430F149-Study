/***************************************************************************
 * File          : usart1.c
 * Author        : Luckys.
 * Date          : 2023-06-06
 * description   : ����1  
****************************************************************************/

#include <main.h>

/*====================================variable definition declaration area BEGIN=================================*/
static uint8_t ucSend_Buffer[USART1_Send_LEN];  //��������
static uint8_t ucRec_Buffer[USART1_Rec_LEN];    // ��������
/*====================================variable definition declaration area   END=================================*/


/*====================================static function declaration area BEGIN====================================*/
static void vUSART1_Init(void);   // ����1��ʼ��
static void vUSART1_SendArray(uint8_t*, uint16_t);        // ��������
static void vUSART1_SendString(uint8_t*); // �����ַ���
static void vUSART1_SendData(uint8_t);  // �����ַ�
static void vUSART1_Protocol(void);     // �ӿ�Э��
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
* @brief        : ����1��ʼ��
*/
static void vUSART1_Init(void)
{
  P3SEL |= BIT6 + BIT7; // �����������Ź���P36(TX) P3(RX)
  // ��������
  UCTL1 |= SWRST;       // ģ�鴦�ڸ�λ״̬(Ĭ���Ѿ���1�����п�Ҫ�ɲ�Ҫ)
  ME2 |= UTXE1 + URXE1; // ʹ�ܴ���1���ͺͽ���
  UCTL1 |= CHAR;        // ���ݳ���ѡ��8λ
  // ���������� -- �ֲ��ѯ��֪��9600pcs ��Ӧ 00 03 4A
  UTCTL1 |= SSEL0;      // ����ACLK
  UBR11 = 0x00; // UxBR1
  UBR01 = 0x03; // UxBR0
  UMCTL1 = 0x4A;        // UxMCTL
  UCTL1 &= ~SWRST;      // ��SWRST��0������ģ��
  // ���������ж�
  IE2 |= URXIE1;
}

/*
* @function     : vUSART1_SendArray
* @param        : p_Arr --> Ҫ���͵����� Arr_len --> ���ݵĳ���
* @retval       : None
* @brief        : ��������
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
* @param        : p_Str --> Ҫ���͵��ַ���
* @retval       : None
* @brief        : ��������
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
* @param        : ch --> Ҫ���͵��ַ�����
* @retval       : None
* @brief        : �����ַ�
*/
static void vUSART1_SendData(uint8_t ch)
{
  while (!(IFG2 & UTXIFG1));    // �ȴ�Ϊ�ղ��ܷ���
  TXBUF1 = ch;
}

// putchar���� �ض���
extern int putchar(int c)
{
  vUSART1_SendData((uint8_t)c);
  
  return c;
}

// ���ڽ����ж�
#pragma vector = UART1RX_VECTOR
__interrupt void USART1_RX_isr(void)
{
  uint8_t Rec_Data;
  
  if (USART1.ucRec_Cnt < USART1_Rec_LEN)
  {
    // ��ȡ����
    Rec_Data = RXBUF1; 
    ucRec_Buffer[USART1.ucRec_Cnt++] = Rec_Data;
  }
  // ��λ���ձ�־λ
  USART1.ucRec_Flag = TRUE;
}

/*
* @function     : vUSART1_Protocol
* @param        : None
* @retval       : None
* @brief        : �����ַ�
*/
static void vUSART1_Protocol(void)
{
  if (USART1.ucRec_Flag == TRUE)
  {
    // ���˸�������
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
      // Э�����
      Modbus.vModbus_Protocol_Analysis(&USART1);
    }
    // ���½���
    USART1.ucRec_Cnt = 0;
    USART1.ucRec_Flag = FALSE;
  }
}

