
#ifndef __DS18B20_H
#define __DS18B20_H 

#include "sys.h"
#include "delay.h"

//IO方向设置
#define DS18B20_IO_IN()  {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=1<<31;}
#define DS18B20_IO_OUT() {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=3<<28;}
//IO操作函数											   
#define	DS18B20_DQ_OUT PAout(15) //数据端口	PA15
#define	DS18B20_DQ_IN  PAin(15)  //数据端口	PA15

void Delay_us(uint16_t us);
u8 DS18B20_Init(void);			//初始化DS18B20
short DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20

#endif
