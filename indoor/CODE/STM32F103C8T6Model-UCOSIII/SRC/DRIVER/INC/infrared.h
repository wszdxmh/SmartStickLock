
#ifndef __INFRARED_H_
#define __INFRARED_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "sys.h"

#define LIG_HIGHT()		TIM_Cmd(TIM4, ENABLE)
#define LIG_LOW()			TIM_Cmd(TIM4, DISABLE)

#define HIGHT()				{LIG_HIGHT();Delay_us(600);LIG_LOW();Delay_us(1600);}
#define LOW()					{LIG_HIGHT();Delay_us(600);LIG_LOW();Delay_us(600);}

#define LEFT(x)						\
				{									\
						if(x & 0x01)	\
							HIGHT()			\
						else					\
							LOW()				\
						x >>= 1;			\
				}

static uint8_t Temperature[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e};
static uint8_t Verification_Code[]={0x60,0x70,0x80,0x90,0xA0,0xB0,0xC0,0xD0,0xE0,0xF0,0x00,0x10,0x20,0x30,0x40};
				
void Delay_us(uint16_t us);
void Delay_ms(uint16_t ms);
void Remote_Init(uint16_t arr, uint16_t psc);
void Remote_Send(void);
void Set_Temp(uint8_t temp);
void Air_Conditioner_Off(void);

#endif
