
#ifndef __MPUIIC_H_
#define __MPUIIC_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "sys.h"

//IO��������
#define MPU_SDA_IN()	{GPIOB->CRH &= 0xFFF0FFFF; GPIOB->CRH |= 0x01 << 19;}
#define MPU_SDA_OUT()	{GPIOB->CRH &= 0xFFF0FFFF; GPIOB->CRH |= 0x03 << 16;}

//IO������
#define MPU_IIC_SCL		PBout(13)
#define MPU_IIC_SDA		PBout(12)
#define MPU_READ_SDA	PBin(12)//SDA����

//���庯����
#define MPU_IIC_DELAY()	{delay_us(2);}

void MPU_IIC_Init(void);
void MPU_IIC_Start(void);
void MPU_IIC_Stop(void);
unsigned char MPU_IIC_Wait_Ack(void);
void MPU_IIC_Ack(void);
void MPU_IIC_NAck(void);
void MPU_IIC_Send_Byte(unsigned char txd);
unsigned char MPU_IIC_Read_Byte(unsigned char ack);

#endif
