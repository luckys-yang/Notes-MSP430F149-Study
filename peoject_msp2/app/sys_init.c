/***************************************************************************
* File          : sys_init.c
* Author        : Luckys.
* Date          : 2023-06-04
* description   : ϵͳ��ʼ��  
****************************************************************************/

#include <main.h>
/*====================================static function declaration area BEGIN====================================*/
static void vCLK_Init(void);  // ʱ�ӳ�ʼ��
static void vGPIO_Init(void); // ͨ����������˿ڳ�ʼ��
static void vIE_Init(void); // �жϳ�ʼ��
static void vPower_On_Indication(void); // �ϵ�ָʾ
static void vSys_Init(void); // ϵͳ��ʼ��
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
* @brief        : ʱ�ӳ�ʼ��
*/
static void vCLK_Init(void)
{
  uint8_t i;
  uint8_t Error_Cnt = 0;
  
  // MCLK  -> ��ϵͳʱ�ӣ�CPUʹ��
  // SMCLK -> ��ϵͳʱ�ӣ���Ҫ����������ʹ��
  // ACLK  -> ����ϵͳ����Ҫ����������ʹ��
  
  // ϵͳ��λ��ʱ��Ĭ��������£�
  // MCLK��SMCLKĬ��ʹ��DCO��Ƶ��Ϊ800kHz��
  // ACLKʹ��LFXT1��Ƶ��Ϊ32.768KH;
  
  // ����ʱ��
  // MCLK = SMCLK = XT2 = 8MHz
  // ACLK = LFXT1 = 32.768KHZ
  BCSCTL1 &= ~XT2OFF;  // ��XT2��Ƶ��������
  do
  {
    IFG1 &= ~OFIFG;     // ����������ϱ�־λ
    for (i = 0; i < 100; i++);  // ���ٵȴ�50������--50us

    if(Error_Cnt++ >= 100)        // ��ʱ�˳�
    {
      break;
    }
  }
  while (IFG1&OFIFG);   // ��⵴�����ϱ�־λ��Ϊ0ʱ�˳�ѭ��
  
  if (Error_Cnt < 100)  // ˵��������������������������˳�
  {
    BCSCTL2 |= SELM_2 + SELS;   // MCLK��SMCLKѡ��XT2(����ʹ��|)
  }
  else
  {
    BCSCTL1 = XT2OFF;   // �ر�XT2��Ƶ��������
    // DCO����Ϊ���ֵ(ȫ����1)��8MHz
    DCOCTL = DCO0 + DCO1 + DCO2;
    BCSCTL1 = RSEL0 + RSEL1 + RSEL2;
    // ϵͳ������
    System.ucSystem_ErrorCode = ErrorCode_CLK;
    System.vError_Handle();
  }
}

/*
* @function     : vGPIO_Init
* @param        : None
* @retval       : None
* @brief        : ͨ����������˿ڳ�ʼ��
*/
static void vGPIO_Init(void)
{
  // GPIO����
  // ���Ÿ���PxSEL ->  0 - GPIO / 1 - ����(�ⲿ�жϽ���)
  // ����PxDIR -> 0 - ���� / 1 - ���
  // ����PxIN / ���PxOUT
  
  // P1��P2�ж�
  // ��־λ   P1IFG / P2IFG - �жϱ�־λ(����������)
  // ����ѡ�� P1IES / P2IES - 0 - ������   1 - �½���(д��ʱ��Ӱ��PxIFG)
  // �ж����� P1IE / P2IE 
  
  // ���������������
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
* @brief        : �жϳ�ʼ��
*/
static void vIE_Init(void)
{
  // KEY�ж�
  P1IES |= KEY1;        // �½��ش���
  P1IFG &= (~KEY1);     // �����־λ�������󴥷�
  P1IE |= KEY1; // ʹ��KEY1�ж�
  
  __enable_interrupt(); // ʹ��ȫ���ж�
}

/*
* @function     : vPower_On_Indication
* @param        : None
* @retval       : None
* @brief        : �ϵ�ָʾ
*/
static void vPower_On_Indication(void)
{
  
}

/*
* @function     : Sys_Init
* @param        : None
* @retval       : None
* @brief        : ϵͳ��ʼ��
*/
static void vSys_Init(void)
{
  WatchDog.vWatchDog_Init();    // ���Ź���ʼ��
  Hardware_Init.vCLK_Init();    // ʱ�ӳ�ʼ��
  Hardware_Init.vGPIO_Init();   // GPIO��ʼ��
  TimerA.vTimerA_Init();        // ��ʱ��A��ʼ��
  Hardware_Init.vIE_Init();     // �жϳ�ʼ��
  USART1.vUSART1_Init();        // ����1��ʼ��
  Pwm.vPWM_Init();      // PWM��ʼ��
  ADC.vADC_Init();      // ADC��ʼ��
  OLED.vOLED_Init();                //OLED��ʼ��
  TimerB.vTimerB_Init();        // ��ʱ��B��ʼ��
  USART1.vUSART1_SendString("ϵͳ��ʼ�����\r\n");
  printf("PI = %.1f\r\n",3.14);
}

