/**
  ************************************
  * @file    spi.h
  * @author  wszdxmh
  * @version V3.5.0
  * @date    2015.08.09
  * @brief   SPI2:  SCL(PB13) SDA|MOSI(PB15) MISO(PB14) RST(PD9) DC(PD8)
	************************************
*/

#ifndef __SPI_H
#define __SPI_H

#include "string.h"
#include "oled_library.h"

//OLEDÒý½Å¶¨Òå
#define SCL_SPI2  PBout(13)
#define SDA_SPI2  PBout(15)
#define RST_SPI2  PAout(8)
#define DC_SPI2   PAout(11)

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "delay.h"
#include "sys.h"

#define PLEASE_INPUT_PASSWD(x,y)		Write_OLED_Bufeer_Str(x,y,Please_Input_Passwd,sizeof(Please_Input_Passwd),16)
#define PLEASE_INPUT_ID(x,y)				Write_OLED_Bufeer_Str(x,y,Input_ID,sizeof(Input_ID),16)
#define DIPLAY_STAR(x,y)						Write_OLED_Bufeer_Str(x,y,Star,sizeof(Star),8)
#define PLEASE_FINGER(x,y)					Write_OLED_Bufeer_Str(x,y,Please_Fingerprint,sizeof(Please_Fingerprint),16)
#define PLEASE_AGAN_FINGER(x,y)			Write_OLED_Bufeer_Str(x,y,Please_Agan_Fingerprint,sizeof(Please_Agan_Fingerprint),16)
#define ENTERY_SUCCESS(x,y)					Write_OLED_Bufeer_Str(x,y,Entery_Success,sizeof(Entery_Success),16)
#define ENTERY_ERROR(x,y)						Write_OLED_Bufeer_Str(x,y,Entery_Error,sizeof(Entery_Error),16)
#define NOT_FINE_FINGER(x,y)				Write_OLED_Bufeer_Str(x,y,Not_Find_Finger,sizeof(Not_Find_Finger),16)
#define ENTER_OK(x,y)								Write_OLED_Bufeer_Str(x,y,Entery_OK,sizeof(Entery_OK),16)
#define IDENTIFICATION_RIGHT(x,y)		Write_OLED_Bufeer_Str(x,y,Correct_Identification,sizeof(Correct_Identification),16)
#define IDENTIFICATION_ERROR(x,y)		Write_OLED_Bufeer_Str(x,y,Identification_Error,sizeof(Identification_Error),16)
//#define WELCOME(x,y)								Write_OLED_Bufeer_Str(x,y,Welcome,sizeof(Welcome),16);

//uint8_t SPI2_ReadWriteByte(uint8_t TxData);
uint8_t LCD_SPI2_WrCmd(uint8_t cmd);
uint8_t LCD_SPI2_WrData(uint8_t data);
void LCD_SPI2_Set_Pos(uint8_t x, uint8_t y);
void LCD_SPI2_Fill(uint8_t bmp_dat);
void SPI2_Init(void);
void LCD_SPI2_Init(void);
void LCD_SPI2_P6x8Num(uint8_t x, uint8_t y, uint16_t data);
void LCD_SPI2_P8x16Str(uint8_t x,uint8_t y,char ch[]);

void LCD_SPI2_P16x16Str(uint8_t x, uint8_t y);
void LCD_SPI_Display_ALL(void);
void Write_OLED_Bufeer_Str(uint8_t x, uint8_t y, unsigned char *str, size_t size, size_t size_x);
void Clear_OLED_Zone(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void Battery_Display(uint8_t x, uint8_t y, uint8_t vaule);
void NRF_EN_Display(uint8_t x, uint8_t y);
void Display_Dividing_Line(uint8_t y);
void Display_Fingerprint_Flag(uint8_t x, uint8_t y);
void Display_P16x16_Number(uint8_t x, uint8_t y, uint8_t num);

#endif
