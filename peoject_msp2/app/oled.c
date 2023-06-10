/***************************************************************************
 * File          : oled.c
 * Author        : Luckys.
 * Date          : 2023-06-08
 * description   : OLED
****************************************************************************/


#include <main.h>
#include "oled_font.h"  // ������Ҫ�����ٵ���

/*====================================static function declaration area BEGIN====================================*/
static void vOLED_Init(void);     // OLED��ʼ��
static void vOLED_Clear(void);    // ����
static void vOLED_Show_Char(uint8_t, uint8_t, uint8_t, ASCII_Size_t);        // OLED��ʾ�ַ�
static void vOLED_Show_String(uint8_t, uint8_t, const char*, ASCII_Size_t);       // OLED��ʾ�ַ���
static void vOLED_Show_CHN(uint8_t, uint8_t, const char*);        // OLED��ʾ����
static void vOLED_Set_Pos(uint8_t, uint8_t);   // ����λ��

static void vIIC_Start(void);    // IIC����
static void vIIC_Stop(void);    // IICֹͣ
static uint8_t ucIIC_Write_Byte(uint8_t);    // IICд���ֽ�
static void vOLED_Write_CMD(uint8_t);    // OLEDд����
static void vOLED_Write_Data(uint8_t);    // OLEDд����
/*====================================static function declaration area   END====================================*/


OLED_t OLED = 
{
  vOLED_Init,
  vOLED_Clear,
  vOLED_Show_Char,
  vOLED_Show_String,
  vOLED_Show_CHN,
};


/*
* @function     : vOLED_Init
* @param        : None
* @retval       : None
* @brief        : OLED��ʼ��
*/
static void vOLED_Init(void)
{
  Public.vDelay_ms(100);        // �ϵ���ʱ
  vOLED_Write_CMD(0xAE);//--display off
  vOLED_Write_CMD(0x00);//---set low column address
  vOLED_Write_CMD(0x10);//---set high column address
  vOLED_Write_CMD(0x40);//--set start line address  
  vOLED_Write_CMD(0xB0);//--set page address
  vOLED_Write_CMD(0x81); // contract control
  vOLED_Write_CMD(0xFF);//--128   
  vOLED_Write_CMD(0xA1);//set segment remap 
  vOLED_Write_CMD(0xA6);//--normal / reverse
  vOLED_Write_CMD(0xA8);//--set multiplex ratio(1 to 64)
  vOLED_Write_CMD(0x3F);//--1/32 duty
  vOLED_Write_CMD(0xC8);//Com scan direction
  vOLED_Write_CMD(0xD3);//-set display offset
  vOLED_Write_CMD(0x00);//
  
  vOLED_Write_CMD(0xD5);//set osc division
  vOLED_Write_CMD(0x80);//
  
  vOLED_Write_CMD(0xD8);//set area color mode off
  vOLED_Write_CMD(0x05);//
  
  vOLED_Write_CMD(0xD9);//Set Pre-Charge Period
  vOLED_Write_CMD(0xF1);//
  
  vOLED_Write_CMD(0xDA);//set com pin configuartion
  vOLED_Write_CMD(0x12);//
  
  vOLED_Write_CMD(0xDB);//set Vcomh
  vOLED_Write_CMD(0x30);//
  
  vOLED_Write_CMD(0x8D);//set charge pump enable
  vOLED_Write_CMD(0x14);//
  
  vOLED_Write_CMD(0xAF);//--turn on oled panel
  
  //OLED����
  OLED.vOLED_Clear();                  
  //OLED��Ļ��ʼ��ʾ
  OLED.vOLED_Show_CHN(0,8,"̫");
  OLED.vOLED_Show_CHN(0,32,"��");
  OLED.vOLED_Show_CHN(0,56,"��");
  OLED.vOLED_Show_CHN(0,80,"·");
  OLED.vOLED_Show_CHN(0,104,"��");
  
  OLED.vOLED_Show_String(2,24,"VIN:",ASCII_SIZE_16);
  OLED.vOLED_Show_String(4,24,"BAT:",ASCII_SIZE_16);
  
  OLED.vOLED_Show_CHN(6,96,"��");
  OLED.vOLED_Show_CHN(6,112,"��");  
}

/*
* @function     : vOLED_Clear
* @param        : None
* @retval       : None
* @brief        : ����
*/
static void vOLED_Clear(void)
{
  uint8_t Page,Seg;
  
  for (Page = 0; Page < 8; Page++)
  {
    vOLED_Write_CMD(0xB0 + Page);       // һ��8ҳ(��)
    vOLED_Write_CMD(0x00);      // ��
    vOLED_Write_CMD(0x01);      // ��
    
    for (Seg = 0; Seg < 128; Seg++)
    {
      vOLED_Write_Data(0x00);
    }
  }
}

/*
* @function     : vOLED_Show_Char
* @param        : Page -> ҳλ�� Seg -> ��λ��
* @retval       : None
* @brief        : OLED��ʾ�ַ�
*/
static void vOLED_Set_Pos(uint8_t Page, uint8_t Seg)
{
  vOLED_Write_CMD(0xB0 + Page);
  vOLED_Write_CMD(Seg & 0x0F);  // ��4λ
  vOLED_Write_CMD((Seg & 0xF0 >> 4) | 0x10);  // ��4λ
}

/*
* @function     : vOLED_Show_Char
* @param        : Page -> ҳλ�� Seg -> ��λ�� ch -> Ҫ��ʾ���ַ� ch_size -> �����С
* @retval       : None
* @brief        : OLED��ʾ�ַ�
*/
static void vOLED_Show_Char(uint8_t Page, uint8_t Seg, uint8_t ch, ASCII_Size_t ch_size)
{
  uint8_t ucIndex,i;
  if (PWR_Status_ON == Power.Peripheral_3V3_Status)
  {
    // ASCII �ַ���������������Ҫ��ȥƫ����(' ' -> �ո��Ӧ����ֵ)
    ucIndex = ch - ' ';
    // �жϴ�С
    if (ASCII_SIZE_16 == ch_size)
    {
      // �����ַ��ϰ벿��
      vOLED_Set_Pos(Page,Seg);    
      // д���ַ��ϰ벿������
      for (i = 0; i < 8; i++)
      {
        vOLED_Write_Data(ucASCII_16x8[ucIndex][i]);
      }
      // �����ַ��°벿��
      vOLED_Set_Pos(Page + 1,Seg);
      // д���ַ��°벿������
      for (i = 0; i < 8; i++)
      {
        vOLED_Write_Data(ucASCII_16x8[ucIndex][i + 8]);
      }
    }    
  }
}

/*
* @function     : vOLED_Show_String
* @param        : Page -> ҳλ�� Seg -> ��λ�� p_Str -> Ҫ��ʾ���ַ��� ch_size -> �����С
* @retval       : None
* @brief        : OLED��ʾ�ַ���
*/
static void vOLED_Show_String(uint8_t Page, uint8_t Seg, const char* p_Str, ASCII_Size_t ch_size)
{
  // �ַ��ߴ�߶�Ϊ 1-8ʱռ1Page  9-16ʱռ2Page 17-24ʱռ3Page ....�Դ�����
  // ����ô�����Ӧ��Page��1,2,3.... ��ֱ�Ӽ�7����8����
  uint8_t ch_Pages = (ch_size + 7) / 8; // �ַ�ռ�õ�ҳ��
  
  if (PWR_Status_ON == Power.Peripheral_3V3_Status)
  {
    while(*p_Str != '\0')
    {
      // �Զ�����
      if ((Seg + ch_size / 2) > OLED_WIDTH)       // ��� λ��+�ַ����(16/2=8) ���� ��Ļ���
      {
        Seg = 0;
        Page += ch_Pages;
        if (Page >= OLED_PAGE_MAX)
        {
          Page = 0;
        }
      }
      // �Զ���ҳ
      if ((Page + ch_Pages) > OLED_PAGE_MAX)
      {
        Seg = 0;
        Page = 0;
      }
      // ��ʾ�ַ�
      vOLED_Show_Char(Page,Seg,*p_Str,ch_size);
      // �����ַ�
      p_Str++;
      // ������ʾλ��
      Seg += ch_size / 2;
    }    
  }
}

/*
* @function     : vOLED_Show_CHN
* @param        : Page -> ҳλ�� Seg -> ��λ�� p_Str -> Ҫ��ʾ���ַ���
* @retval       : None
* @brief        : OLED��ʾ����
*/
static void vOLED_Show_CHN(uint8_t Page, uint8_t Seg, const char* p_Str)
{
  uint16_t usCHN_Number;  // �ֿ��к�������
  uint16_t usIndex;     // �ֿ��еĺ�������
  uint8_t i;
  
  if (PWR_Status_ON == Power.Peripheral_3V3_Status)
  {
    // ͳ�ƺ��ֵ�λ��
    usCHN_Number = sizeof(CHN_16x16) / sizeof(CHN_16x16_t);
    // ���Һ��ֵ�λ��
    for (usIndex = 0; usIndex < usCHN_Number; usIndex++)
    {
      if ((CHN_16x16[usIndex].Index[0] == *p_Str) && (CHN_16x16[usIndex].Index[1] == *(p_Str + 1)))       // ��Ϊһ������ռ�����ֽ�
      {
        // �����ַ��ϰ벿����ʼλ��
        vOLED_Set_Pos(Page,Seg);   
        // д���ϰ벿������
        for (i = 0; i < 16; i++)
        {
          vOLED_Write_Data(CHN_16x16[usIndex].CHN_code[i]);
        }
        // �����ַ��°벿����ʼλ��
        vOLED_Set_Pos(Page + 1,Seg);
        // д���°벿������
        for (i = 0; i < 16; i++)
        {
          vOLED_Write_Data(CHN_16x16[usIndex].CHN_code[i + 16]);
        }
        break;    // �ҵ�ָ�룬�˳�ѭ��
      }
    }    
  }
}

/*
* @function     : vIIC_Start
* @param        : None
* @retval       : None
* @brief        : IIC����
*/
static void vIIC_Start(void)
{
  //SCLΪ�ߵ�ƽ��SDA���½���ΪI2C��ʼ�ź�
  SET_SDA;
  SET_SCL;
  _NOP();
  CLR_SDA;
  _NOP();
  CLR_SCL;
}

/*
* @function     : vIIC_Stop
* @param        : None
* @retval       : None
* @brief        : IICֹͣ
*/
static void vIIC_Stop(void)
{
  //SCLΪ�ߵ�ƽ��SDA��������ΪI2Cֹͣ�ź�
  CLR_SDA;
  SET_SCL;
  _NOP();
  SET_SDA;
}

/*
* @function     : ucIIC_Write_Byte
* @param        : WR_byte -> ��д���ֽ�
* @retval       : Ӧ��
* @brief        : IICд���ֽ�
*/
static uint8_t ucIIC_Write_Byte(uint8_t WR_byte)
{
    uint8_t i;
    // �洢��ȡ��SDA��ƽ״̬
    uint8_t  ACK_Rspond;

    //SCLΪ�͵�ƽʱ��SDA׼������,����SCLΪ�ߵ�ƽ����ȡSDA����
    //���ݰ�8λ���䣬��λ��ǰ������forѭ���������
    for(i = 0; i < 8; i++)
    {
        //SCL���㣬����SDA׼������
        CLR_SCL;
        if((WR_byte & BIT7) == BIT7)
        {
            SET_SDA;
        }
        else
        {
            CLR_SDA;
        }
        _NOP();
        //SCL�øߣ���������
        SET_SCL;
        _NOP();

        //׼��������һ����λ
        WR_byte <<= 1;
    }

    CLR_SCL;
    //SDA��Ϊ���� �ͷ�SDA���ȴ��ӻ�Ӧ��
    SET_SDA;
    SDA_IN; 
    _NOP();

    SET_SCL;
    _NOP();

    ACK_Rspond = READ_SDA;      // ����Ӧ��
    // SDA��Ϊ���
    SDA_OUT; 
    CLR_SCL;
    _NOP();

    //���شӻ���Ӧ���ź�
    return ACK_Rspond;
}

/*
* @function     : vOLED_Write_CMD
* @param        : CMD -> ��д������
* @retval       : None
* @brief        : OLEDд����
*/
static void vOLED_Write_CMD(uint8_t CMD)
{
  vIIC_Start();
  ucIIC_Write_Byte(OLED_ADDR & (~BIT0));        // R/W#=0
  ucIIC_Write_Byte(0x00);       // Co=0,D/C#=0
  ucIIC_Write_Byte(CMD);
  vIIC_Stop();
}

/*
* @function     : vOLED_Write_Data
* @param        : Data -> ��д������
* @retval       : None
* @brief        : OLEDд����
*/
static void vOLED_Write_Data(uint8_t Data)
{
  vIIC_Start();
  ucIIC_Write_Byte(OLED_ADDR & (~BIT0));        // R/W#=0
  ucIIC_Write_Byte(0x40);       // Co=0,D/C#=1
  ucIIC_Write_Byte(Data);
  vIIC_Stop();
}


