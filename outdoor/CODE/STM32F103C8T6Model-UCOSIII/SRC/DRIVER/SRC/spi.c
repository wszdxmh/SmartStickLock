/**
  ************************************
  * @file    spi.c
  * @author  缪辉
  * @version V3.5.0
  * @date    2015.08.09
  * @brief   spi驱动函数
	* SPI2:  SCK(PB13) SDA|MOSI(PB15) MISO(PB14) RST(PD9) DC(PD8)
	************************************
*/

#include "spi.h"


/**
  **********************************
  * @brief  SPI2读写数据
  * @param  TxData
  * @retval None
  **********************************
*/
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200) return 0;
	}			  
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry = 0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200) return 0;
	}
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据
}

/**
  **********************************
  * @brief  OLED写命令
  * @param  cmd
  * @retval None
  **********************************
*/
uint8_t LCD_SPI2_WrCmd(uint8_t cmd)
{
	uint8_t read = 0;
	DC_SPI2 = 0;
	read = SPI2_ReadWriteByte(cmd);
	return read;
}

/**
  **********************************
  * @brief  OLED写数据
  * @param  data
  * @retval None
  **********************************
*/
uint8_t LCD_SPI2_WrData(uint8_t data)
{
	uint8_t read = 0;
	DC_SPI2 = 1;
	read = SPI2_ReadWriteByte(data);
	return read;
}

/**
  **********************************
  * @brief  OLED设置坐标
  * @param  x：0~127  y:0~7
  * @retval None
  **********************************
*/
void LCD_SPI2_Set_Pos(uint8_t x, uint8_t y)
{
	LCD_SPI2_WrCmd(0xb0+y);
	LCD_SPI2_WrCmd(((x&0xf0)>>4)|0x10);
	LCD_SPI2_WrCmd((x&0x0f)|0x01);
}

/**
  **********************************
  * @brief  OLED清屏
  * @param  bmp_dat: 写0x00达到清屏
  * @retval None
  **********************************
*/
void LCD_SPI2_Fill(uint8_t bmp_dat)
{
	uint8_t x,y;
	LCD_SPI2_Set_Pos(0,0);
	for(y=0; y<8; y++)
	{
		LCD_SPI2_Set_Pos(0,y);
		for(x=0; x<128;x++)
			LCD_SPI2_WrData(0x00);
	}
}

/**
  **********************************
  * @brief  SPI2初始化
  * @param  None
  * @retval None
  **********************************
*/
void SPI2_Init(void)
{
	SPI_InitTypeDef SPI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//开启SPI2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//开启GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//开启GPIOD时钟
	//初始化GPIO口
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);//拉高电平
	//初始化RST DC
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_11);//拉高电平
	//初始化CS
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);//拉低电平
	//初始化SPI2
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//使用2分频
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//在第一个跳变沿被采集
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//空闲极性为低电平
	SPI_InitStruct.SPI_CRCPolynomial = 7;//CRC校检
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//传输数据长度
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线全双工
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//先传高字节
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;//主机模式
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;//片选方式为软件
	SPI_Init(SPI2, &SPI_InitStruct);
	SPI_Cmd(SPI2, ENABLE);//使能SPI2
}

void LCD_SPI2_Init(void)
{
	SPI2_Init();
	//初始化OLED
	RST_SPI2 = 0;//RST拉低
	delay_ms(1);
	RST_SPI2 = 1;//RST拉高
	delay_ms(1);
	//写入初始化命令
	LCD_SPI2_WrCmd(0xae);//--turn off oled panel
	LCD_SPI2_WrCmd(0x00);//---set low column address
	LCD_SPI2_WrCmd(0x10);//---set high column address
	LCD_SPI2_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	LCD_SPI2_WrCmd(0x81);//--set contrast control register
	LCD_SPI2_WrCmd(0xcf); // Set SEG Output Current Brightness
	LCD_SPI2_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0???? 0xa1??
	LCD_SPI2_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0???? 0xc8??
	LCD_SPI2_WrCmd(0xa6);//--set normal display
	LCD_SPI2_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	LCD_SPI2_WrCmd(0x3f);//--1/64 duty
	LCD_SPI2_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	LCD_SPI2_WrCmd(0x00);//-not offset
	LCD_SPI2_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	LCD_SPI2_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	LCD_SPI2_WrCmd(0xd9);//--set pre-charge period
	LCD_SPI2_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	LCD_SPI2_WrCmd(0xda);//--set com pins hardware configuration
	LCD_SPI2_WrCmd(0x12);
	LCD_SPI2_WrCmd(0xdb);//--set vcomh
	LCD_SPI2_WrCmd(0x40);//Set VCOM Deselect Level
	LCD_SPI2_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	LCD_SPI2_WrCmd(0x02);//
	LCD_SPI2_WrCmd(0x8d);//--set Charge Pump enable/disable
	LCD_SPI2_WrCmd(0x14);//--set(0x10) disable
	LCD_SPI2_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	LCD_SPI2_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	LCD_SPI2_WrCmd(0xaf);//--turn on oled panel
	LCD_SPI2_Fill(0x00);  //--clear
	LCD_SPI2_Set_Pos(0,0);  
}

/**
  **********************************
  * @brief  显示6x8数字
  * @param  x：0~128  y:0~7  data:0~9999
  * @retval LCD_SPI2_P6x8Num(32, 2, 1234)
  **********************************
*/
//void LCD_SPI2_P6x8Num(uint8_t x, uint8_t y, uint16_t data)
//{
//	uint8_t k=0;
//  for(uint8_t j=0; j<4; j++)
//  {
//  	switch (j)
//		{
//		 	case 0:{k = data/1000;} break;
//			case 1:{k = data%1000/100;} break;
//			case 2:{k = data%100/10;} break;
//			case 3:{k = data%10;} break;
//		}
//    LCD_SPI2_Set_Pos(x,y);
//  	for(uint8_t i=0; i<6; i++)
//			LCD_SPI2_WrData(F6x8_num[k][i]);
//  	x += 6;
//  }
//}

/**
  **********************************
  * @brief  显示8x16的字符串
  * @param  x：0~128  y:0~7  
  * @retval LCD_SPI2_P8x16Str(32, 2, "OLED")
  **********************************
*/
//void LCD_SPI2_P8x16Str(uint8_t x,uint8_t y,char ch[])
//{
//  uint8_t c=0,i=0,j=0;
//  while (ch[j]!='\0')
//  {
//    c =ch[j]-32;
//    if(x>127){x=0;y++;}
//    LCD_SPI2_Set_Pos(x,y);
//  	for(i=0;i<8;i++)
//  	  LCD_SPI2_WrData(F8X16[c*16+i]);
//  	LCD_SPI2_Set_Pos(x,y+1);
//  	for(i=0;i<8;i++)     
//  	  LCD_SPI2_WrData(F8X16[c*16+i+8]);
//  	x += 8;
//  	j++;
//  }
//}

void LCD_SPI2_P16x16Str(uint8_t x, uint8_t y)
{
	if(x > 127)
		x = 0, y++;
	for (uint8_t i=0; i<12; i++)
	{
		if (i%2 == 0)
			LCD_SPI2_Set_Pos(x,y);
		else
			LCD_SPI2_Set_Pos(x,y+1),x+=16;
		for (uint8_t j=0; j<16; j++)
			LCD_SPI2_WrData(Please_Input_Passwd[i*16+j]);
	}
}

//OLED缓冲池
uint8_t OLED_Buffer[8][128];
/**
*	@brief 把缓冲池数据写入OLED
*	@param[in] none
* @retf none
**/
void LCD_SPI_Display_ALL(void)
{
	for (uint8_t i=0; i<8; i++)
	{
		LCD_SPI2_Set_Pos(0,i);
		for (uint8_t j=0; j<128; j++)
			LCD_SPI2_WrData(OLED_Buffer[i][j]);
	}
}

/**
*	@brief 把要显示的字写入缓冲池(最高16)
* @param[in]	x:OLED x轴坐标 (0~127)
* @param[in]	y:OLED y轴坐标 (0~7)
* @param[in]	*str	写入数组指针
*	@param[in]	size	数组大小
* @param[in]	size_x	一个字的宽度
* @retf	none
**/
void Write_OLED_Bufeer_Str(uint8_t x, uint8_t y, unsigned char *str, size_t size, size_t size_x)
{
	uint16_t len = size;
	while(len-- != 0)
	{
		for (uint8_t i=0; i<(size/size_x); i++)
			for (uint8_t j=0; j<size_x; j++)
				if(i%2 == 0)
					*(*OLED_Buffer + 128*y + x + i/2*size_x+j) = *(str + i*size_x + j);
				else
					*(*OLED_Buffer + 128*(y+1) + x + (i-1)/2*size_x+j) = *(str + i*size_x + j);
	}
}

/**
*	@brief			清零指定区域
*	@param[in]	x1:起始x轴坐标
* @param[in]	y1:起始y轴坐标
*	@param[in]	x2:结束x轴坐标
*	@param[in]	y2:结束y轴坐标
*	@retf				none
**/
void Clear_OLED_Zone(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	for (uint8_t i=y1; i<=y2; i++)
		for (uint8_t j=x1; j<=x2; j++)
			*(*OLED_Buffer + 128*i + j) = 0x00;
}

/**
*	@breif			电池图标电量管理
*	@param[in]	x:起始x轴坐标
*	@param[in]	y:起始y轴坐标
*	@param[in]	vaule:电量(0~20)
*	@retf				none
**/
void Battery_Display(uint8_t x, uint8_t y, uint8_t vaule)
{
	for (uint8_t i=1; i<21; i++)
	{
		if (i < vaule)
			*(Batey+i) = 0xff;
		else
			*(Batey+i) = 0x81;
	}
	Write_OLED_Bufeer_Str(x,y,Batey,sizeof(Batey),24);
}

/**
*	@breif			NRF图标
*	@param[in]	x:起始x轴坐标
*	@param[in]	y:起始y轴坐标
*	@retf				none
**/
void NRF_EN_Display(uint8_t x, uint8_t y)
{
		Write_OLED_Bufeer_Str(x,y,NRF_EN,sizeof(NRF_EN),8);
}

/**
*	@breif			分割线
*	@param[in]	y:起始y轴坐标
*	@retf				none
**/
void Display_Dividing_Line(uint8_t y)
{
	for (uint8_t j=0; j<128; j++)
		*(*OLED_Buffer + 128*y + j) = 0x04;
}

void Display_P16x16_Number(uint8_t x, uint8_t y, uint8_t num)
{
	for (uint8_t i=0; i<8; i++)
	{
		*(*OLED_Buffer + 128*y + x + i) = *(P16x16_Number + num*16 + i);
		*(*OLED_Buffer + 128*(y + 1) + x + i) = *(P16x16_Number + num*16 + i+8);
	}
}

void Display_Fingerprint_Flag(uint8_t x, uint8_t y)
{
	Write_OLED_Bufeer_Str(x,y,Fingerprint_Flag,sizeof(Fingerprint_Flag),8);
}
