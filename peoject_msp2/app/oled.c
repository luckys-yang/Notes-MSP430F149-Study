/***************************************************************************
 * File          : oled.c
 * Author        : Luckys.
 * Date          : 2023-06-08
 * description   : OLED
****************************************************************************/


#include <main.h>
#include "oled_font.h"  // 哪里需要调用再调用

/*====================================static function declaration area BEGIN====================================*/
static void vOLED_Init(void);     // OLED初始化
static void vOLED_Clear(void);    // 清屏
static void vOLED_Show_Char(uint8_t, uint8_t, uint8_t, ASCII_Size_t);        // OLED显示字符
static void vOLED_Show_String(uint8_t, uint8_t, const char*, ASCII_Size_t);       // OLED显示字符串
static void vOLED_Show_CHN(uint8_t, uint8_t, const char*);        // OLED显示汉字
static void vOLED_Set_Pos(uint8_t, uint8_t);   // 设置位置

static void vIIC_Start(void);    // IIC启动
static void vIIC_Stop(void);    // IIC停止
static uint8_t ucIIC_Write_Byte(uint8_t);    // IIC写入字节
static void vOLED_Write_CMD(uint8_t);    // OLED写命令
static void vOLED_Write_Data(uint8_t);    // OLED写数据
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
* @brief        : OLED初始化
*/
static void vOLED_Init(void)
{
  Public.vDelay_ms(100);        // 上电延时
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
  
  //OLED清屏
  OLED.vOLED_Clear();                  
  //OLED屏幕初始显示
  OLED.vOLED_Show_CHN(0,8,"太");
  OLED.vOLED_Show_CHN(0,32,"阳");
  OLED.vOLED_Show_CHN(0,56,"能");
  OLED.vOLED_Show_CHN(0,80,"路");
  OLED.vOLED_Show_CHN(0,104,"灯");
  
  OLED.vOLED_Show_String(2,24,"VIN:",ASCII_SIZE_16);
  OLED.vOLED_Show_String(4,24,"BAT:",ASCII_SIZE_16);
  
  OLED.vOLED_Show_CHN(6,96,"充");
  OLED.vOLED_Show_CHN(6,112,"电");  
}

/*
* @function     : vOLED_Clear
* @param        : None
* @retval       : None
* @brief        : 清屏
*/
static void vOLED_Clear(void)
{
  uint8_t Page,Seg;
  
  for (Page = 0; Page < 8; Page++)
  {
    vOLED_Write_CMD(0xB0 + Page);       // 一共8页(行)
    vOLED_Write_CMD(0x00);      // 低
    vOLED_Write_CMD(0x01);      // 高
    
    for (Seg = 0; Seg < 128; Seg++)
    {
      vOLED_Write_Data(0x00);
    }
  }
}

/*
* @function     : vOLED_Show_Char
* @param        : Page -> 页位置 Seg -> 段位置
* @retval       : None
* @brief        : OLED显示字符
*/
static void vOLED_Set_Pos(uint8_t Page, uint8_t Seg)
{
  vOLED_Write_CMD(0xB0 + Page);
  vOLED_Write_CMD(Seg & 0x0F);  // 低4位
  vOLED_Write_CMD((Seg & 0xF0 >> 4) | 0x10);  // 高4位
}

/*
* @function     : vOLED_Show_Char
* @param        : Page -> 页位置 Seg -> 段位置 ch -> 要显示的字符 ch_size -> 字体大小
* @retval       : None
* @brief        : OLED显示字符
*/
static void vOLED_Show_Char(uint8_t Page, uint8_t Seg, uint8_t ch, ASCII_Size_t ch_size)
{
  uint8_t ucIndex,i;
  if (PWR_Status_ON == Power.Peripheral_3V3_Status)
  {
    // ASCII 字符集数组索引，需要减去偏移量(' ' -> 空格对应的码值)
    ucIndex = ch - ' ';
    // 判断大小
    if (ASCII_SIZE_16 == ch_size)
    {
      // 设置字符上半部分
      vOLED_Set_Pos(Page,Seg);    
      // 写入字符上半部分数据
      for (i = 0; i < 8; i++)
      {
        vOLED_Write_Data(ucASCII_16x8[ucIndex][i]);
      }
      // 设置字符下半部分
      vOLED_Set_Pos(Page + 1,Seg);
      // 写入字符下半部分数据
      for (i = 0; i < 8; i++)
      {
        vOLED_Write_Data(ucASCII_16x8[ucIndex][i + 8]);
      }
    }    
  }
}

/*
* @function     : vOLED_Show_String
* @param        : Page -> 页位置 Seg -> 段位置 p_Str -> 要显示的字符串 ch_size -> 字体大小
* @retval       : None
* @brief        : OLED显示字符串
*/
static void vOLED_Show_String(uint8_t Page, uint8_t Seg, const char* p_Str, ASCII_Size_t ch_size)
{
  // 字符尺寸高度为 1-8时占1Page  9-16时占2Page 17-24时占3Page ....以此类推
  // 那怎么算出对应的Page是1,2,3.... 那直接加7除以8即可
  uint8_t ch_Pages = (ch_size + 7) / 8; // 字符占用的页数
  
  if (PWR_Status_ON == Power.Peripheral_3V3_Status)
  {
    while(*p_Str != '\0')
    {
      // 自动换行
      if ((Seg + ch_size / 2) > OLED_WIDTH)       // 如果 位置+字符宽度(16/2=8) 大于 屏幕宽度
      {
        Seg = 0;
        Page += ch_Pages;
        if (Page >= OLED_PAGE_MAX)
        {
          Page = 0;
        }
      }
      // 自动换页
      if ((Page + ch_Pages) > OLED_PAGE_MAX)
      {
        Seg = 0;
        Page = 0;
      }
      // 显示字符
      vOLED_Show_Char(Page,Seg,*p_Str,ch_size);
      // 更新字符
      p_Str++;
      // 更新显示位置
      Seg += ch_size / 2;
    }    
  }
}

/*
* @function     : vOLED_Show_CHN
* @param        : Page -> 页位置 Seg -> 段位置 p_Str -> 要显示的字符串
* @retval       : None
* @brief        : OLED显示汉字
*/
static void vOLED_Show_CHN(uint8_t Page, uint8_t Seg, const char* p_Str)
{
  uint16_t usCHN_Number;  // 字库中汉字数量
  uint16_t usIndex;     // 字库中的汉字索引
  uint8_t i;
  
  if (PWR_Status_ON == Power.Peripheral_3V3_Status)
  {
    // 统计汉字的位置
    usCHN_Number = sizeof(CHN_16x16) / sizeof(CHN_16x16_t);
    // 查找汉字的位置
    for (usIndex = 0; usIndex < usCHN_Number; usIndex++)
    {
      if ((CHN_16x16[usIndex].Index[0] == *p_Str) && (CHN_16x16[usIndex].Index[1] == *(p_Str + 1)))       // 因为一个汉字占两个字节
      {
        // 设置字符上半部分起始位置
        vOLED_Set_Pos(Page,Seg);   
        // 写入上半部分数据
        for (i = 0; i < 16; i++)
        {
          vOLED_Write_Data(CHN_16x16[usIndex].CHN_code[i]);
        }
        // 设置字符下半部分起始位置
        vOLED_Set_Pos(Page + 1,Seg);
        // 写入下半部分数据
        for (i = 0; i < 16; i++)
        {
          vOLED_Write_Data(CHN_16x16[usIndex].CHN_code[i + 16]);
        }
        break;    // 找到指针，退出循环
      }
    }    
  }
}

/*
* @function     : vIIC_Start
* @param        : None
* @retval       : None
* @brief        : IIC启动
*/
static void vIIC_Start(void)
{
  //SCL为高电平，SDA的下降沿为I2C起始信号
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
* @brief        : IIC停止
*/
static void vIIC_Stop(void)
{
  //SCL为高电平，SDA的上升沿为I2C停止信号
  CLR_SDA;
  SET_SCL;
  _NOP();
  SET_SDA;
}

/*
* @function     : ucIIC_Write_Byte
* @param        : WR_byte -> 待写入字节
* @retval       : 应答
* @brief        : IIC写入字节
*/
static uint8_t ucIIC_Write_Byte(uint8_t WR_byte)
{
    uint8_t i;
    // 存储读取的SDA电平状态
    uint8_t  ACK_Rspond;

    //SCL为低电平时，SDA准备数据,接着SCL为高电平，读取SDA数据
    //数据按8位传输，高位在前，利用for循环逐个接收
    for(i = 0; i < 8; i++)
    {
        //SCL清零，主机SDA准备数据
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
        //SCL置高，传输数据
        SET_SCL;
        _NOP();

        //准备发送下一比特位
        WR_byte <<= 1;
    }

    CLR_SCL;
    //SDA设为输入 释放SDA，等待从机应答
    SET_SDA;
    SDA_IN; 
    _NOP();

    SET_SCL;
    _NOP();

    ACK_Rspond = READ_SDA;      // 接收应答
    // SDA设为输出
    SDA_OUT; 
    CLR_SCL;
    _NOP();

    //返回从机的应答信号
    return ACK_Rspond;
}

/*
* @function     : vOLED_Write_CMD
* @param        : CMD -> 待写入命令
* @retval       : None
* @brief        : OLED写命令
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
* @param        : Data -> 待写入数据
* @retval       : None
* @brief        : OLED写数据
*/
static void vOLED_Write_Data(uint8_t Data)
{
  vIIC_Start();
  ucIIC_Write_Byte(OLED_ADDR & (~BIT0));        // R/W#=0
  ucIIC_Write_Byte(0x40);       // Co=0,D/C#=1
  ucIIC_Write_Byte(Data);
  vIIC_Stop();
}


